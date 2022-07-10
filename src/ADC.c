#include "ADC.h" // Inkluderar egen headerfil.

// Statiska funktioner:
static void init_ADC(void);
static uint16_t ADC_read(const uint8_t PIN);

/*
* EDIT: Tog bort minnesallokering f�r struct tempsensor
*/

/******************************************************************************
* Funktionen new_TempSensor anv�nds f�r implementering av en temperatursensor 
* ansluten till n�gon av analoga pinnar A0 - A5 via ett objekt av strukten
* TempSensor. Ing�ende argument PIN utg�r en pekare till aktuellt PIN-nummer. 
* Ett objekt av strukten TempsSensor deklareras och d�ps till self, d�r sparas 
* aktuellt PIN-nummer. AD-omvandlaren initieras sedan via anrop av statiska 
* funktionen init_ADC. Sedan returneras objektet f�r anv�ndning.
******************************************************************************/
struct TempSensor new_TempSensor(const uint8_t PIN)
{
	struct TempSensor self;
	self.PIN = PIN;
	init_ADC();
	return self;
}

/******************************************************************************
* Funktionen print_temperature anv�nds f�r att l�sa av rumstemperaturen och 
* skriva till v�r PC. F�rst l�ses temperatursensorn av och resultatet lagras 
* i konstanten ADC_result. D�refter ber�knas motsvarande analoga insp�nning Uin 
* med f�ljande formel:
*							input_voltage = Vcc * ADC_result / ADC_MAX,
*
* d�r ADC_result �r resultatet fr�n senaste AD-omvandling (0 - 1023).
*
* D�refter ber�knas motsvarande temperatur i grader Celcius via f�ljande formel:
*
*							temperatur = 100 * Uin - 50,
*
* d�r Uin �r den ber�knade analoga insp�nningen (0 - 5 V).
*
* Temperaturen som ber�knas lagras i konstanten temperature. Det v�rdet avrundas 
* till n�rmsta heltal och lagras i konstanten rounded_temperature. Sedan 
* transmitterras serial_print_integer, som m�jligg�r sammans�ttning av text och
* heltal, samt seriell �verf�ring. Med det klart s� transmitteras 
* textstycket: "Temperature: %ld degress Celcius\n".
* %ld �r en formatspecificerare f�r heltal och ers�tts med avl�st temperatur.
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
  * Funktionen init_ADC anv�nds f�r att initiera AD-omvandlaren. F�rst s�tts
  * AD-omvandlaren till att matas med intern matningssp�nning. D�refter aktiveras
  * AD-omvandlaren och startas med l�gsta m�jliga frekvens (125 kHz) f�r h�gsta
  * m�jliga precision. D�refter v�ntar vi till att AD-omvandlingen �r slutf�rd,
  * vilket signaleras via interrupt-flaggan ADIF (ADC Interrupt Flag), som d�
  * blir ettst�lld. F�r att sedan �terst�lla ADIF inf�r n�sta AD-omvandling
  * s� ettst�lls denna innan �terhoppet.
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
* Funktionen ADC_read anv�nds f�r att l�sa av temperatursensorn och returnera 
* resultatet. F�rst v�ljs analog kanal f�r avl�sning, samtidigt som 
* AD-omvandlaren s�tts till att matas med intern matningssp�nning. 
* D�refter aktiveras AD-omvandlaren och startas med l�gsta m�jliga 
* frekvens (125 kHz) f�r h�gsta m�jliga precision. D�refter inv�ntar programmet
* till att AD-omvandlingen �r slutf�rd, vilket signaleras via AD-omvandlarens 
* interrupt-flagga ADIF (ADC Interrupt Flag), som d� blir ettst�lld. 
* F�r att sedan �terst�lla ADIF inf�r n�sta AD-omvandlaren s� ettst�lls denna, 
* f�ljt av att avl�st resultat returneras vid �terhoppet.
 ******************************************************************************/
static uint16_t ADC_read(const uint8_t PIN)
{
	ADMUX = (1 << REFS0) | PIN; 
	ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); 
	while ((ADCSRA & (1 << ADIF)) == 0); 
	ADCSRA = (1 << ADIF); 
	return ADC;
}
