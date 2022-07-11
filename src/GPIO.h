#ifndef GPIO_H_
#define GPIO_H_

// Inkluderingsdirektiv: 
#include "definitions.h"

/******************************************************************************
* Strukten Led används för implementering av lysdioder, som kan placeras på
* någon av digitala PINs 0 - 13 på Arduino Uno. Varje lysdiod kan tändas, 
* släckas och togglas.
******************************************************************************/
struct Led 
{
	uint8_t PIN;		// Skapar en medlem av datatypen uint8_t som lagrar det önskat PIN-nummer som man vill ansluta sin LED till.
	bool enabled;		// Skapar en medlem av datatypen bool som döps till enabled, som indikerar om lysdioden är på eller inte.
	IO_port io_port;	// Skapar en medlem av datatypen/enumerationen IO_port som döps till io_port, för att lagra vilken IO-port som skall användas för led.
};

/******************************************************************************
* Strukten Button används för implementering av tryckknappar, som kan placeras 
* på någon av digitala PINs 0 - 13 på Arduino Uno. Det finns möjlighet att
* enkelt läsa av ifall tryckknappen är nedtryckt. Det finns också möjlighet 
* att aktivera samt inaktivera PCI-avbrott på tryckknappens PIN.
* 
* Avbrottsvektorer gällande PCI-avbrott för respektive I/O-port är följande:
*
* I/O-port B (PIN 8 - 13): PCINT0_vect
* I/O-port C (PIN A0 - A5): PCINT1_vect - används dock inte för tryckknappar.
* I/O-port D (PIN 0 - 7): PCINT2_vect
******************************************************************************/
struct Button 
{
	uint8_t PIN;			// Skapar en medlem av datatypen uint8_t som lagrar det önskat PIN-nummer som man vill ansluta sin knapp till. 
	IO_port io_port;		// Skapar en medlem av datatypen/enumerationen IO_port som döps till io_port, för att lagra vilken IO-port som skall användas för led. 
	bool interrupt_enabled; // Skapar en medlem av datatypen bool som döps till interrupt_enabled, som om indikerar PCI-avbrott är aktiverat. 
};

// Funktionsdeklarationer:
struct Led new_Led(const uint8_t PIN); 
void Led_on(struct Led* self);
void Led_off(struct Led* self);
void Led_toggle(struct Led* self);
void Led_blink(struct Led* self, const uint16_t delay_time); 

struct Button new_Button(const uint8_t PIN); 
bool Button_is_pressed(struct Button* self); 
void Button_enable_interrupt(struct Button* self); 
void Button_disable_interrupt(struct Button* self); 

#endif /* GPIO_H_ */
