#ifndef SPI_H_
#define SPI_H_

#include "stm32f407xx.h"
#include "types.h"

void spi_init();
char spi_read_data(char reg);

#endif