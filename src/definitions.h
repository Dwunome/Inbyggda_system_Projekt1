#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define F_CPU 16000000UL // Sätter klockfrekvensen till 16 MHz. 

// Inkluderingsdirektiv: 
#include <avr/io.h> // Bibliotek för implementering av I/O gällande AVR-enheter. 
#include <avr/interrupt.h> // Bibliotek för avbrott. 
#include <stdio.h> // Bibliotek för implementering av I/O i C. 
#include <stdlib.h> // C:s standardbibliotek. 
#include "Byte.h" // Headerfil för inkludering av union för enkel bithantering. 
#include <util/delay.h> // Bibliotek generering av fördröjning.

/******************************************************************************
* För att aktivera avbrott globalt så ettställs I-flaggan (Interrupt Flag)
* i statusregistret SREG, vilket åstadkommes via assemblerinstruktionen SEI
* (Set Interrupt Flag). För att vid behov inaktivera avbrott globalt, vilket
* exempelvis är nödvändigt vid skrivning till EEPROM-minnet, så nollställs
* I-biten via assemblerinstruktionen CLI (Clear Interrupt Flag).
******************************************************************************/
#define ENABLE_INTERRUPTS asm("SEI") // Aktiverar avbrott globalt. 
#define DISABLE_INTERRUPTS asm("CLI") // Inaktiverar avbrott globalt. 

/* Typdefinitioner: */
typedef enum bool { false, true } bool; // Realiserar datatypen bool.
typedef enum IO_port { IO_PORTB, IO_PORTC, IO_PORTD } IO_port; // Enumeration för I/O-portar. La till typedef. 
typedef enum TimerSelection { TIMER0, TIMER1, TIMER2 } TimerSelection; // Enumeration för timerkretsar. 
typedef enum DataType { INT, DOUBLE, SIZE_T, CHAR } DataType; // Val av datatyp för vektor.

#endif /* DEFINITIONS_H_ */