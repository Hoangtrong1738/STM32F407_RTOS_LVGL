#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_
#include <stdint.h>
#include "stddef.h"

#define _vo   volatile
#define __weak __attribute__((weak))
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
/*
 * ARM Cortex Mx Processor NVIC ISERx register Addresses
 *
 */

#define NVIC_ISER0            ((_vo uint32_t*)0xE000E100) // biến nó thành con trỏ số nguyên không dấu dễ thay đổi, chỉ cần viết _vo là xong
#define NVIC_ISER1            ((_vo uint32_t*)0xE000E104)
#define NVIC_ISER2            ((_vo uint32_t*)0xE000E108)
#define NVIC_ISER3            ((_vo uint32_t*)0xE000E10c)

/*
 * ARM Cortex Mx Processor NVIC ICERx register Addresses
 *
 */

#define NVIC_ICER0            ((_vo uint32_t*)0xE000E180)
#define NVIC_ICER1            ((_vo uint32_t*)0xE000E184)
#define NVIC_ICER2            ((_vo uint32_t*)0xE000E188)
#define NVIC_ICER3            ((_vo uint32_t*)0xE000E18C)
/*
 * ARM Cortex Mx Processor Priority Register Address Calculation
 */
#define NVIC_PR_BASE_ADDR     ((_vo uint32_t*)0xE000E400)

/*
 * ARM Cortex Mx Processor number of priority bits implemented in Priority Register
 */
#define NO_PR_BITS_IMPLEMENTED      4
// dn ban do bo nho (Memory Map)
// base addresses of Flash and SRAM memories
#define FLASH_BASEADDR         0x08000000U
#define SRAM1_BASEADDR         0x20000000U
#define SRAM2_BASEADDR         0x2001C000U
#define SRAM                   SRAM1_BASEADDR
#define ROM_BASEADDR           0x1FFF0000U

//AHBx and APBx Bus Peripheral base assresses
//dn addr base cua cac mien bus khac nhau
#define PERIPH_BASEADDR            0x40000000U
#define APB1PERIPH_BASEADDR        PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR        0x40010000U
#define AHB1PERIPH_BASEADDR        0x40020000U
#define AHB2PERIPH_BASEADDR        0X50000000U

// buoc tiep theo xac dinh addr base co so cua cac ngoai vi AHB1,APB1 VA APB2

/*
  base addresses of perpherals which are hanging on AHB1 bus
*/
 // Xac dinh addr base cua cac thiet bi ngoai vi dang ket noi voi bus AHB1
 /*
 Lam the nao de tinh toan dia chi co so GPIOA
 chung ta lay day lam co so sau do cong them phan bu

 Dô lech là khoang cách addresses tính tu addr base
 "khoang cach tu diem bat dau cua bus / peripheral toi thiet bi hoac thanh ghi can truy cap
 addr thuc = base address + offset
 base address : dia chi bat dau cua mot vung (bus / perpheral)
 offset: phan dich them de toi dung thiêt bi hoac thanh ghi bên trong vùng do
 */
#define GPIOA_BASEADDR        (AHB1PERIPH_BASEADDR + 0x0000)  // day la dia chi that trong khong gian nho do hang  dinh nghia
#define GPIOB_BASEADDR        (AHB1PERIPH_BASEADDR + 0x0400)
#define GPIOC_BASEADDR        (AHB1PERIPH_BASEADDR + 0x0800)
#define GPIOD_BASEADDR        (AHB1PERIPH_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR        (AHB1PERIPH_BASEADDR + 0x1000)
#define GPIOF_BASEADDR        (AHB1PERIPH_BASEADDR + 0x1400)
#define GPIOG_BASEADDR        (AHB1PERIPH_BASEADDR + 0x1800)
#define GPIOH_BASEADDR        (AHB1PERIPH_BASEADDR + 0x1C00)
#define GPIOI_BASEADDR        (AHB1PERIPH_BASEADDR + 0x2000)
#define RCC_BASEADDR          (AHB1PERIPH_BASEADDR + 0x3800)

/*
  Base addresses of peripherals which are hanging on APB1 bus
	dia chi co so cua cac thiet bi ngoai vi dang ket noi voi bus APB1
*/

#define I2C1_BASE         (APB1PERIPH_BASEADDR + 0x5400)
#define I2C2_BASE         (APB1PERIPH_BASEADDR + 0x5800)
#define I2C3_BASE         (APB1PERIPH_BASEADDR + 0x5C00)

#define SPI2_BASE         (APB1PERIPH_BASEADDR + 0x3800)
#define SPI3_BASE         (APB1PERIPH_BASEADDR + 0x3C00)

#define USART2_BASE       (APB1PERIPH_BASEADDR + 0x4400)
#define USART3_BASE       (APB1PERIPH_BASEADDR + 0x4800)
#define UART4_BASE        (APB1PERIPH_BASEADDR + 0x4C00)
#define UART5_BASE        (APB1PERIPH_BASEADDR + 0x5000)

