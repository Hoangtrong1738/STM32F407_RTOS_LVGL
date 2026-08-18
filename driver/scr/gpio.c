#include "gpio.h"
#include "stm32f407xx.h"

/*
 * API is APPlication Programming Interface
 * Tập hợp các hàm / giao diện để mình sử dụng chức năng của một module hoặc thư viện mà không
 * cần biết bên trong nó làm gì
 * trong STM32 API = các ham driver giúp bạn điều khiển phần cứng
 */
void GPIO_PClkControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}else if(pGPIOx == GPIOI)
		{
			GPIOI_PCLK_EN();
		}

	}
	else
	{
		if(pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}else if(pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}else if(pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}else if(pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}else if(pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}else if(pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}else if(pGPIOx == GPIOG)
		{
			GPIOG_PCLK_DI();
		}else if(pGPIOx == GPIOH)
		{
			GPIOH_PCLK_DI();
		}else if(pGPIOx == GPIOI)
		{
			GPIOI_PCLK_DI();
		}

	}

}

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t Pin)
{
	uint8_t value;
	value = (uint8_t)((pGPIOx ->IDR >> Pin) & 0x00000001);
	return value;
}
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
	value = (uint16_t)pGPIOx -> IDR;
	return value;


}
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, u16 Pin, u8 Value)
{
	if(Value){
		//Write 1 to the output data register at the bit field corresponding to the pin number
		pGPIOx -> ODR |= (1 << Pin);
	}else{
		//write 0
		pGPIOx -> ODR &= ~(1 << Pin);
	}

}
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint8_t Value)
{
	pGPIOx->ODR = Value;

}
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t Pin)
{
	pGPIOx -> ODR ^= (1 << Pin);

}

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)//
{

	if(pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}else if(pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}else if(pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}else if(pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}else if(pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}else if(pGPIOx == GPIOF)
	{
		GPIOF_REG_RESET();
	}else if(pGPIOx == GPIOG)
	{
		GPIOG_REG_RESET();
	}else if(pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET();
	}else if(pGPIOx == GPIOI)
	{
		GPIOI_REG_RESET();
	}

}

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp;

	// enable the peripheral clock
	GPIO_PClkControl(pGPIOHandle->pGPIOx, ENABLE);
	//1. configure the mode of gpio pin
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)// chế độ bình thường
	{
		temp = (pGPIOHandle ->GPIO_PinConfig.GPIO_PinMode << (2 *pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle ->pGPIOx->MODER  &= ~(0x3 <<(2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));// clearing
		pGPIOHandle ->pGPIOx->MODER |= temp;

	}else//GPIO ->SYSCFG ->EXTI ->NVIC -> CPU
	{
		//this part will code later . (interrupt mode)
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			//1. configure the FTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the corresponding RTSR bit
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			//1. configure the RTSR
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
						//Clear the corresponding FTSR bit
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);



		}else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			//1. configure the FTSR and RTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}

		//2. configure the GPIO port selection in SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;

		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCSFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = portcode << (temp2 * 4);




		//3. enable the exti interrupt delivery using IMR
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		/*
		 * /4 → tìm thanh ghi
           %4 → tìm vị trí trong thanh ghi
           *4 → tìm vị trí bit
		 */
	}
	temp = 0;

	//2.configure the speed
	temp = (pGPIOHandle -> GPIO_PinConfig.GPIO_PinSpeed  <<  (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle ->pGPIOx->OSPEEDR  &= ~(0x3 <<(2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));// clearing
	pGPIOHandle->pGPIOx->OSPEEDR |= temp;
	temp = 0;

	//3,configure the pupd settings
	temp = (pGPIOHandle -> GPIO_PinConfig.GPIO_PinPuPdControl  << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle ->pGPIOx->PUPDR  &= ~(0x3 << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));// clearing
	pGPIOHandle->pGPIOx->PUPDR |= temp;
    temp = 0;
	//4. configure the optype
    temp = (pGPIOHandle -> GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    pGPIOHandle ->pGPIOx->OTYPER  &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);// clearing
    pGPIOHandle->pGPIOx->OTYPER |= temp;
    temp = 0;

	//5. configure the alt fuctionality
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFM)
    {
    	//configure the alt function registers
    	uint32_t temp1, temp2;
    	temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;
    	temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;
    	pGPIOHandle ->pGPIOx->AFR[temp1]  &= ~(0xF << (4 * temp2));// clearing
    	pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4 * temp2));
    }

}
/*
 * GPIO Pin Interrupt Configuration

👉 Cấu hình ngắt cho chân GPIO

Pin must be in input configuration
→ Chân phải được cấu hình ở chế độ đầu vào (Input).

Configure the edge trigger (RT, FT, RFT)
→ Cấu hình kiểu kích hoạt theo cạnh:

RT (Rising Trigger) – kích hoạt cạnh lên

FT (Falling Trigger) – kích hoạt cạnh xuống

RFT (Rising & Falling Trigger) – kích hoạt cả hai cạnh

Enable interrupt delivery from peripheral to the processor (on peripheral side)
→ Bật cho phép truyền tín hiệu ngắt từ ngoại vi đến bộ xử lý (cấu hình phía ngoại vi).

Identify the IRQ number on which the processor accepts the interrupt from that pin
→ Xác định số IRQ mà bộ xử lý sẽ dùng để nhận ngắt từ chân đó.

Configure the IRQ priority for the identified IRQ number (Processor side)
→ Cấu hình mức ưu tiên (priority) cho IRQ đã xác định (phía bộ xử lý).

Enable interrupt reception on that IRQ number (Processor side)
→ Bật cho phép nhận ngắt trên số IRQ đó (phía bộ xử lý).
 */
