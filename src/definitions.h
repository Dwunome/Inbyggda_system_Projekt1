#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#define F_CPU 16000000UL // S�tter klockfrekvensen till 16 MHz. 

// Inkluderingsdirektiv: 
#include <avr/io.h> // Bibliotek f�r implementering av I/O g�llande AVR-enheter. 
#include <avr/interrupt.h> // Bibliotek f�r avbrott. 
#include <stdio.h> // Bibliotek f�r implementering av I/O i C. 
#include <stdlib.h> // C:s standardbibliotek. 
#include "Byte.h" // Headerfil f�r inkludering av union f�r enkel bithantering. 
#include <util/delay.h> // Bibliotek generering av f�rdr�jning.

/******************************************************************************
* F�r att aktivera avbrott globalt s� ettst�lls I-flaggan (Interrupt Flag)
* i statusregistret SREG, vilket �stadkommes via assemblerinstruktionen SEI
* (Set Interrupt Flag). F�r att vid behov inaktivera avbrott globalt, vilket
* exempelvis �r n�dv�ndigt vid skrivning till EEPROM-minnet, s� nollst�lls
* I-biten via assemblerinstruktionen CLI (Clear Interrupt Flag).
******************************************************************************/
#define ENABLE_INTERRUPTS asm("SEI") // Aktiverar avbrott globalt. 
#define DISABLE_INTERRUPTS asm("CLI") // Inaktiverar avbrott globalt. 

/* Typdefinitioner: */
typedef enum bool { false, true } bool; // Realiserar datatypen bool.
typedef enum IO_port { IO_PORTB, IO_PORTC, IO_PORTD } IO_port; // Enumeration f�r I/O-portar. La till typedef. 
typedef enum TimerSelection { TIMER0, TIMER1, TIMER2 } TimerSelection; // Enumeration f�r timerkretsar. 
typedef enum DataType { INT, DOUBLE, SIZE_T, CHAR } DataType; // Val av datatyp f�r vektor.

#endif /* DEFINITIONS_H_ */