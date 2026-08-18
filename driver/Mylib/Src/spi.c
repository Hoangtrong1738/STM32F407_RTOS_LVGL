#include "spi.h"

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
    *DR = reg;
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