#define TIM2_BASEADDR      (APB1PERIPH_BASEADDR + 0x0000)
#define TIM3_BASEADDR      (APB1PERIPH_BASEADDR + 0x0400)
#define TIM4_BASEADDR      (APB1PERIPH_BASEADDR + 0x0800)
#define TIM5_BASEADDR      (APB1PERIPH_BASEADDR + 0x0C00)
#define TIM6_BASEADDR      (APB1PERIPH_BASEADDR + 0x1000)
#define TIM7_BASEADDR      (APB1PERIPH_BASEADDR + 0x1400)
#define TIM12_BASEADDR      (APB1PERIPH_BASEADDR + 0x1800)
#define TIM13_BASEADDR      (APB1PERIPH_BASEADDR + 0x1C00)
#define TIM14_BASEADDR      (APB1PERIPH_BASEADDR + 0x2000)

/*
  Base addresses of peripherals which are hanging on APB2 bus
	dia chi co so cua cac ngoai vi dang ket noi voi bus APB2
*/

#define EXTI_BASE        (APB2PERIPH_BASEADDR + 0x3C00)
#define SPI1_BASE             (APB2PERIPH_BASEADDR + 0x3000)
#define SYSCFG_BASE           (APB2PERIPH_BASEADDR + 0x3800)
#define USART1_BASE           (APB2PERIPH_BASEADDR + 0x1000)
#define USART6_BASE           (APB2PERIPH_BASEADDR + 0x1400)

#define TIM1_BASEADDR      (APB2PERIPH_BASEADDR + 0x0000)
#define TIM8_BASEADDR      (APB2PERIPH_BASEADDR + 0x0400)
#define TIM9_BASEADDR      (APB2PERIPH_BASEADDR + 0x4000)
#define TIM10_BASEADDR      (APB2PERIPH_BASEADDR + 0x4400)
#define TIM11_BASEADDR      (APB2PERIPH_BASEADDR + 0x4800)


#define DMA1_BASEADDR     (AHB1PERIPH_BASEADDR + 0x6000U)
#define DMA2_BASEADDR     (AHB1PERIPH_BASEADDR + 0x6400U)
#define DMA2D_BASEADDR    (AHB1PERIPH_BASEADDR + 0xB000U)






// peripheral register definition structures

typedef struct
{
	_vo uint32_t MODER;                   //GPIO port mode registe  // address offset : 0x00
	_vo uint32_t OTYPER;                                            // address offset : 0x04
	_vo uint32_t OSPEEDR;
	_vo uint32_t PUPDR;
	_vo uint32_t IDR;
	_vo uint32_t ODR;
	_vo uint32_t BSRR;
	_vo uint32_t LCKR;
	_vo uint32_t AFR[2];              // AFR[0] :GPIO alternate function low register / AFR[1] :GPIO alternate function high register
}GPIO_RegDef_t;

typedef struct
{
	_vo uint32_t CR;
	_vo uint32_t PLLCFGR;
	_vo uint32_t CFGR;
	_vo uint32_t CIR;
	_vo uint32_t AHB1RSTR;
	_vo uint32_t AHB2RSTR;
	_vo uint32_t AHB3RSTR;
	uint32_t RESERVED0;
	_vo uint32_t APB1RSTR;
	_vo uint32_t APB2RSTR;
	uint32_t RESERVED1[2];
	_vo uint32_t AHB1ENR;
	_vo uint32_t AHB2ENR;
	_vo uint32_t AHB3ENR;
	uint32_t RESERVED2;
	_vo uint32_t APB1ENR;
	_vo uint32_t APB2ENR;
	uint32_t RESERVED3[2];
	_vo uint32_t AHB1LPENR;
	_vo uint32_t AHB2LPENR;
	_vo uint32_t AHB3LPENR;
	uint32_t RESERVED4;
	_vo uint32_t APB1LPENR;
	_vo uint32_t APB2LPENR;
	uint32_t RESERVED5[2];
	_vo uint32_t BDCR;
	_vo uint32_t CSR;
	uint32_t RESERVED6[2];
	_vo uint32_t SSCGR;
	_vo uint32_t PLLI2SCFGR;


}RCC_RegDef_t;
//

// peripheral register definition structures for EXTI
// EXTI là bộ điều khiển ngắn ngoài .
//Nó cho phép một chân GPIO tạo ra ngắt khi có tín hiệu thay đổi

typedef struct
{
	_vo uint32_t IMR;                // address offset: 0x00
	_vo uint32_t EMR;                //address offset: 0x04
	_vo uint32_t RTSR;               //                0x08
	_vo uint32_t FTSR;               //   0x0C
	_vo uint32_t SWIER;              // 0x10
	_vo uint32_t PR;                 // 0x14
}EXTI_RegDef_t;

/*
 * Configuration structure for SPIx peripheral
 */

typedef struct
{
	u8 SPI_DeviceMode;
	u8 SPI_BusConfig;
	u8 SPI_DFF;
	u8 SPI_SclkSpeed;
	u8 SPI_CPOL;
	u8 SPI_CPHA;
	u8 SPI_SSM;
}SPI_Config_t;

