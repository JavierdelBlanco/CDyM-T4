#include "utils.h"
#include "pwm.h"
#include "adc.h"
#include "uart.h"
#include "cli.h"

#define ROJO PB5
#define VERDE PB2
#define AZUL PB1

int main(void)
{
	DDRB |= 1<<ROJO | 1<< VERDE | 1 << AZUL; //Establezco los pines del verde y el azul como salidas
	UART_Init(); //Inicializo el uart
	CLI_Init(); //Inicializo la terminal
	PWM_Init(); //Inicializo las señales PWM
	ADC_Init(); //Inicializa el ADC
	sei();

	while (1)
	{
		if (UART_GetCmdFlag()) //veifica si se envio el valor del color por la teminal
		{
			UART_ClearCmdFlag(); //limpio el flag de la uart
			CLI_cmd(UART_ReadBuffer()); //Se lee el dato del color enviado a traves del uart (letra del color o intensidad)
			UART_Enable_Rx(); //habilito a la uart para recibir infomacion
			
		}
		ADC_Read(); //Leer el valor del brillo
		if(ADC_GetFlag()){ //si ya se leyo el valor del brillo
			PWM_SetBrillo(ADC_GetBrillo()); //asigno el brillo a los colores
		}
	}
}
