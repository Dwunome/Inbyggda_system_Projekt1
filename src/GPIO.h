#ifndef GPIO_H_
#define GPIO_H_

// Inkluderingsdirektiv: 
#include "definitions.h"

// static void delay(const uint16_t delay_time);

/******************************************************************************
* Strukten Led anv�nds f�r implementering av lysdioder, som kan placeras p�
* n�gon av digitala PINs 0 - 13 p� Arduino Uno. Varje lysdiod kan t�ndas, 
* sl�ckas och togglas.
******************************************************************************/
struct Led 
{
	uint8_t PIN;		// Skapar en medlem av datatypen uint8_t som lagrar det �nskat PIN-nummer som man vill ansluta sin LED till.
	bool enabled;		// Skapar en medlem av datatypen bool som d�ps till enabled, som indikerar om lysdioden �r p� eller inte.
	IO_port io_port;	// Skapar en medlem av datatypen/enumerationen IO_port som d�ps till io_port, f�r att lagra vilken IO-port som skall anv�ndas f�r led.
};

/******************************************************************************
* Strukten Button anv�nds f�r implementering av tryckknappar, som kan placeras 
* p� n�gon av digitala PINs 0 - 13 p� Arduino Uno. Det finns m�jlighet att
* enkelt l�sa av ifall tryckknappen �r nedtryckt. Det finns ocks� m�jlighet 
* att aktivera samt inaktivera PCI-avbrott p� tryckknappens PIN.
* 
* Avbrottsvektorer g�llande PCI-avbrott f�r respektive I/O-port �r f�ljande:
*
* I/O-port B (PIN 8 - 13): PCINT0_vect
* I/O-port C (PIN A0 - A5): PCINT1_vect - anv�nds dock inte f�r tryckknappar.
* I/O-port D (PIN 0 - 7): PCINT2_vect
******************************************************************************/
struct Button 
{
	uint8_t PIN;			// Skapar en medlem av datatypen uint8_t som lagrar det �nskat PIN-nummer som man vill ansluta sin knapp till. 
	IO_port io_port;		// Skapar en medlem av datatypen/enumerationen IO_port som d�ps till io_port, f�r att lagra vilken IO-port som skall anv�ndas f�r led. 
	bool interrupt_enabled; // Skapar en medlem av datatypen bool som d�ps till interrupt_enabled, som om indikerar PCI-avbrott �r aktiverat. 
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