// peripheral register definition structures for SPI
typedef struct
{
	_vo u32 CR1;
	_vo u32 CR2;
	_vo u32 SR;
	_vo u32 DR;
	_vo u32 CRCPR;
	_vo u32 RXCRCR;
	_vo u32 TXCRCR;
	_vo u32 I2SCFGR;
	_vo u32 I2SPR;
}SPI_RegDef_t;
/*
 * Handle structure for SPIx peripheral
 */

typedef struct
{
	SPI_RegDef_t *pSPIx;
	SPI_Config_t SPIConfig;
	u8 *pTxBuffer;
	u8 *pRxBuffer;
	u32 TxLen;
	u32 RxLen;
	u8 TxState;
	u8 RxState;

}SPI_Handle_t;





/*
 * Configuration structure for I2Cx peripheral
 */
/*
typedef struct
{
	u32 I2C_SCLSpeed;
	u8 I2C_DeviceAddress;
	u8 I2C_ACKControl;
	u16 I2C_FMDutyCycle;

}I2C_Config_t;

*/
/*
 * Handle structure for I2Cx PERIPHERAL
 */
 /*
typedef struct
{
	I2C_RegDef_t *pI2Cx;
	I2C_Config_t I2C_Config;

}I2C_Handle_t;
*/
 /*
  * I2C_SCL Speed
  */
#define I2C_SCL_SPEED_SM   		100000
#define I2C_SCL_SPEED_FM4K 		400000
#define I2C_SCL_SPEED_FM2K      200000

/*
 * I2C_ACKControl
 */
#define I2C_ACK_ENABLE            1
#define I2C_ACK_DISABLE           0

/*
 *I2C FMDutyCycle
 */
#define I2C_FM_DUTY_2            0
#define I2C_FM_DUTY_16_9         1
/*****************************
 * Bit position definitions of I2C peripheral
 *
 * *******************************
 */

/*
 * bit position definitions I2C_CR1
 */

#define I2C_CR1_PE             0
#define I2C_CR1_NOSTRETCH      7
#define I2C_CR1_START          8
#define I2C_CR1_STOP           9
#define I2C_CR1_ACK            10
#define I2C_CR1_SWRST          15

/*
 * bit position definition I2C_CR2
 *
 */
#define I2C_CR2_FREQ           0
#define I2C_CR2_ITERREN        8
#define I2C_CR2_ITEVTEN        9
#define I2C_CR2_ITBUFEN        10

/*
 * bit position definitIon I2C_OAR1
 *
 */
#define I2C_OAR1_ADD0           0
#define I2C_OAR1_ADD71          1
#define I2C_OAR1_ADD98          8
#define I2C_OAR1_ADDMODE        15

/*
 * bit position definitions I2C_SR1
 *
 */
#define I2C_SR1_SB             0
#define I2C_SR1_ADDR           1
#define I2C_SR1_BTF            2
#define I2C_SR1_ADD10          3
#define I2C_SR1_STOPF          4
#define I2C_SR1_RXNE           6
#define I2C_SR1_TXE			   7
#define I2C_SR1_BERR		   8
#define I2C_SR1_ARLO           9
#define I2C_SR1_AF             10
#define I2C_SR1_OVR            11
#define I2C_SR1_TIMEOUT        14

/*
 * bit position definition I2C_SR2
 */

#define I2C_SR2_MSL            0
#define I2C_SR2_BUSY 		   1
#define I2C_SR2_TRA            2
#define I2C_SR2_GENCALL        4
#define I2C_SR2_DUALF          7

/*
 * bit position definition I2C_CCR
 *
 */
#define I2C_CCR_CCR      0
#define I2C_CCR_DUTY     14
#define I2C_CCR_F5       15
/*
 * SPI Application states
 */

#define SPI_READY       0
#define SPI_BUSY_IN_RX  1
#define SPI_BUSY_IN_TX  2
/*
 * Possible SPI_application events
 */
#define SPI_EVENT_TX_CMPLT       1
#define SPI_EVENT_RX_CMPLT       2
#define SPI_EVENT_OVR_ERR        3
#define SPI_EVENT_CRC_ERR        4
/*
 * SPI_DeviceMode
 */
#define SPI_DEVICE_MODE_MASTER   1
#define SPI_DEVICE_MODE_SLAVE    0

/*
 * Bit position definitions SPI_CR1
 *
 */

#define SPI_CR1_CPHA                     0
#define SPI_CR1_CPOL                     1
#define SPI_CR1_MSTR                     2
#define SPI_CR1_BR                       3
#define SPI_CR1_SPE                      6
#define SPI_CR1_LSBFIRST                 7
#define SPI_CR1_SSI                      8
#define SPI_CR1_SSM                      9
#define SPI_CR1_RXONLY                   10
#define SPI_CR1_DFF                      11
#define SPI_CR1_CRCNEXT                  12
#define SPI_CR1_CRCEN                    13
#define SPI_CR1_BIDIOE                   14
#define SPI_CR1_BIDIMODE                 15

/*
 * Bit position definitions SPI_CR2
 *
 */

#define SPI_CR2_TXEIE                   7
#define SPI_CR2_RXEIE                   6
#define SPI_CR2_ERRIE                   5
#define SPI_CR2_FRF                       4
#define SPI_CR2_SSOE                      2
#define SPI_CR2_TXDMAEN                   1
#define SPI_CR2_RXDMAEN                   0

