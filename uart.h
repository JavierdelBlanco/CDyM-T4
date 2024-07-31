#ifndef UART_H
#define UART_H

#include "utils.h"

void UART_Init();
void UART_SendMsg(char *msg);
char *UART_ReadBuffer();
uint8_t UART_GetCmdFlag();
void UART_ClearCmdFlag();
void UART_Disable_Rx();
void UART_Enable_Rx();
void UART_Disable_Tx();
void UART_Enable_Tx();

#endif