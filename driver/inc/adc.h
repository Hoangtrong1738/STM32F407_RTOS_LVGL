#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "stm32f407xx.h"

void adc_init(void);
float adc_get_temp_ss(void);

#endif /* INC_ADC_H_ */
