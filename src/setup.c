/* Inkluderingsdirektiv: */
#include "header.h"

static void init_GPIO(void);
static void init_timers(void);
static void init_analog(void);

/*
* EDIT: Tog bort variabeldeklarationerna i b�rjan av setup och deklarerade variablerna direkt inne
* i funktionsanropet. Skapade separata funktioner f�r initiering av GPIO, timers, anaolg.
*/

/******************************************************************************
* Funktionen setup anv�nds f�r att initiera programmets globala variabler.
* F�rst implementeras en lysdiod p� PIN 9 via ett objekt av strukten Led, 
* som d�ps till led1. Sedan implementeras en tryckknapp p� PIN 13 via ett 
* objekt av struken Button, som d�ps till button. PCI-avbrott aktiveras p� 
* tryckknappens PIN f�r avl�sning av aktuell rumstemperatur, d�r lysdioden
* togglas vid varje avl�sning.
* 
* D�refter implementeras timerkretsen Timer 0, som anv�nds f�r att generera
* en bouncetid p� 300 ms efter nedtryckning av tryckknappar f�r att f�rhindra
* att kontaktstudsar orsakar multipla avbrott. Ytterligare en timerkrets, 
* Timer 1, anv�nds f�r att m�ta temperaturen med ett visst intervall, vilket
* vid start �r 60 sekunder. D�rmed aktiveras denna timer direkt.
* Slutligen initeras seriell �verf�ring via anrop av funktionen serial, 
* vilket m�jligg�r transmission till PC.
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
	led1 = new_Led(9);			// i variabeln led1 lagras den instansierade struktmedlemmen self av datatypen struct Led. variabeln led1 �r global och har initierats i header.h.
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



