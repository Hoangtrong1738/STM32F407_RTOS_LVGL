#include "spi.h"
/*
void spi_active_slave()
{
    u32* GPIOE_ODR = (u32*)(0x40021014);
    //set LOW for PE3
    *GPIOE_ODR &= ~(1 << 3);
}

void spi_inactive_slave()
{   
     u32* GPIOE_ODR = (u32*)(0x40021014);
    //set HIGH for PE3
    *GPIOE_ODR |= (1 << 3);  

}
void spi_init()
{
    //set PA5(alterhate function _SPI1_SCK), PA6(alternate function - SPI_MISO), PA7(alternate function - SIMO)
    GPIOA_PCLK_EN();
    u32 *GPIOA_MODER = (u32*)(0x40020000);
    u32 *GPIOA_AFRL = (u32*)(0x40020020);

    *GPIOA_MODER |= (0b10 << 10) | (0b10 << 12) | (0b10 << 14);
    *GPIOA_AFRL |= (5 << 20) | (5 << 24) | (5 << 28);
    //SET PE3 in output mode

    GPIOE_PCLK_EN();
    u32 *GPIOE_MODER = (u32*)(0x40021000);
    *GPIOE_MODER |= (0b01 << 6);
    //init spi in master
    SPI1_PCLK_EN(); // enable pclk 84MHz
    u32 *CR1 = (u32*)(0x40013000);
    *CR1 |= (0b010 << 3); // set baudrate 42MHz(84 / 2)
    *CR1 |= 1 << 2; // SET master mode for spi1
    *CR1 |= (1 << 8) | (1 << 9); // SS pin is controlled by GPIO without SPI

    *CR1 |= 1 << 6; // enable SPI1
    spi_inactive_slave();
}

char spi_read_data(char reg)
{
    u32 *DR = (u32*)(0x4001300c);
    u32 *SR = (u32*)(0x40013008);

    //active slave - set PE3 to LOW
    spi_active_slave();

    //send reg to slave - write reg value to DR of SPI1
    while(((*SR >> 1) & 1) != 1 ); // wait TX empty to write data  DR
    *DR = reg | (1 << 7);
    while(((*SR >> 1) & 1) == 1); // wait data be transfered to TX bufer 
    while(((*SR >> 0) & 1) != 1); // wait RXNE not empty (has recv data) 
    while(((*SR >> 7) & 1) == 1);  // wait not busy

    // clear spam data - read data from DR
    u8 temp = *DR;
    // send clock for slave to slave send data to master, write dummy data ( 0x00 or 0xff) to DR
    while(((*SR >> 1) & 1) != 1 ); // wait TX empty to write data  DR
    *DR = 0xff;
    while(((*SR >> 1) & 1) == 1); // wait data be transfered to TX bufer 
    while(((*SR >> 0) & 1) != 1); // wait RXNE not empty (has recv data) 
    while(((*SR >> 7) & 1) == 1);  // wait not busy
    // read data from  DR
    temp = *DR;
    // inactive slave - set PE3 to HIGH
    spi_inactive_slave();
    
    return temp;
}

void spi_write_data(char reg, char data)
{
    u32 *DR = (u32*)(0x4001300c);
    u32 *SR = (u32*)(0x40013008);
    // active slave - set to low
    spi_inactive_slave();
    // send reg to slave - write reg value to DR of SPI1 
// cho biet 1 da trong
    while(((*SR >> 1 ) & 1) != 1); // TX empty  to write data DR 
    *DR = reg;
    while(((*SR >> 1) & 1) == 1); // wait data be transfered to TX buffer
    while (((*SR >> 0) & 1) != 1); // wait RXNE not empty(has recv data) to read data
    while(((*SR >> 7) & 1) == 1); //  wait not busy 

    // clear spam data - read data from DR
    u8 temp = *DR;// send clock for slave to slave send data to master, write dummy data ( 0x00 or 0xff) to DR
    while(((*SR >> 1) & 1) != 1 ); // wait TX empty to write data  DR
    *DR = data;
    while(((*SR >> 1) & 1) == 1); // wait data be transfered to TX bufer 
    while(((*SR >> 0) & 1) != 1); // wait RXNE not empty (has recv data) 
    while(((*SR >> 7) & 1) == 1);  // wait not busy
    // read data from  DR
    temp = *DR;
    // inactive slave - set PE3 to HIGH
    spi_inactive_slave();
    
}
*/
#define SPI1_TFT_DMA_STREAM          3U
#define SPI1_TFT_DMA_CHANNEL         3U
#define DMA2_STREAM3_IRQ_NUMBER      59U
#define SPI1_DMA_MAX_LEN             65535U

