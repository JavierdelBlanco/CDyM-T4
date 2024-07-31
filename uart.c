#include "uart.h"

#define USART_BAUDRATE 9600 // Velocidad de baudios deseada (velocidad de tansferencia)
#define BAUD_PRESCALER (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define ASYNCHRONOUS (0 << UMSEL00) // USART Mode Selection

#define DISABLED (0 << UPM00)
#define PARITY_MODE DISABLED // USART Parity Bit Selection

#define ONE_BIT (0 << USBS0)
#define STOP_BIT ONE_BIT // USART Stop Bit Selection

#define EIGHT_BIT (3 << UCSZ00)
#define DATA_BIT EIGHT_BIT // USART Data Bit Selection

static char buffer_tx[10]; // Buffer de transmisión
static char buffer_rx[10]; // Buffer de recepcion 
uint8_t cmd_flag = 0;

void UART_Init()
{
	// Establecer baud rate
	UBRR0L = BAUD_PRESCALER;  //103, pero el numero calculado no es un numero entera ya que posee decimales, por lo que habra un pequeño error y no sera exactamente 9600 bps

	// Establecer formato de trama
	UCSR0C = ASYNCHRONOUS | PARITY_MODE | STOP_BIT | DATA_BIT;

	/*Permitir que la USART sobreescriba la operacion normal en los pines TX y RX*/
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Activar interrupciones para la recepcion
	UART_Enable_Rx();
	UART_Enable_Tx();
}

/*
 * Control de interrupciones de transmision y recepcion
 */
void UART_Disable_Rx()
{
	UCSR0B &= ~(1 << RXCIE0);
}

void UART_Enable_Rx()
{
	UCSR0B |= (1 << RXCIE0);
}

void UART_Disable_Tx()
{
	UCSR0B &= ~(1 << UDRIE0);
}

void UART_Enable_Tx()
{
	UCSR0B |= (1 << UDRIE0);
}

void UART_SendMsg(char *msg)
{
	strcpy(buffer_tx, msg);
	UART_Enable_Tx();
}

char *UART_ReadBuffer()
{
	return buffer_rx;
}

uint8_t UART_GetCmdFlag()
{
	return cmd_flag;
}

void UART_ClearCmdFlag()
{
	cmd_flag = 0;
}

/*
 * Interrupcion de recepcion
 * Disparada por la UART cuando hay un nuevo dato en UDR0 listo para ser leido
 */
ISR(USART_RX_vect)
{
	static volatile uint8_t i = 0;
	unsigned char dato;

	dato = UDR0;

	/* Detectar fin de linea */
	if (dato == '\r')
	{
		buffer_rx[i] = '\0';
		i = 0;
		cmd_flag = 1;
		UART_Disable_Rx();
	}
	/* Detectar borrado de caracter */
	else
	{
		if (dato == '\b')
			i--;
		else
		{
			buffer_rx[i] = dato;
			i++;
		}
	}
}
/*
* Interrupcion de transimision

* EL flag de UDRE0 se activa cuando el UDR0 está listo para recibir nuevos datos.
*
* Este bit se establece cuando UDR0 está vacío, y se borra cuando el UDR0 contiene
* datos que aún no han sido movidos al registro de desplazamiento para ser transmitidos.

* Cargo el caracter de mensaje a enviar hasta alcanzar el fin del mensaje
*/
ISR(USART_UDRE_vect)
{
	cli();//Se desabilitan las interrupciones
	static volatile uint8_t i = 0;
	if (buffer_tx[i] != '\0')
	{
		UDR0 = buffer_tx[i]; //Guardo el caracter del buffer de transmision en el registro de datos
		i++;
	}
	else
	{
		i = 0;
		UART_Disable_Tx();//Es necesario desactivar las interrupciones para evitar que se produzca otra interrupcion cuando sea leido ya que no se vacio UDR0
	}
	sei();//Se habilitan las interrupciones
}