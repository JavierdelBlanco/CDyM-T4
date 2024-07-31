#include "pwm.h"

#define ROJO PB5

static volatile uint8_t r=255;
static volatile uint8_t g=255;
static volatile uint8_t b=255;

static uint16_t brillo=1024;

static void Timer0_Init();
static void Timer1_Init();

static void Timer1_Init()
{
	// Configura los pines OC1A y OC1B para modo comparador inverso (PWM)
	TCCR1A |= (1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0);
	
	// Modo de generación de onda: Fast PWM, 8 bits
	TCCR1A |= (1 << WGM10);
	TCCR1B |= (1<<WGM12);
	
	// Prescaler de 1024 para una frecuencia de 62Hz
	TCCR1B |= (1 << CS12) | (1 << CS10);
	
}

static void Timer0_Init()
{	
	//Interrupciones de compare match y overflow
	TIMSK0 |= (1<<OCIE0A) | (1<<TOIE0);
	// Prescaler de 1024 para una frecuencia de 62Hz
	TCCR0B |= (1 << CS00) | (1 << CS02);
}

void PWM_Init()
{
	OCR0A = r;  // Inicializa el valor de comparación de Timer0 para el rojo
	OCR1B = g;  // Inicializa el valor de comparación de Timer1 para el verde
	OCR1A = b;  // Inicializa el valor de comparación de Timer1 para el azul
	
	Timer1_Init();
	Timer0_Init();
}

// Función para ajustar el valor de PWM del LED verde basado en el brillo
static void PWM_Green()
{
	float aux = g * (brillo/1024.0);
	OCR1B = (uint8_t) aux;
}

// Función para ajustar el valor de PWM del LED azul basado en el brillo
static void PWM_Blue()
{
	float aux = b * (brillo/1024.0);
	OCR1A = (uint8_t) aux;
}

// Función para ajustar el brillo general de los LEDs
void PWM_SetBrillo(uint16_t b){
	brillo = b;	
	
	PWM_Green();
	PWM_Blue();
}

// Función para ajustar el color del LED azul
void PWM_SetGreen(uint8_t color){
	g=color;
	if (g>0){
		TCCR1A |= (1 << COM1B1) | (1 << COM1B0); // Habilita PWM en OC1B
		PWM_Green();
		}
	else{
		TCCR1A &= ~(1 << COM1B1);
		TCCR1A &= ~(1 << COM1B0);  // Deshabilita PWM en OC1B
		PORTB |= 1 << PORTB2;      // Apaga el LED verde
	}
	
}

// Función para ajustar el color del LED azul
void PWM_SetBlue(uint8_t color){
	b=color;
	if (b>0){
		TCCR1A |= (1 << COM1A1) | (1 << COM1A0);  // Habilita PWM en OC1A
		PWM_Blue();
	} else {
		TCCR1A &= ~(1 << COM1A1);
		TCCR1A &= ~(1 << COM1A0);  // Deshabilita PWM en OC1A
		PORTB |= 1 << PORTB1;      // Apaga el LED azul
	}
}

// Función para ajustar el color del LED rojo
void PWM_SetRed(uint8_t color){
	r=color;
	if (r>0){
		Timer0_Init();  // Inicializa Timer0 si el color es mayor que 0;
	}else{
		TCCR0B = 0;     // Detiene Timer0
		PORTB |= 1 << PORTB5;  // Apaga el LED rojo
	}
	
}

// ISR para manejar el overflow del Timer0
ISR(TIMER0_OVF_vect){
	float aux = r * (brillo/1024.0);
	OCR0A = (uint8_t) aux;  // Ajusta el valor de OCR0A basado en el brillo
	PORTB &= ~(1 << ROJO);  // Apaga el LED rojo
}

// ISR para manejar la coincidencia  de comparación del Timer0
ISR(TIMER0_COMPA_vect)
{
	PORTB |= (1<<ROJO); // Enciende el LED rojo
}