static volatile u8 spi1_dma_tx_busy = 0;

static void spi1_gpio_init(void)
{
    GPIOA_PCLK_EN();

    GPIOA->MODER &= ~((0x3U << (PIN_5 * 2U)) |
                      (0x3U << (PIN_6 * 2U)) |
                      (0x3U << (PIN_7 * 2U)));
    GPIOA->MODER |= ((GPIO_MODE_ALTFM << (PIN_5 * 2U)) |
                     (GPIO_MODE_ALTFM << (PIN_6 * 2U)) |
                     (GPIO_MODE_ALTFM << (PIN_7 * 2U)));

    GPIOA->OTYPER &= ~((1U << PIN_5) | (1U << PIN_6) | (1U << PIN_7));

    GPIOA->OSPEEDR &= ~((0x3U << (PIN_5 * 2U)) |
                        (0x3U << (PIN_6 * 2U)) |
                        (0x3U << (PIN_7 * 2U)));
    GPIOA->OSPEEDR |= ((GPIO_SPEED_HIGH << (PIN_5 * 2U)) |
                       (GPIO_SPEED_HIGH << (PIN_6 * 2U)) |
                       (GPIO_SPEED_HIGH << (PIN_7 * 2U)));

    GPIOA->PUPDR &= ~((0x3U << (PIN_5 * 2U)) |
                      (0x3U << (PIN_6 * 2U)) |
                      (0x3U << (PIN_7 * 2U)));

    GPIOA->AFR[0] &= ~((0xFU << (PIN_5 * 4U)) |
                       (0xFU << (PIN_6 * 4U)) |
                       (0xFU << (PIN_7 * 4U)));
    GPIOA->AFR[0] |= ((5U << (PIN_5 * 4U)) |
                      (5U << (PIN_6 * 4U)) |
                      (5U << (PIN_7 * 4U)));
}

static void spi1_dma_irq_enable(void)
{
    if(DMA2_STREAM3_IRQ_NUMBER < 64U)
    {
        *NVIC_ISER1 |= (1U << (DMA2_STREAM3_IRQ_NUMBER - 32U));
    }
}

void spi_active_slave(void)
{
    GPIOE->ODR &= ~(1U << PIN_3);
}

void spi_inactive_slave(void)
{
    GPIOE->ODR |= (1U << PIN_3);
}

void spi_init(void)
{
    spi1_lcd_init();

    GPIOE_PCLK_EN();
    GPIOE->MODER &= ~(0x3U << (PIN_3 * 2U));
    GPIOE->MODER |= (GPIO_MODE_OUT << (PIN_3 * 2U));
    spi_inactive_slave();
}

void spi1_wait_idle(void)
{
    while((SPI1->SR & SPI_TXE_FLAG) == 0U)
    {
    }
    while((SPI1->SR & SPI_BUSY_FLAG) != 0U)
    {
    }
}

void spi1_enable_tx_only(void)
{
    spi1_wait_idle();
    SPI1->CR1 &= ~(1U << SPI_CR1_SPE);
    SPI1->CR1 |= (1U << SPI_CR1_BIDIMODE) | (1U << SPI_CR1_BIDIOE);
    SPI1->CR1 |= (1U << SPI_CR1_SPE);
}

void spi1_enable_full_duplex(void)
{
    spi1_wait_idle();
    SPI1->CR1 &= ~(1U << SPI_CR1_SPE);
    SPI1->CR1 &= ~((1U << SPI_CR1_BIDIMODE) | (1U << SPI_CR1_BIDIOE));
    SPI1->CR1 |= (1U << SPI_CR1_SPE);
    (void)SPI1->DR;
    (void)SPI1->SR;
}

void spi1_lcd_init(void)
{
    spi1_gpio_init();
    SPI1_PCLK_EN();
    DMA2_PCLK_EN();

    SPI1->CR1 = 0;
    SPI1->CR2 = 0;
    SPI1->CR1 |= (SPI_SCLK_SPEED_DIV2 << SPI_CR1_BR);
    SPI1->CR1 |= (1U << SPI_CR1_MSTR);
    SPI1->CR1 |= (1U << SPI_CR1_SSI) | (1U << SPI_CR1_SSM);
    SPI1->CR1 |= (1U << SPI_CR1_BIDIMODE) | (1U << SPI_CR1_BIDIOE);
    SPI1->CR1 |= (1U << SPI_CR1_SPE);

    DMA2->S[SPI1_TFT_DMA_STREAM].CR &= ~(1U << DMA_SxCR_EN);
    while((DMA2->S[SPI1_TFT_DMA_STREAM].CR & (1U << DMA_SxCR_EN)) != 0U)
    {
    }
    DMA2->LIFCR = DMA_STREAM3_FLAG_MASK;
    spi1_dma_irq_enable();
    spi1_dma_tx_busy = 0;
}