/*
 * Bit position definitions SPI_SR
 *
 */
#define SPI_SR_FRE     8
#define SPI_SR_BSY     7
#define SPI_SR_OVR     6
#define SPI_SR_MODF    5
#define SPI_SR_CRCERR  4
#define SPI_SR_UDR     3
#define SPI_SR_CHSIDE  2
#define SPI_SR_TXE     1
#define SPI_SR_RXNE     0

/*
 * SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FD                1
#define SPI_BUS_CONFIG_HD                2
#define SPI_BUS_CONFIG_SIMPLEX_TXONLY    3
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY    4

/*
 * SPI_SclkSpeed
 */
#define SPI_SCLK_SPEED_DIV2     0
#define SPI_SCLK_SPEED_DIV4     1
#define SPI_SCLK_SPEED_DIV8     2
#define SPI_SCLK_SPEED_DIV16     3
#define SPI_SCLK_SPEED_DIV32     4
#define SPI_SCLK_SPEED_DIV64     5
#define SPI_SCLK_SPEED_DIV128    6
#define SPI_SCLK_SPEED_DIV256    7

/*
 * SPI_DFF
 */
#define SPI_DFF_8BIT    0
#define SPI_DFF_16BITS  1

/*
 * SPI_CPOL
 */

#define SPI_CPOL_HIGH  1
#define SPI_CPOL_LOW   0

/*
 * SPI_CPHA
 */
#define SPI_CPHA_HIGH  1
#define SPI_CPHA_LOW   0
/*
 * SPI_SSM
 */
#define SPI_SSM_EN    1
#define SPI_SSM_DI    0

/*
 *
 *
 */

#define SPI_TXE_FLAG  (1 << SPI_SR_TXE)
#define SPI_RXNE_FLAG (1 << SPI_SR_RXNE)
#define SPI_BUSY_FLAG (1 << SPI_SR_BSY)

typedef struct
{
	_vo u32 SR;
	_vo u32 DR;
	_vo u32 BRR;
	_vo u32 CR1;
	_vo u32 CR2;
	_vo u32 CR3;
	_vo u32 GTPR;


}USART_RegDef_t;
/*
 *
 * peripheral register definition structure for SYSCFG
 * SYSCFG( System Configuration Controller) là khối cấu hình hệ thống
 */

typedef struct
{
	_vo uint32_t MEMRMP;                 // address offset: 0x00
	_vo uint32_t PMC;                  // 0x04
	_vo uint32_t EXTICR[4];
	uint32_t     RESERVED1[2];
	_vo uint32_t CMPCR;
	uint32_t     RESERVED2[2];
	_vo uint32_t CFGR;
}SYSCFG_RegDef_t;

/*
 * peripheral register definition structure for TIMER
 *
 */
// TIM2 AND TIM5
typedef struct
{
	_vo u32 CR1;                /*Offset:  */
	_vo u32 CR2;
	_vo u32 SMCR;
	_vo u32 DIER;
	_vo u32 SR;
	_vo u32 EGR;
	_vo u32 CCMR1;
	_vo u32 CCMR2;
	_vo u32 CCER;
	_vo u32 CNT;
	_vo u32 PSC;
	_vo u32 ARR;
	    u32 RESERVED1;
	_vo u32 CCR1;
	_vo u32 CCR2;
	_vo u32 CCR3;
	_vo u32 CCR4;
	    u32 RESERVED2;
	_vo u32 DCR;
	_vo u32 DMAR;
	_vo u32 OR; // of TIM2 AND TIM5

}TIM2_5_RegDef_t;
//TM3 AND TM4
typedef struct
{
	_vo u32 CR1;                /*Offset:  */
	_vo u32 CR2;
	_vo u32 SMCR;
	_vo u32 DIER;
	_vo u32 SR;
	_vo u32 EGR;
	_vo u32 CCMR1;
	_vo u32 CCMR2;
	_vo u32 CCER;
	_vo u32 CNT;
	_vo u32 PSC;
	_vo u32 ARR;
	    u32 RESERVED1;
	_vo u32 CCR1;
	_vo u32 CCR2;
	_vo u32 CCR3;
	_vo u32 CCR4;
	    u32 RESERVED2;
	_vo u32 DCR;
	_vo u32 DMAR;

}TIM3_4_RegDef_t;

typedef struct
{
	_vo u32 CR1;          /*Offset:  */
    _vo u32 CR2;
    _vo u32 SMCR;
	_vo u32 DIER;
	_vo u32 SR;
	_vo u32 EGR;
	_vo u32 CCMR1;
	_vo u32 CCMR2;
	_vo u32 CCER;
	_vo u32 CNT;
	_vo u32 PSC;
	_vo u32 ARR;
	_vo u32 RCR;
	_vo u32 CCR1;
	_vo u32 CCR2;
	_vo u32 CCR3;
	_vo u32 CCR4;
	_vo u32 BDTR;
	_vo u32 DCR;
	_vo u32 DMAR;

}TIM_Adv_RegDef_t;

