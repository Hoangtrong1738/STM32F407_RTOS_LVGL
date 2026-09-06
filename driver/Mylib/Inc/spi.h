#ifndef SPI_H_
#define SPI_H_

#include "stm32f407xx.h"
/*
void spi_init();
char spi_read_data(char reg);
void spi_write_data(char reg, char data);
void spi_init(void);
*/
void spi_active_slave(void);
void spi_inactive_slave(void);
char spi_read_data(char reg);
void spi_write_data(char reg, char data);

void spi1_lcd_init(void);
void spi1_wait_idle(void);
void spi1_enable_tx_only(void);
void spi1_enable_full_duplex(void);
void spi1_write8(u8 data);
void spi1_write_buffer(const u8 *data, u32 len);
void spi1_write_dma_start(const u8 *data, u32 len);
void spi1_write_dma_wait(void);
void spi1_write_dma(const u8 *data, u32 len);
u8 spi1_dma_tx_is_busy(void);
u8 spi1_transfer_byte(u8 data);

#endif
