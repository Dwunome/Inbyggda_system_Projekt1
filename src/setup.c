/* Inkluderingsdirektiv: */
#include "header.h"

static void init_GPIO(void);
static void init_timers(void);
static void init_analog(void);

/*
* EDIT: Tog bort variabeldeklarationerna i början av setup och deklarerade variablerna direkt inne
* i funktionsanropet. Skapade separata funktioner för initiering av GPIO, timers, anaolg.
*/

/******************************************************************************
* Funktionen setup används för att initiera programmets globala variabler.
* Först implementeras en lysdiod på PIN 9 via ett objekt av strukten Led, 
* som döps till led1. Sedan implementeras en tryckknapp på PIN 13 via ett 
* objekt av struken Button, som döps till button. PCI-avbrott aktiveras på 
* tryckknappens PIN för avläsning av aktuell rumstemperatur, där lysdioden
* togglas vid varje avläsning.
* 
* Därefter implementeras timerkretsen Timer 0, som används för att generera
* en bouncetid på 300 ms efter nedtryckning av tryckknappar för att förhindra
* att kontaktstudsar orsakar multipla avbrott. Ytterligare en timerkrets, 
* Timer 1, används för att mäta temperaturen med ett visst intervall, vilket
* vid start är 60 sekunder. Därmed aktiveras denna timer direkt.
* Slutligen initeras seriell överföring via anrop av funktionen serial, 
* vilket möjliggör transmission till PC.
******************************************************************************/

void setup(void)
{
	init_serial();
	init_GPIO();
	init_timers();
	init_analog();
	
	serial_print("Dynamic temperature measurement system!\n");
	return;
}

static void init_GPIO(void)
{
	led1 = new_Led(9);			// i variabeln led1 lagras den instansierade struktmedlemmen self av datatypen struct Led. variabeln led1 är global och har initierats i header.h.
	button = new_Button(13);
	Button_enable_interrupt(&button);
	return;
}

static void init_timers(void)
{
	timer0 = new_Timer(TIMER0, 300);
	timer1 = new_DynamicTimer(TIMER1, 60000);
	DynamicTimer_on(&timer1);
	return;
}

/*
* Deklarerar en temperatursensor ansluten till analog PIN A1 via ett objekt.
* av strukten tempSensor.
*/
static void init_analog(void)
{
	tempSensor = new_TempSensor(1);
	return;
}



