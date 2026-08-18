
 * spi.h
 *
 *  Created on: Mar 24, 2026
 *      Author: GB Center
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_
#include <stm32f407xx.h>

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);
void SPI_PClkControl(SPI_RegDef_t *pSPIx,u8 EnorDi);

/*
 * Data Send and Receive
 */
void SPI_SendData(SPI_RegDef_t *pSPIx,u8 *pTxBuffer,u32 Len);

void SPI_ReceiveData(SPI_RegDef_t *pSPIx,u8 *pRxBuffer,u32 Len);

u8 SPI_SenDataIT(SPI_Handle_t *spi, u8 *pTxBuffer, u32 Len);
u8 SPI_ReceiveDataIT(SPI_Handle_t *spi,u8 *pRxBuffer,u32 Len);

/*
 *
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, u32 IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx,u8 EnDrDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, u8 EnOrDi);
void SPI_SSDEConfig(SPI_RegDef_t *pSPIx, u8 EnOrDi);

u8 SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, u32 FlagName);
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx);
void SPI_CloseTransmisson(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);

/*
 * Application callback
 */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,u8 AppEv);

#endif /* INC_SPI_H_ */
















