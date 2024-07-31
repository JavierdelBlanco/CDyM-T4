#include "cli.h"

static volatile uint8_t colorFlag = 0; // flag para indicar si se ha seleccionado un color
static volatile char color; 
static char inicio[] = "Bienvenido\rSeleccione el led: R - G - B\r"; // Mensaje de bienvenida
static char valor[] = "\rA continuacion seleccione un valor para ajustar la intensidad (0..255)\r"; // Mensaje para solicitar el valor de intensidad
static char color_invalido[] = "\rEl color ingresado es invalido\r"; // Mensaje para indicar color inv�lido
static char valor_invalido[] = "\rEl valor ingresado es invalido\r"; // Mensaje para indicar valor inv�lido

// Funci�n de inicializaci�n del CLI (Interfaz de L�nea de Comandos)
void CLI_Init()
{
	UART_SendMsg(inicio); // Enviar mensaje de bienvenida a trav�s de UART
}

// Funci�n para validar el color ingresado
static char Validar_Color(char command[])
{
	// Verifica que el comando sea 'R', 'G' o 'B'
	if (!strcmp(command, "R") | !strcmp(command, "G") | !strcmp(command, "B"))
	{
		return command[0]; // Si es v�lido, devuelve el car�cter del comando
	}
	else
	{
		return 0; // Si no es v�lido, devuelve 0
	}
}

// Funci�n para validar el valor ingresado
static int Validar_Valor(char command[])
{
	int digitos = strlen(command); // Obtiene la longitud del comando
	int valor = 0; // Inicializa el valor a 0

	// Valida si el valor tiene m�s de 3 d�gitos o no tiene ninguno
	if ((digitos > 3) | (digitos == 0)){
		return -1; // Si es as�, devuelve -1
	}

	// Recorre cada d�gito del comando
	for (int i = 0; i < digitos; i++)
	{
		// Verifica si cada car�cter es un d�gito v�lido
		if ((command[i] > '9') | (command[i] < '0'))
		{
			return -1; // Si no es un d�gito v�lido, devuelve -1
		}
		valor = valor * 10; // Desplaza el valor a la izquierda
		valor += command[i] - '0'; // Agrega el d�gito actual al valor
	}
	// Verifica si el valor es mayor de 255
	if (valor > 255)
	{
		return -1; // Si es mayor de 255, devuelve -1
	}

	return valor; // Devuelve el valor v�lido
}

// Funci�n para manejar comandos del CLI
void CLI_cmd(char command[])
{
	// Si ya se ha seleccionado un color, asigna el valor al mismo
	if (colorFlag)
	{
		int valor = Validar_Valor(command); // Valida el valor ingresado
		if (valor != -1)
		{
			colorFlag = 0; // Resetea la flag de color
			// Asigna el valor al color correspondiente
			switch (color)
			{
			case 'R':
				PWM_SetRed(valor);
				break;
			case 'G':
				PWM_SetGreen(valor);
				break;
			case 'B':
				PWM_SetBlue(valor);
				break;
			}
		}
		else
		{
			UART_SendMsg(valor_invalido); // Envia mensaje de valor inv�lido
		}
	}
	// Si no se ha seleccionado un color, lo valida
	else
	{
		char c = Validar_Color(command); // Valida el color ingresado
		if (c){
			color = c; // Asigna el color v�lido
			colorFlag = 1; // Activa el flag de color
			UART_SendMsg(valor); // Solicita el valor de intensidad
		}
		else
		{
			UART_SendMsg(color_invalido); // Envia mensaje de color inv�lido
		}
	}
}
