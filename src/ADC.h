
#ifndef ADC_H_
#define ADC_H_

// Inkluderingsdirektiv: 
#include "definitions.h"
#include "Serial.h"

/******************************************************************************
* Formler för beräkning av temperatur:
*
* Analog inspänning Uin:
* Uin = Vcc * ADC_result / ADC_max,
* där ADC_result är resultatet från senaste AD-omvandling (0 - 1023).
*
* Temperatur temp i grader Celcius:
* temp = 100 * Uin - 50,
* där Uin är den beräknade analoga inspänningen (0 - 5 V).
******************************************************************************/

#define VCC 5.0f // Matningsspänning 5 V.
#define ADC_MAX 1023.0 // Maxvärde vid AD-omvandling. Ändrade till 1023.0

/******************************************************************************
* För att välja intern matningsspänning för AD-omvandlaren, så ettställs biten
* REFS0 i register ADMUX (ADC Multiplexer Select Register). Som default läses
* analog PIN A0 av. För att starta AD-omvandlingen så ettställs samtliga
* bitar i registret ADCSRA (ADC Control and Status Register A). För att 
* kontrollera så att en given AD-omvandling är slutförd innan resultatet 
* hämtas, så sker avläsning av biten ADIF (ADC Interrupt Flag), så blir 
* ettställt när AD-omvandlingen är färdig. Så länge denna bit är nollställd,
* så väntar vi in att AD-omvandlingen är färdig via en while-sats. Biten
* ADCSRA återställs inför nästa AD-omvandling genom att denna nollställs.
******************************************************************************/

#define SELECT_ANALOG_CHANNEL ADMUX = (1 << REFS0)
#define START_AD_CONVERSION ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2)
#define WAIT_FOR_AD_CONVERSION_COMPLETE while ((ADCSRA & (1 << ADIF)) == 0)
#define RESET_ADC_INTERRUPT_FLAG ADCSRA = (1 << ADIF)

/******************************************************************************
* Strukten TempSensor används för implementering av en temperatursensor
* ansluten till en given analog PIN A0 - A5.
******************************************************************************/
struct TempSensor 
{
	uint8_t PIN; // PIN-nummer för avläsning.
};

// Funktionsdeklarationer:
struct TempSensor new_TempSensor(const uint8_t PIN);
void print_temperature(const struct TempSensor* self); 

#endif /* ADC_H_ */
