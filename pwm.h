#ifndef PWM_H_
#define PWM_H_

#include "utils.h"
#include "uart.h"
#include "adc.h"
void PWM_Init();

void PWM_SetRed(uint8_t);
void PWM_SetGreen(uint8_t);
void PWM_SetBlue(uint8_t);
void PWM_SetBrillo(uint16_t b);

#endif /* PWM_H_ */