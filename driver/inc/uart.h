#include "stm32f407xx.h"
#define DMA2_ADDRESS 0x40026400


extern char recv_data[20];
extern int rx_index;
void UART1_init();
void UART1_Send_1byte(char data);
void UART1_Send_String(char* msg);
char UART1_Recv_1Byte();
void uart1_rx_handler();
void dma2_uart1rx_init();
void dma2_stream5_handler();
void USART1_IRQHandler(void);
void UART1_Interrupt_init();
void printlog(char *format, ...);