typedef struct{ // TIMER9/12
	_vo u32 CR1;          /*Offset:  */
	    u32 RESERVED1;
	_vo u32 SMCR;
    _vo u32 DIER;
    _vo u32 SR;
	_vo u32 EGR;
	_vo u32 CCMR1;
	    u32 RESERVED2;
	_vo u32 CCER;
	_vo u32 CNT;
	_vo u32 PSC;
	_vo u32 ARR;
	    u32 RESERVED3;
	_vo u32 CCR1;
	_vo u32 CCR2;
	    u32 RESERVED4[5];
}TIM_9and12_RegDef_t;


typedef struct
{
	_vo u32 CR1;
	_vo u32 SMCR;
	_vo u32 DIER;
	_vo u32 SR;
	_vo u32 EGR;
	_vo u32 CCMR1;
	    u32 RESERVED1;
	_vo u32 CCER;
	_vo u32 CNT;
	_vo u32 PSC;
	_vo u32 ARR;
	    u32 RESERVED2;
	_vo u32 CCR1;
	    u32 RESERVED3[2];
	_vo u32 OR;


}TIM_10to14_RegDef_t;
typedef struct{ // TIMER10/11/13/14
	_vo u32 CR1;          /*Offset:  */
	_vo u32 CR2;
	_vo u32 SMCR;
    _vo u32 DIER;
    _vo u32 SR;
	_vo u32 EGR;
	_vo u32 CCMR1;
	_vo u32 CCMR2;
	    u32 RESERVED1;
	_vo u32 CCER;
	_vo u32 CNT;
	_vo u32 PSC;
	_vo u32 ARR;
	    u32 RESERVED2;
	_vo u32 CCR1;
	    u32 RESERVED3[2];
	_vo u32 OR;

}TIM_Gp2_RegDef_t;

typedef struct
{
	_vo u32 CR1;          /*Offset:  */
	_vo u32 CR2;
	    u32 RESERVED1;
	_vo u32 DIER;
	_vo u32 SR;
	_vo u32 EGR;
	    u32 RESERVED2[3];
	_vo u32 CNT;
	_vo u32 PSC;
	_vo u32 ARR;
}TIM_BASIC_RegDef_t;


typedef struct
{
	_vo u32 CR;
	_vo u32 NDTR;
	_vo u32 PAR;
	_vo u32 M0AR;
	_vo u32 M1AR;
	_vo u32 FCR;
}DMA_Stream_RegDef_t;

typedef struct
{
	_vo u32 LISR;
	_vo u32 HISR;
	_vo u32 LIFCR;
	_vo u32 HIFCR;
	DMA_Stream_RegDef_t S[8];
}DMA_RegDef_t;

/*
 * Bit position definitions DMA_SxCR
 */
#define DMA_SxCR_EN                       0
#define DMA_SxCR_DMEIE                    1
#define DMA_SxCR_TEIE                     2
#define DMA_SxCR_HTIE                     3
#define DMA_SxCR_TCIE                     4
#define DMA_SxCR_PFCTRL                   5
#define DMA_SxCR_DIR                      6
#define DMA_SxCR_CIRC                     8
#define DMA_SxCR_PINC                     9
#define DMA_SxCR_MINC                     10
#define DMA_SxCR_PSIZE                    11
#define DMA_SxCR_MSIZE                    13
#define DMA_SxCR_PINCOS                   15
#define DMA_SxCR_PL                       16
#define DMA_SxCR_DBM                      18
#define DMA_SxCR_CT                       19
#define DMA_SxCR_PBURST                   21
#define DMA_SxCR_MBURST                   23
#define DMA_SxCR_CHSEL                    25

#define DMA_DIR_PERIPHERAL_TO_MEMORY      0
#define DMA_DIR_MEMORY_TO_PERIPHERAL      1
#define DMA_DIR_MEMORY_TO_MEMORY          2

#define DMA_PRIORITY_LOW                  0
#define DMA_PRIORITY_MEDIUM               1
#define DMA_PRIORITY_HIGH                 2
#define DMA_PRIORITY_VERY_HIGH            3

/*
 * DMA2 Stream 3 flags in LISR/LIFCR.
 * SPI1_TX on STM32F407: DMA2 Stream 3 Channel 3.
 */
#define DMA_STREAM3_FLAG_MASK             0x0F400000U
#define DMA_STREAM3_TCIF                  (1U << 27)
#define DMA_STREAM3_TEIF                  (1U << 25)

