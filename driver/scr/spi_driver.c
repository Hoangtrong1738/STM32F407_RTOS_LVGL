/*
 * spi.c
 *
 *  Created on: Mar 24, 2026
 *      Author: GB Center
 */
#include "spi.h"

void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	SPI_PClkControl(pSPIHandle->pSPIx, ENABLE);
	// first lets configure the SPI_CR1 register
	u32 tempreg = 0;

	// 1. configure the device mode
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << 2;

	//2. configure the bus
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD)
	{
		//BIDIMODE should be cleared
		tempreg &= ~(1<<15);
	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD)
	{
		//BIDIMODE should be SET
		tempreg |= (1<<15);

	}else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY)
	{
		// BIDIMODE should be cleared
		tempreg &= ~(1<<15);
		// RXONLY bit must be set
		tempreg |=(1<<10);
	}

	// 3.Configure the SPI serial clock speed (baud rate)
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << 3;

	//4. Configure the DFF
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << 11;
	// 5.Configure the CPOL
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << 1;

	//6. Configure the CPHA
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << 0;
	pSPIHandle->pSPIx->CR1 = tempreg;



}
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{

}
u8 SPI_GetFlagStatus(SPI_RegDef_t *pSPIx,u32 FlagName)
{
	if(pSPIx->SR & FlagName)
	{
		return FLAG_SET;
	}

	return FLAG_RESET;
}
void SPI_PClkControl(SPI_RegDef_t *pSPIx,u8 EnorDi)
{
	if(EnorDi == ENABLE)
		{
			if(pSPIx == SPI1)
			{
				SPI1_PCLK_EN();
			}else if(pSPIx == SPI2)
			{
				SPI2_PCLK_EN();
			}else if(pSPIx == SPI3)
			{
				SPI3_PCLK_EN();
			}

		}
		else
		{
			/*
			if(pSPIx == SPI1)
			{
				SPI1_PCLK_DI();
			}else if(pSPIx == SPI2)
			{
				SPI2_PCLK_DI();
			}else if(pSPIx == SPI3)

				SPI3_PCLK_DI();
		    }
		    */

		}
}

/*
 * Data Send and Receive
 */