void spi1_write8(u8 data)
{
    spi1_enable_tx_only();
    while((SPI1->SR & SPI_TXE_FLAG) == 0U)
    {
    }
    *((_vo u8 *)&SPI1->DR) = data;
    spi1_wait_idle();
}

void spi1_write_buffer(const u8 *data, u32 len)
{
    spi1_enable_tx_only();
    while(len-- > 0U)
    {
        while((SPI1->SR & SPI_TXE_FLAG) == 0U)
        {
        }
        *((_vo u8 *)&SPI1->DR) = *data++;
    }
    spi1_wait_idle();
}

void spi1_write_dma_start(const u8 *data, u32 len)
{
    if((data == 0) || (len == 0U))
    {
        return;
    }

    spi1_write_dma_wait();
    spi1_enable_tx_only();

    DMA2->LIFCR = DMA_STREAM3_FLAG_MASK;
    DMA2->S[SPI1_TFT_DMA_STREAM].CR = 0;
    DMA2->S[SPI1_TFT_DMA_STREAM].PAR = (u32)&SPI1->DR;
    DMA2->S[SPI1_TFT_DMA_STREAM].M0AR = (u32)data;
    DMA2->S[SPI1_TFT_DMA_STREAM].NDTR = len;
    DMA2->S[SPI1_TFT_DMA_STREAM].FCR = 0;
    DMA2->S[SPI1_TFT_DMA_STREAM].CR =
        (SPI1_TFT_DMA_CHANNEL << DMA_SxCR_CHSEL) |
        (DMA_DIR_MEMORY_TO_PERIPHERAL << DMA_SxCR_DIR) |
        (1U << DMA_SxCR_MINC) |
        (DMA_PRIORITY_HIGH << DMA_SxCR_PL) |
        (1U << DMA_SxCR_TCIE) |
        (1U << DMA_SxCR_TEIE);

    spi1_dma_tx_busy = 1;
    SPI1->CR2 |= (1U << SPI_CR2_TXDMAEN);
    DMA2->S[SPI1_TFT_DMA_STREAM].CR |= (1U << DMA_SxCR_EN);
}

void spi1_write_dma_wait(void)
{
    while(spi1_dma_tx_busy != 0U)
    {
    }
}

void spi1_write_dma(const u8 *data, u32 len)
{
    while(len > 0U)
    {
        u32 chunk = (len > SPI1_DMA_MAX_LEN) ? SPI1_DMA_MAX_LEN : len;
        spi1_write_dma_start(data, chunk);
        spi1_write_dma_wait();
        data += chunk;
        len -= chunk;
    }
}

u8 spi1_dma_tx_is_busy(void)
{
    return spi1_dma_tx_busy;
}

u8 spi1_transfer_byte(u8 data)
{
    spi1_write_dma_wait();
    spi1_enable_full_duplex();

    while((SPI1->SR & SPI_TXE_FLAG) == 0U)
    {
    }
    *((_vo u8 *)&SPI1->DR) = data;

    while((SPI1->SR & SPI_RXNE_FLAG) == 0U)
    {
    }
    data = *((_vo u8 *)&SPI1->DR);

    spi1_wait_idle();
    return data;
}

char spi_read_data(char reg)
{
    u8 temp;

    spi_active_slave();
    (void)spi1_transfer_byte((u8)reg | 0x80U);
    temp = spi1_transfer_byte(0xFFU);
    spi_inactive_slave();

    return (char)temp;
}

void spi_write_data(char reg, char data)
{
    spi_active_slave();
    (void)spi1_transfer_byte((u8)reg);
    (void)spi1_transfer_byte((u8)data);
    spi_inactive_slave();
}

void DMA2_Stream3_IRQHandler(void)
{
    if((DMA2->LISR & (DMA_STREAM3_TCIF | DMA_STREAM3_TEIF)) != 0U)
    {
        DMA2->S[SPI1_TFT_DMA_STREAM].CR &= ~(1U << DMA_SxCR_EN);
        SPI1->CR2 &= ~(1U << SPI_CR2_TXDMAEN);
        spi1_wait_idle();
        DMA2->LIFCR = DMA_STREAM3_FLAG_MASK;
        spi1_dma_tx_busy = 0;
    }
}