typedef struct
{
	_vo u32 SR;          /*Offset:  */
	_vo u32 CR1;
	_vo u32 CR2;
	_vo u32 SMPR1;
	_vo u32 SMPR2;
	_vo u32 JOFR1;
	_vo u32 JOFR2;
	_vo u32 JOFR3;
	_vo u32 JOFR4;
	_vo u32 HTR;
	_vo u32 LTR;
	_vo u32 SQR1;         /*Offset:  */
	_vo u32 SQR2;
	_vo u32 SQR3;
	_vo u32 JSQR;
	_vo u32 JDR1;
	_vo u32 JDR2;
	_vo u32 JDR3;
	_vo u32 JDR4;
	_vo u32 DR;
	_vo u32 S2FCR;
	_vo u32 S3CR;
	_vo u32 S3NDTR;;          /*Offset:  */
	_vo u32 S3PAR;
	_vo u32 S3M0AR;
	_vo u32 S3M1AR;
	_vo u32 S3FCR;
	_vo u32 S4CR;
	_vo u32 S4NDTR;
	_vo u32 S4PAR;
	_vo u32 S4M0AR;
	_vo u32 S4M1AR;
	_vo u32 S4FCR;
	_vo u32 S5CR;
	_vo u32 S5NDTR;
	_vo u32 S5PAR;
	_vo u32 S5M0AR;
	_vo u32 S5M1AR;;          /*Offset:  */
	_vo u32 S5FCR;
	_vo u32 S6CR;
	_vo u32 S6NDTR;
	_vo u32 S6PAR;
	_vo u32 S6M0AR;
	_vo u32 S6M1AR;
	_vo u32 S6FCR;
	_vo u32 S7CR;
	_vo u32 S7NDTR;
	_vo u32 S7PAR;
	_vo u32 S7M0AR;
	_vo u32 S7M1AR;
	_vo u32 S7FCR;

}ADC_RegDef_t;

#define GPIOA      ((GPIO_RegDef_t*)GPIOA_BASEADDR)   // GPIO_RegDef_t *pGPIOA = (GPIO_RegDef_t*)0x40020000;
#define GPIOB      ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC      ((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD      ((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE      ((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF      ((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG      ((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH      ((GPIO_RegDef_t*)GPIOH_BASEADDR)
#define GPIOI      ((GPIO_RegDef_t*)GPIOI_BASEADDR)
#define RCC        ((RCC_RegDef_t*)RCC_BASEADDR)

#define EXTI       ((EXTI_RegDef_t*)EXTI_BASE)

#define SYSCFG     ((SYSCFG_RegDef_t*)SYSCFG_BASE)


#define TIM2        ((TIM2_5_RegDef_t *) TIM2_BASEADDR)
#define TIM5        ((TIM2_5_RegDef_t *) TIM5_BASEADDR)

#define TIM3        ((TIM3_4_RegDef_t *) TIM3_BASEADDR)
#define TIM4        ((TIM3_4_RegDef_t *) TIM4_BASEADDR)

#define TIM1        ((TIM_Adv_RegDef_t*) TIM1_BASEADDR)
#define TIM8        ((TIM_Adv_RegDef_t*) TIM8_BASEADDR)

#define TIM6        ((TIM_BASIC_RegDef_t*) TIM6_BASEADDR)
#define TIM7        ((TIM_BASIC_RegDef_t*) TIM7_BASEADDR)

#define TIM9        ((TIM_Gp1_RegDef_t*) TIM9_BASEADDR)
#define TIM12       ((TIM_Gp1_RegDef_t*) TIM12_BASEADDR)

#define TIM10        ((TIM_Gp2_RegDef_t*) TIM10_BASEADDR)
#define TIM11        ((TIM_Gp2_RegDef_t*) TIM11_BASEADDR)
#define TIM13        ((TIM_Gp2_RegDef_t*) TIM13_BASEADDR)
#define TIM14        ((TIM_Gp2_RegDef_t*) TIM14_BASEADDR)


#define SPI1        ((SPI_RegDef_t*)SPI1_BASE)
#define SPI2        ((SPI_RegDef_t*)SPI2_BASE)
#define SPI3        ((SPI_RegDef_t*)SPI3_BASE)


#define I2C1        ((I2C_RegDef_t*)I2C1_BASE)
#define I2C2        ((I2C_RegDef_t*)I2C2_BASE)
#define I2C3        ((I2C_RegDef_t*)I2C3_BASE)

#define USART1      ((USART_RegDef_t*)USART1_BASE)
#define USART2      ((USART_RegDef_t*)USART2_BASE)
#define USART3      ((USART_RegDef_t*)USART3_BASE)


#define DMA1         ((DMA_RegDef_t*)DMA1_BASEADDR)
#define DMA2         ((DMA_RegDef_t*)DMA2_BASEADDR)
#define DMA2D        ((DMA_RegDef_t*)DMA2D_BASEADDR)
/*
  Clock Enable/Disable Macros for Gpiox peripherals

*/

#define GPIOA_PCLK_EN()     	(RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN()     	(RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()     	(RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()     	(RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()     	(RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN()     	(RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN()     	(RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN()     	(RCC->AHB1ENR |= (1 << 7))
#define GPIOI_PCLK_EN()     	(RCC->AHB1ENR |= (1 << 8))



#define GPIOA_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 7))
#define GPIOI_PCLK_DI()         (RCC->AHB1ENR &= ~(1 << 8))


/*
  Clock Enable Macros for I2Cx peripherals
*/
#define I2C1_PCLK_EN()        (RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()        (RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()        (RCC->APB1ENR |= (1 << 23))

#define I2C1_PCLK_DI()        (RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()        (RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()        (RCC->APB1ENR &= ~(1 << 23))