void SPI_SendData(SPI_RegDef_t *pSPIx,u8 *pTxBuffer,u32 Len)
{
	while(Len > 0)
	{
		//1.wait until TXE is set

		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		//2. check the DFF bit in CR1
		if(pSPIx->CR1 & ( 1 << SPI_CR1_DFF))
		{
			//16 bit DFF

			pSPIx->DR = *((u16 *)pTxBuffer);
			Len--;
			Len--;
			(u16*)pTxBuffer++;// trỏ tiếp đến dữ liệu để gửi byte tiếp theo không muốn gửi lại byte cũ
		}else
		{
			//8 bit DFF
			pSPIx->DR = *pTxBuffer;

			Len--;
			pTxBuffer++;
		}

	}
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx,u8 *pRxBuffer,u32 Len)
{
	while(Len > 0)
		{
			//1.wait until TXE is set

			while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

			//2. check the DFF bit in CR1
			if(pSPIx->CR1 & ( 1 << SPI_CR1_DFF))
			{
				//16 bit DFF
				//1. load the data form DR to Rxbuffer address

				*((u16 *)pRxBuffer) = pSPIx->DR;
				Len--;
				Len--;
				/*
				 * khí=i DFF= 1 -> 16 bit mode
				 * Nghĩa là ghi vào thanh ghi DR gửi 2 byte(16 bit)
				 * Len = số byte
				 * vậy lên nếu mode gửi 16 bit nên là 2 byte
				 */
				(u16*)pRxBuffer++;// trỏ tiếp đến dữ liệu để gửi byte tiếp theo không muốn gửi lại byte cữ
			}else
			{
				//8 bit DFF
				*(pRxBuffer) = pSPIx->DR;
				Len--;
				pRxBuffer++;
			}

		}

}

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx,u8 EnOrDi) //  bật tăt SPI
{
	if(EnOrDi == ENABLE)
	{
		pSPIx ->CR1 |= (1 << SPI_CR1_SPE);
	}else
	{
		pSPIx ->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}
void SPI_SSIConfig(SPI_RegDef_t *pSPIx, u8 EnOrDi) // Set bit SSI (Software NSS)
{
	if(EnOrDi == ENABLE)
	{
		pSPIx ->CR1 |= (1 << SPI_CR1_SSI);
	}else
	{
		pSPIx ->CR1 &= ~(1 << SPI_CR1_SSI);
	}
}
void SPI_SSDEConfig(SPI_RegDef_t *pSPIx, u8 EnOrDi) // Thực ra là SSM Config
{
	// SSM = 1 -> bỏ qua chân NSS thật
	//SSM = 0 -> phải dùng chân NSS
	if(EnOrDi == ENABLE)
	{
		pSPIx ->CR1 |= (1 << SPI_CR1_SSM);
	}else
	{
		pSPIx ->CR1 &= ~(1 << SPI_CR1_SSM);
	}
}
u8 SPI_SenDataIT(SPI_Handle_t *spi, u8 *pTxBuffer, u32 Len)
{
	u8 state = spi -> TxState;
	if(state != SPI_BUSY_IN_RX)
	{
		//1. Save the TX buffer address and len information in some global variables: lưu bộ đệm Tx và thông tin độ dài
		spi->pTxBuffer = pTxBuffer;
		spi->TxLen = Len;
		//2. Mark the SPI state as busy transmission so that no other code can take over same SPI peripheral until transmission is over
		spi->TxState = SPI_BUSY_IN_TX;
		// 3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		spi->pSPIx->CR2 = (1 << SPI_CR2_TXEIE);

		//4. Data transmission will be handled by the ISR code (will implement later)
	}

	return state;
}
u8 SPI_ReceiveDataIT(SPI_Handle_t *spi,u8 *pRxBuffer,u32 Len)
{
	u8 state = spi->RxState;
	if(state != SPI_BUSY_IN_RX)
	{
		//1. Save the RX buffer address and len information in some global variables: lưu bộ đệm Tx và thông tin độ dài
		spi->pRxBuffer = pRxBuffer;
		spi->RxLen = Len;
		//2. Mark the SPI state as busy transmission so that no other code can take over same SPI peripheral until transmission is over
		spi->RxState = SPI_BUSY_IN_RX;
		// 3. Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		spi->pSPIx->CR2 = (1 << SPI_CR2_RXEIE);

		//4. Data transmission will be handled by the ISR code (will implement later)
	}


	return state;

}
/*
 *
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
		{
			if(IRQNumber <= 31)
			{
				// program ISER0 register
				*NVIC_ISER0 |=(1<<IRQNumber);

			}else if(IRQNumber > 31 && IRQNumber < 64) //32 to 63
			{
				// program ISER1 register
				*NVIC_ISER1 |=(1<<IRQNumber % 32);

			}else if(IRQNumber >= 64 && IRQNumber <96)
			{
				// program ISER2 register
				*NVIC_ISER2 |=(1<<IRQNumber % 64);
			}
		}else
		{
			if(IRQNumber <= 31)
			{
				// program ICER0 register
				*NVIC_ICER0 |= (1<<IRQNumber);
			}else if(IRQNumber > 31 && IRQNumber < 64)
			{
				// program ICER1 register
				*NVIC_ICER1 |= (1<<IRQNumber % 32);

			}else if(IRQNumber >= 6 && IRQNumber <96)
			{
				//program ICER2 register
				*NVIC_ICER2 |= (1<<IRQNumber % 64);
			}

		}
}
void SPI_IRQPriorityConfig(uint8_t IRQNumber, u32 IRQPriority)
{
	uint8_t iprx =IRQNumber / 4;

	uint8_t iprx_section = IRQNumber % 4;

	uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);
	*( NVIC_PR_BASE_ADDR + iprx ) |= (IRQPriority << shift_amount);

}


// static đi chỉ ra đây là hàm trợ giúp riêng tư
static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);

static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);

static void spi_ovr_error_interrupt_handle(SPI_Handle_t *pSPIHandle);



void SPI_IRQHandling(SPI_Handle_t *pHandle)
{
	 u8 temp1, temp2;
	 // first lets check for TXE

	 temp1 = pHandle ->pSPIx->SR & (1<< SPI_SR_TXE);
	 temp2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE); //nếu cả hai biến này đều đúng thì ngắt nayflaf do cài đặt cờ TXE
	 if(temp1 && temp2 )
	 {
		 // handle TXE
		 spi_txe_interrupt_handle(pHandle);
	 }

	 // check for RXNE
	 temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	 temp2 = pHandle->pSPIx->CR2 & (1<< SPI_CR2_RXEIE);
	 if(temp1 && temp2)
	 {
		 // handle RXNE
		 spi_rxne_interrupt_handle(pHandle);
	 }

	 //check for ovr flag
	 temp1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	 temp2 = pHandle->pSPIx->CR2 & (1<< SPI_CR2_ERRIE);
	 if(temp1 && temp2)
	 {
		 // handle OVR error
		 spi_ovr_error_interrupt_handle(pHandle);
	 }

}

//some helper function implementations

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	if(pSPIHandle->pSPIx->CR1 & ( 1 << SPI_CR1_DFF))
	{
		//16 bit DFF

		pSPIHandle->pSPIx->DR = *((u16 *)pSPIHandle->pTxBuffer);
		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;
		(u16*)pSPIHandle->pTxBuffer++;// trỏ tiếp đến dữ liệu để gửi byte tiếp theo không muốn gửi lại byte cũ
	}else
	{
		//8 bit DFF
		pSPIHandle->pSPIx->DR = *(pSPIHandle->pTxBuffer);

		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}

	if(!pSPIHandle->TxLen)
	{
		// TxLen is zero, so close the spi transmission and inform the application
		//TX is over
		// this prevents interrupts from setting up of TXE flag
		/*
		pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
		pSPIHandle->pTxBuffer = NULL;
		pSPIHandle->TxLen = 0;
		pSPIHandle->TxState = SPI_READY;
		*/
		SPI_CloseTransmisson(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_TX_CMPLT);
	}
}
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	if(pSPIHandle->pSPIx->CR1 & ( 1 << SPI_CR1_DFF))
	{
		//16 bit DFF
		//1. load the data form DR to Rxbuffer address

		*((u16 *)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->RxLen--;
		(u16*)pSPIHandle->pRxBuffer++;// trỏ tiếp đến dữ liệu để gửi byte tiếp theo không muốn gửi lại byte cữ
	}else
	{
		//8 bit DFF
		*(pSPIHandle->pRxBuffer) = (u8)pSPIHandle->pSPIx->DR;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer++;
	}
	if(!pSPIHandle->RxLen)
	{
/*
		pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXEIE);
		pSPIHandle->pRxBuffer = NULL;
		pSPIHandle->RxLen = 0;
		pSPIHandle->RxState = SPI_READY;
		*/
		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_RX_CMPLT);
	}

}
static void spi_ovr_error_interrupt_handle(SPI_Handle_t *pSPIHandle)
{
	u8 temp;

	//1. clear the over flag
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
	    /*temp = pSPIHandle->pSPIx->DR;
	    temp = pSPIHandle->pSPIx->SR;
	    */
		SPI_ClearOVRFlag(pSPIHandle->pSPIx);
	}
	(void)temp;
	//2. inform the application
	SPI_ApplicationEventCallback(pSPIHandle,SPI_EVENT_OVR_ERR);
}

void SPI_CloseTransmisson(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
}


void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~(1 << SPI_CR2_RXEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	u8 temp;

    temp = pSPIx->DR;
    temp = pSPIx->SR;

    (void)temp;

}


__weak void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle,u8 AppEv) // về cơ bản đây là một triển khoai yếu và ứng dụng dụng chức năng này
{
	// This is a week implementation the application may override this function

}








