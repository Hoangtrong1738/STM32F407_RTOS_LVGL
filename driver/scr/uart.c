#include "string.h"
#include <stdarg.h>
#include "string.h"
#include "uart.h"
char recv_data[20];
int rx_index;


void UART1_init()
{
	GPIOB_PCLK_EN();
	u32 *MODE = (u32 *)(0x40020400);
	// Set PB6 as UART1_TX (AF07) and PB7 as UART1_RX (AF07)
	u32 *AFRL = (u32 *)(0x40020420);
	*MODE &= ~(0b1111 << 12);
	*MODE |= (0b10 << 12) | (0b10 << 14);//set alternate function mode for PB6 and PB7

	*AFRL &= ~(0xff << 24);
	*AFRL |= (7<<24) | (7<<28);// Set AF07 for PB6 and PB7
	USART1_PCLK_EN();

	u32 *BRR = (u32*)(0x40011008);
	u32 *CR1 = (u32*)(0x4001100c);
	*BRR = (546 << 4) | 14; // set baudrate in 9600
	*CR1 &= ~(1 << 10); // disable parity
	*CR1 &= ~(1<<12); // set data 8bits data

	*CR1 |= (1<<13)|(1<<2)|(1<<3);// enable trasmiter,reciever,uart
#if 1
	// enable RXNE interrupt -> when RXNE is set, UART1 generate interrupt event send to NVIC
	*CR1 |= (1<<5);

	// NVIC accept interrupt event, which is send from UART1
	u32 *ISER1 = (u32*)(0xE000E104);
	*ISER1 |= 1<<(37-32);
#else
	// when RXNE is set, send signal to DMA2, DMA2 move data to RAM
	u32 *CR3 = (u32*)(0x40011014);
	*CR3 |= (1<<6);

#endif


}
void UART1_Interrupt_init()
{
    u32 *function_address = (u32*)0x200000D4;
	*function_address = (u32)(uart1_rx_handler) | 1;

	function_address = (u32*)0x20000150;
	*function_address = (u32)(dma2_stream5_handler) | 1;
}
void UART1_Send_1byte(char data)
{
	u32 *SR =(u32*)(0x40011000);
	u32 *DR = (u32*)(0x40011004);
	while(((*SR >> 7) & 1) == 0); // Kiem tra thanh ghi co dang trong khong
	*DR = data;
	while(((*SR >> 6) & 1) == 0); // wait transmitter of UART1 complete transfer
	*SR &= ~(1<<6);  // clear TC flag

}

void UART1_Send_String(char* msg)
{
	int msg_len = strlen(msg);
	for(int i = 0; i< msg_len; i++)
	{
		UART1_Send_1byte(msg[i]);

	}
}


char UART1_Recv_1Byte()
{
	u32 *SR =(u32*)(0x40011000);
	u32 *DR = (u32*)(0x40011004);
	while(((*SR >> 5) & 1) == 0); // wait RXNE to read recv data
	char recv_data = *DR;
	return recv_data;
}
/*
void USART1_IRQHandler()
{
	u32 *DR= (u32*)(0x40011004);
	recv_data[rx_index++] = *DR;
}
*/

void uart1_rx_handler()
{
	u32 *DR= (u32*)(0x40011004);
	recv_data[rx_index++] = *DR;
	if(strstr(recv_data, "LED ON") != NULL)
	{
		//GPIO_WriteToOutputPin(GPIOA, PIN_6, RESET);
		rx_index = 0;
		memset(recv_data,0,sizeof(recv_data));

	}
	else if(strstr(recv_data, "LED OFF") != NULL)
	{
		//GPIO_WriteToOutputPin(GPIOA, PIN_6, SET);
		rx_index = 0;
		memset(recv_data,0,sizeof(recv_data));

	}
}


void dma2_uart1rx_init()
{
	DMA2_PCLK_EN();
	//use DMA2 stream 5 channel 4 --> UART1_RX
	u32 *DMA_S5CR = (u32*)(DMA2_ADDRESS + 0x10 + 0x18 * 5);
	u32 *DMA_S5NDTR = (u32*)(DMA2_ADDRESS + 0x14 + 0x18 * 5);
	u32 *DMA_S5PAR = (u32*)(DMA2_ADDRESS + 0x18 + 0x18 * 5);
	u32 *DMA_S5M0AR= (u32*)(DMA2_ADDRESS + 0x1C + 0x18 * 5);
	/*
	 * recv 7 bytes
	 * from: UART DR (0x40011004)
	 * to : recv_data (0x20000428)
 v * to : recv_data
	 */
	*DMA_S5NDTR = 7;
	*DMA_S5PAR = 0x40011004;
	*DMA_S5M0AR = (u32) recv_data;

	*DMA_S5CR |= (0b100 << 25);// select channel 4 for stream 5
	*DMA_S5CR |= (0b1 << 25); // enable memory increment mode

	*DMA_S5CR |= (0b1 << 4); // enable transfer complete interrupt
	*DMA_S5CR |= (0b1 << 8); // enable Circular mode : khi ma no nhan du 7 byte roi va no nhan 7 byte tiep theo

	//*DMA_S5CR |= (1 << 4);   // enable transfer complete interrupt
	*DMA_S5CR |= (0b1 << 0); // enable DMA2 stream 5

	u32 *ISER2 = (u32*)(0xE000E108);
	*ISER2 |= 1 << (68-64);

}

void dma2_stream5_handler()
{
	//_asm("NOP");

	//clear interrupt flag -> transfer complete interrupt

	u32 *HIFCR = (u32*)(DMA2_ADDRESS + 0X0C);
	*HIFCR |= 1 << 11;
	if(strstr(recv_data, "LED ON ") != NULL)
	{

	}else if(strstr(recv_data, "LED OFF") != NULL)
	{

	}
	memset(recv_data,0,7);
}
void USART1_IRQHandler(void)
{
	uart1_rx_handler();
}

void printlog(char *format, ...)
{
	char buf[256] = {0};   // giảm size nếu chạy trong task FreeRTOS
	int buf_len = 0;
	va_list ap;

	va_start(ap, format);
	vsnprintf(buf, sizeof(buf), format, ap);  // dùng vsnprintf để tránh tràn buf
	va_end(ap);

	buf_len = strlen(buf);
	for(int i = 0; i < buf_len; i++)
	{
		UART1_Send_1byte(buf[i]);
	}
}