/*
  Clock Enable Macros for SPIx peripherals

*/

#define SPI1_PCLK_EN()        (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()        (RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()        (RCC->APB1ENR |= (1 << 15))

#define SPI1_PCLK_DI()        (RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()        (RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()        (RCC->APB1ENR &= ~(1 << 15))


/*
  Clock Enable Macros for USARTx peripherals

*/

#define USART1_PCLK_EN()        (RCC->APB2ENR |= (1 << 4))
#define USART6_PCLK_EN()        (RCC->APB2ENR |= (1 << 5))
#define USART2_PCLK_EN()        (RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()        (RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN()         (RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()         (RCC->APB1ENR |= (1 << 20))
/*
 * Clock enable/Disable Macros for SYSCFG peripheral
 */
#define TIM2_PCLK_EN()   (RCC->APB1ENR |= (1 << 0))
#define TIM3_PCLK_EN()   (RCC->APB1ENR |= (1 << 1))
#define TIM4_PCLK_EN()   (RCC->APB1ENR |= (1 << 2))
#define TIM5_PCLK_EN()   (RCC->APB1ENR |= (1 << 3))
#define TIM6_PCLK_EN()   (RCC->APB1ENR |= (1 << 4))
#define TIM7_PCLK_EN()   (RCC->APB1ENR |= (1 << 5))
#define TIM12_PCLK_EN()  (RCC->APB1ENR |= (1 << 6))
#define TIM13_PCLK_EN()  (RCC->APB1ENR |= (1 << 7))
#define TIM14_PCLK_EN()  (RCC->APB1ENR |= (1 << 8))

#define TIM1_PCLK_EN()   (RCC->APB2ENR |= (1 << 0))
#define TIM8_PCLK_EN()   (RCC->APB2ENR |= (1 << 1))
#define TIM9_PCLK_EN()   (RCC->APB2ENR |= (1 << 16))
#define TIM10_PCLK_EN()  (RCC->APB2ENR |= (1 << 17))
#define TIM11_PCLK_EN()  (RCC->APB2ENR |= (1 << 18))


#define TIM2_PCLK_DI()   (RCC->APB1ENR &= ~(1 << 0))
#define TIM3_PCLK_DI()   (RCC->APB1ENR &= ~(1 << 1))
#define TIM4_PCLK_DI()   (RCC->APB1ENR &= ~(1 << 2))
#define TIM5_PCLK_DI()   (RCC->APB1ENR &= ~(1 << 3))
#define TIM6_PCLK_DI()   (RCC->APB1ENR &= ~(1 << 4))
#define TIM7_PCLK_DI()   (RCC->APB1ENR &= ~(1 << 5))
#define TIM12_PCLK_DI()  (RCC->APB1ENR &= ~(1 << 6))
#define TIM13_PCLK_DI()  (RCC->APB1ENR &= ~(1 << 7))
#define TIM14_PCLK_DI()  (RCC->APB1ENR &= ~(1 << 8))

#define TIM1_PCLK_DI()   (RCC->APB2ENR &= ~(1 << 0))
#define TIM8_PCLK_DI()   (RCC->APB2ENR &= ~(1 << 1))
#define TIM9_PCLK_DI()   (RCC->APB2ENR &= ~(1 << 16))
#define TIM10_PCLK_DI()  (RCC->APB2ENR &= ~(1 << 17))
#define TIM11_PCLK_DI()  (RCC->APB2ENR &= ~(1 << 18))
/*
 * Clock enable Macros for SYSCFG peripheral
 */
#define SYSCSFG_PCLK_EN()       (RCC->APB2ENR |= (1<<14))

/*
 * Clock enable/di Macros for DMA
 *
 */
#define DMA1_PCLK_EN()           (RCC->AHB1ENR |= (1<<21))
#define DMA2_PCLK_EN()           (RCC->AHB1ENR |= (1<<22))

#define DMA1_PCLK_DI()           (RCC->AHB1ENR &= ~(1<<21))
#define DMA2_PCLK_DI()           (RCC->AHB1ENR &= ~(1<<22))

//SPI peripheral reset macro

#define SPI1_REG_RESET()		   do {(RCC->APB2RSTR |=(1 <<  12)); (RCC->APB2RSTR &= ~(1 << 12));}while(0)
#define SPI2_REG_RESET()           do {(RCC->APB1RSTR |=(1 <<  14)); (RCC->APB1RSTR &= ~(1 << 14));}while(0)
#define SPI3_REG_RESET()           do {(RCC->APB1RSTR |=(1 <<  15)); (RCC->APB1RSTR &= ~(1 << 15));}while(0)



//SPI peripheral reset macro

#define I2C1_REG_RESET()		   do {(RCC->APB1RSTR |=(1 <<  21)); (RCC->APB2RSTR &= ~(1 << 21));}while(0)
#define I2C2_REG_RESET()           do {(RCC->APB1RSTR |=(1 <<  22)); (RCC->APB1RSTR &= ~(1 << 22));}while(0)
#define I2C3_REG_RESET()           do {(RCC->APB1RSTR |=(1 <<  23)); (RCC->APB1RSTR &= ~(1 << 23));}while(0)

