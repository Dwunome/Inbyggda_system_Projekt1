#include "ADC.h" // Inkluderar egen headerfil.

// Statiska funktioner:
static void init_ADC(void);
static uint16_t ADC_read(const uint8_t PIN);

/******************************************************************************
* Funktionen new_TempSensor används för implementering av en temperatursensor 
* ansluten till någon av analoga pinnar A0 - A5 via ett objekt av strukten
* TempSensor. Ingående argument PIN utgör en pekare till aktuellt PIN-nummer. 
* Ett objekt av strukten TempsSensor deklareras och döps till self, där sparas 
* aktuellt PIN-nummer. AD-omvandlaren initieras sedan via anrop av statiska 
* funktionen init_ADC. Sedan returneras objektet för användning.
******************************************************************************/
struct TempSensor new_TempSensor(const uint8_t PIN)
{
	struct TempSensor self;
	self.PIN = PIN;
	init_ADC();
	return self;
}

/******************************************************************************
* Funktionen print_temperature används för att läsa av rumstemperaturen och 
* skriva till vår PC. Först läses temperatursensorn av och resultatet lagras 
* i konstanten ADC_result. Därefter beräknas motsvarande analoga inspänning Uin 
* med följande formel:

* input_voltage = Vcc * ADC_result / ADC_MAX,
*
* där ADC_result är resultatet från senaste AD-omvandling (0 - 1023).
* Därefter beräknas motsvarande temperatur i grader Celcius via följande formel:
*
* temperatur = 100 * Uin - 50,
*
* där Uin är den beräknade analoga inspänningen (0 - 5 V).
*
* Temperaturen som beräknas lagras i konstanten temperature. Det värdet avrundas 
* till närmsta heltal och lagras i konstanten rounded_temperature. Sedan 
* transmitterras serial_print_integer, som möjliggör sammansättning av text och
* heltal, samt seriell överföring. Med det klart så transmitteras 
* textstycket: "Temperature: %ld degress Celcius\n".
* %ld är en formatspecificerare för heltal och ersätts med avläst temperatur.
******************************************************************************/
void print_temperature(const struct TempSensor* self)
{
	const uint16_t ADC_result = ADC_read(self->PIN);
	const double input_voltage = VCC * (double)(ADC_result) / ADC_MAX; 
	const double temperature = 100 * input_voltage - 50;
	const int32_t rounded_temperature = (int32_t)(temperature + 0.5);
	serial_print_integer("Temperature: %d degrees Celcius\n", rounded_temperature); 
	return;
}
 
  /******************************************************************************
  * Funktionen init_ADC las till vid korrigering av koden:
  * Funktionen init_ADC används för att initiera AD-omvandlaren. Först sätts
  * AD-omvandlaren till att matas med intern matningsspänning. Därefter aktiveras
  * AD-omvandlaren och startas med lägsta möjliga frekvens (125 kHz) för högsta
  * möjliga precision. Därefter väntar vi till att AD-omvandlingen är slutförd,
  * vilket signaleras via interrupt-flaggan ADIF (ADC Interrupt Flag), som då
  * blir ettställd. För att sedan återställa ADIF inför nästa AD-omvandling
  * så ettställs denna innan återhoppet.
  *******************************************************************************/
 static void init_ADC(void) 
 {
	 ADMUX = (1 << REFS0);
	 ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);
	 while ((ADCSRA & (1 << ADIF)) == 0);
	 ADCSRA = (1 << ADIF);
	 return;
 }
 
  /******************************************************************************
* Funktionen ADC_read används för att läsa av temperatursensorn och returnera 
* resultatet. Först väljs analog kanal för avläsning, samtidigt som 
* AD-omvandlaren sätts till att matas med intern matningsspänning. 
* Därefter aktiveras AD-omvandlaren och startas med lägsta möjliga 
* frekvens (125 kHz) för högsta möjliga precision. Därefter inväntar programmet
* till att AD-omvandlingen är slutförd, vilket signaleras via AD-omvandlarens 
* interrupt-flagga ADIF (ADC Interrupt Flag), som då blir ettställd. 
* För att sedan återställa ADIF inför nästa AD-omvandlaren så ettställs denna, 
* följt av att avläst resultat returneras vid återhoppet.
 ******************************************************************************/
static uint16_t ADC_read(const uint8_t PIN)
{
	ADMUX = (1 << REFS0) | PIN; 
	ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); 
	while ((ADCSRA & (1 << ADIF)) == 0); 
	ADCSRA = (1 << ADIF); 
	return ADC;
}
