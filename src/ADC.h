
#ifndef ADC_H_
#define ADC_H_

// Inkluderingsdirektiv: 
#include "definitions.h"
#include "Serial.h"

/******************************************************************************
* Formler f�r ber�kning av temperatur:
*
* Analog insp�nning Uin:
* Uin = Vcc * ADC_result / ADC_max,
* d�r ADC_result �r resultatet fr�n senaste AD-omvandling (0 - 1023).
*
* Temperatur temp i grader Celcius:
* temp = 100 * Uin - 50,
* d�r Uin �r den ber�knade analoga insp�nningen (0 - 5 V).
******************************************************************************/
#define VCC 5.0f // Matningssp�nning 5 V.
#define ADC_MAX 1023.0 // Maxv�rde vid AD-omvandling. �ndrade till 1023.0

/******************************************************************************
* F�r att v�lja intern matningssp�nning f�r AD-omvandlaren, s� ettst�lls biten
* REFS0 i register ADMUX (ADC Multiplexer Select Register). Som default l�ses
* analog PIN A0 av. F�r att starta AD-omvandlingen s� ettst�lls samtliga
* bitar i registret ADCSRA (ADC Control and Status Register A). F�r att 
* kontrollera s� att en given AD-omvandling �r slutf�rd innan resultatet 
* h�mtas, s� sker avl�sning av biten ADIF (ADC Interrupt Flag), s� blir 
* ettst�llt n�r AD-omvandlingen �r f�rdig. S� l�nge denna bit �r nollst�lld,
* s� v�ntar vi in att AD-omvandlingen �r f�rdig via en while-sats. Biten
* ADCSRA �terst�lls inf�r n�sta AD-omvandling genom att denna nollst�lls.
******************************************************************************/
#define SELECT_ANALOG_CHANNEL ADMUX = (1 << REFS0)
#define START_AD_CONVERSION ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2)
#define WAIT_FOR_AD_CONVERSION_COMPLETE while ((ADCSRA & (1 << ADIF)) == 0)
#define RESET_ADC_INTERRUPT_FLAG ADCSRA = (1 << ADIF)

/******************************************************************************
* Strukten TempSensor anv�nds f�r implementering av en temperatursensor
* ansluten till en given analog PIN A0 - A5.
******************************************************************************/
struct TempSensor 
{
	uint8_t PIN; // PIN-nummer f�r avl�sning.
};

// Funktionsdeklarationer:
struct TempSensor new_TempSensor(const uint8_t PIN);
void print_temperature(const struct TempSensor* self); 

#endif /* ADC_H_ */