/*
 * Macros to reset GPIOx peripherals
 */

#define GPIOA_REG_RESET()            do{ (RCC->AHB1RSTR|= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0));}while(0)
#define GPIOB_REG_RESET()            do{ (RCC->AHB1RSTR|= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1));}while(0)
#define GPIOC_REG_RESET()            do{ (RCC->AHB1RSTR|= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2));}while(0)
#define GPIOD_REG_RESET()            do{ (RCC->AHB1RSTR|= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3));}while(0)
#define GPIOE_REG_RESET()            do{ (RCC->AHB1RSTR|= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4));}while(0)
#define GPIOF_REG_RESET()            do{ (RCC->AHB1RSTR|= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5));}while(0)
#define GPIOG_REG_RESET()            do{ (RCC->AHB1RSTR|= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6));}while(0)
#define GPIOH_REG_RESET()            do{ (RCC->AHB1RSTR|= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7));}while(0)
#define GPIOI_REG_RESET()            do{ (RCC->AHB1RSTR|= (1 << 8)); (RCC->AHB1RSTR &= ~(1 << 8));}while(0)


#define GPIO_BASEADDR_TO_CODE(x)    ((x == GPIOA)?0:\
		                             (x == GPIOB)?1:\
		                             (x == GPIOC)?2:\
		                             (x == GPIOB)?3:\
		                             (x == GPIOA)?4:\
		                             (x == GPIOB)?5:\
		                             (x == GPIOA)?6:\
		                             (x == GPIOB)?7:0)


/*
 * IRQ(Interrupt Request) Numbers of STM32F407x\
 * note: update these macros with valid values according to your MCU
 * TODO: You may complete this list for other peripherals
 *
 */

#define IRG_NO_EXTI0        6
#define IRQ_NO_EXTI1        7
#define IRQ_NO_EXTI2        8
#define IRQ_NO_EXTI3        9
#define IRQ_NO_EXTI4        10
#define IRQ_NO_EXTI9_5        23
#define IRQ_NO_EXTI15_10      40
// Some generic marcos
#define ENABLE 1
#define DISABLE 0

#define SET   ENABLE
#define RESET DISABLE
#define FLAG_RESET  RESET
#define FLAG_SET    SET


/*
 * bit definition macros of i2c peripheral
 * */

		// CR1 bit position definition

#define I2C_CR1_PE			    0
#define I2C_CR1_SMBUS		    1
#define I2C_CR1_SMBTYPE		    3
#define I2C_CR1_ENARP		    4
#define I2C_CR1_ENPEC		    5
#define I2C_CR1_ENGC		    6
#define I2C_CR1_NOSTRETCH	    7
#define I2C_CR1_START		    8
#define I2C_CR1_STOP		    9
#define I2C_CR1_ACK		    	10
#define I2C_CR1_POS		    	11
#define I2C_CR1_PEC		    	12
#define I2C_CR1_ALERT		    13
#define I2C_CR1_SWRST		    15


		// CR2 bit position definition

#define I2C_CR2_FREQ			0
#define I2C_CR2_ITERREN		    8
#define I2C_CR2_ITEVTEN		    9
#define I2C_CR2_ITBUFEN		    10
#define I2C_CR2_DMAEN		    11
#define I2C_CR2_LAST		    12

		// OAR1 bit position definition

#define I2C_OAR1_ADD0			0
#define I2C_OAR1_ADD			1
#define I2C_OAR1_AD1			8
#define I2C_OAR1_ADDMODE		15

		// OAR2 bit position definition

#define I2C_OAR2_ENDUAL			0
#define I2C_OAR2_ADD2			1

		// DR bit position definition

#define I2C_DR_DR			0

		// SR1 bit position definition

#define I2C_SR1_SB			    0
#define I2C_SR1_ADDR		    1
#define I2C_SR1_BTF		    	2
#define I2C_SR1_ADD10		    3
#define I2C_SR1_STOPF		    4
#define I2C_SR1_RxNE		    6
#define I2C_SR1_TxE				7
#define I2C_SR1_BERR		    8
#define I2C_SR1_ARLO		    9
#define I2C_SR1_AF		    	10
#define I2C_SR1_OVR		    	11
#define I2C_SR1_PECERR		    12
#define I2C_SR1_TIMEOUT		    14
#define I2C_SR1_SMBALERT		15

		// SR2 bit position definition

#define I2C_SR2_MSL			    0
#define I2C_SR2_BUSY		    1
#define I2C_SR2_TRA				2
#define I2C_SR2_GENCALL			4
#define I2C_SR2_SMBDEFAULT		5
#define I2C_SR2_SMBHOST			6
#define I2C_SR2_DUALF			7
#define I2C_SR2_PEC				8

		// CCR bit position definition

#define I2C_CCR_CCR				0
#define I2C_CCR_DUTY			14
#define I2C_CCR_FS				15

		// TRISE bit position definition

#define I2C_TRISE_TRISE			0

		// FLTR bit position definition

#define I2C_FLTR_DNF			0
#define I2C_FLTR_ANOFF			4
#include "gpio.h"
#endif



