#ifndef CLI_H_
#define CLI_H_

#include "utils.h"
#include "uart.h"
#include "pwm.h"

void CLI_Init(); //imprime mensaje bienvenida

void CLI_cmd(char *); //procesar comando

#endif /* CLI_H_ */