/*
 * NVIC = Nested Vectored Interrupt Controller

Là bộ điều khiển ngắt nằm bên trong CPU ARM Cortex-M4 của STM32F407.

Nó có nhiệm vụ:

👉 Nhận yêu cầu ngắt từ ngoại vi (EXTI, TIM, USART, CAN...)
👉 Quyết định ưu tiên
👉 Cho CPU nhảy vào đúng hàm ISR
👉 Quản lý ngắt lồng nhau (nested interrupt)

📌 Ví dụ thực tế

Bạn nhấn nút tại PA0:

1️⃣ GPIO nhận thay đổi mức điện áp
2️⃣ EXTI phát hiện cạnh
3️⃣ EXTI gửi yêu cầu lên NVIC
4️⃣ NVIC quyết định có cho CPU xử lý không
5️⃣ CPU nhảy vào:
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi) // Bật hoặc tắt Interrupt, Số interrupt
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

} // ví dụ : GPIO_IRQInterruuptConfig(6, ENABLE)  -- Bật interrupt số 6 trong NVIC

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
 // 1. First lets find out the ipr register

    uint8_t iprx = IRQNumber / 4;
    uint8_t iprx_section = IRQNumber % 4;

    uint8_t shift_amount =
        (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

    /*
     * Clear old priority first
     *
     * STM32F4 implements only upper 4 bits of each 8-bit priority field.
     */
    *(NVIC_PR_BASE_ADDR + iprx) &= ~(0xF << shift_amount);

    /*
     * Set new priority
     */
    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
	/*
	 * NVIC_PR_BASE_ADDR - base address của IPR
	 * iprx - chọn IPR vd 6/4 -> chọn IPR1
	 * iprx_section - ta phải biết interrupt nằm ở vị trí nào trong thanh ghi đó. VD: 6 % 4 nằm ở slot số 2
	 * iprx * 4 -- offset register
	 * IRQPriority -- GIÁ trị priority: sự ưu tiên
	 */
	/*
	 * Priority field thực tế la 8 bit
	 * trong NVIC mỗi interrupt có 8 bit priority
	 * Nhưng stm32 chỉ implement 1 phần.Trong stm32f4 Chỉ có  4 bit cao được dùng 4 bit thấp không implement:thực hiện
	 * |7|6|5|4|3|2|1|0|
	 * |P3|P2|P1|P0|x|x|x|x|
	 */

}

void GPIO_IRQHandling(uint8_t Pin)
{
// clear the exti PR register corresponding to the pin number
	if(EXTI->PR & (1<<Pin))
	{
		EXTI->PR |= (1<<Pin);
	}
}





