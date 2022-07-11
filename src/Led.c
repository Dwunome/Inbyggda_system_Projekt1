// Inkluderingsdirektiv:
#include "GPIO.h"

static void delay(const uint16_t delay_time);
/******************************************************************************
* Funktionen new_Led utgör initieringsrutin för objekt av strukten Led.
* Ingående argument PIN utgör aktuellt PIN-nummer sett till Arduino Uno
* (PIN 0 - 13), som är ekvivalent med följande:
*
*******************************************************************************
* PIN (Arduino Uno)          I/O-port          PIN (ATmega328P)               *
*     0 - 7                     D         Samma som PIN på Arduino Uno        *
*     8 - 13                    B            PIN på Arduino Uno - 8           *
*******************************************************************************
*
* Först allokeras minne för ett nytt objekt av strukten Led som döps till self.
* Om minnesallokeringen misslyckas så returneras NULL direkt. Annars initieras
* objektets instansvariabler. Om aktuellt PIN-nummer ligger mellan 0 - 7, så 
* är lysdioden ansluten till I/O-port D, vilket lagras via instansvariabeln 
* io_port. Aktuellt PORT-nummer är då samma samma som PIN-numret, vilket 
* lagras via instansvariabel PIN. Denna PIN sätts till utport genom att
* motsvarande bit i datariktningsregister DDRD (Data Direction Register D)
* ettställs. Bitvis OR |= används för att enbart ettställa aktuell bit utan 
* att påverka övriga bitar.
* 
* Motsvarande genomförs ifall aktuellt PIN-nummer ligger mellan 8 - 13, med
* skillnaden att I/O-porten då utgörs av I/O-port B, PIN-numret är lika med
* erhållet PIN-nummer på Arduino Uno - 8, och motsvarande PIN sätts till
* utport via ettställning av motsvarande bit i datariktningsregister DDRB
* (Data Direction Register B).
*
* Slutligen sätts pekarna till att peka på motsvarande funktioner, följt
* av att det nu initierade objektet returneras. Kom ihåg: self.on = Led_on
* betyder att pekaren on pekar på funktionen Led_on.
******************************************************************************/

struct Led new_Led(const uint8_t PIN) 
{
	struct Led self;			// Skapar en variabel/medlem av strukten Led som döps till self.
	self.enabled = false;			// LED ges startvärdet false (släckt) vid initiering/start av denna funktion.
	
	if (PIN >= 0 && PIN <= 7)		// Om ingående parameter (inmatad & önskad PIN vid detta funktionsanrop) är mellan 0 & 7 så sker raderna nedan:
	{
		self.io_port = IO_PORTD;	// Ger instansvariabeln / objektet self.io_port värdet av enumerationen("makrot") IO_port -> IO_PORTD.
		self.PIN = PIN;
		DDRD |= (1 << self.PIN);	// Sätter önskad pin från self.PIN till utport i DDRD.
	}
	
	else if (PIN >= 8 && PIN <= 13)		// Annars om ingående parameter (inmatad & önskad PIN vid detta funktionsanrop) är mellan 8 & 13 så sker raderna nedan:
	{
		self.io_port = IO_PORTB;	// Ger instansvariabeln / objektet self.io_port värdet av enumerationen("makrot") IO_port -> IO_PORTB.
		self.PIN = PIN - 8;
		DDRB |= (1 << self.PIN);	// Sätter önskad pin från self.PIN till utport i DDRB.
	}
	
	 return self;
}

/******************************************************************************
* Funktionen Led_on används för att tända en lysdiod. Ingående argument self
* utgör en pekare till led-objektet i fråga. Utefter aktuell I/O-port så 
* ettställs motsvarande bit i register PORTB eller PORTD.
******************************************************************************/

void Led_on(struct Led* self)
{
	if (self->io_port == IO_PORTB)		// Om struktmedlemmen Led's io_port har värdet IO_PORTB sker villkoret nedan:
	{
		PORTB |= (1 << self->PIN);	// Ettställer önskad bit (PIN som man vill tända) i PORTB och tänder därmed lysdioden.
	}
	
	else if (self->io_port == IO_PORTD) 	// Annars om struktmedlemmen Led's io_port har värdet IO_PORTD sker villkoret nedan:
	{
		PORTD |= (1 << self->PIN);	// Ettställer önskad bit (PIN som man vill tända) i PORTB och tänder därmed lysdioden.
	}
	
	self->enabled = true;			// Sätter slutligen enabled medlemmen till true för att indikera att lysdioden är tänd. 
	return;
}

/******************************************************************************
* Funktionen Led_off används för att släcka en lysdiod. Ingående argument
* self utgör en pekare till lysdioden. Utefter aktuell I/O-port så nollställs
* motsvarande bit i register PORTB eller PORTD.
******************************************************************************/

 void Led_off(struct Led* self)
{
	if (self->io_port == IO_PORTB)		// Om struktmedlemmen Led's io_port har värdet IO_PORTB sker villkoret nedan: 
	{
		PORTB &= ~(1 << self->PIN);	// Nollställer önskad bit (PIN som man vill släcka) i PORTB och släcker därmed lysdioden.
	}
	
	else if (self->io_port == IO_PORTD)	// Om struktmedlemmen Led's io_port har värdet IO_PORTD sker villkoret nedan:
	{
		PORTD &= ~(1 << self->PIN);	// Nollställer önskad bit (PIN som man vill släcka) i PORTD och släcker därmed lysdioden.
	}
	
	self->enabled = false;			// Sätter slutligen enabled medlemmen till false för att indikera att lysdioden är släckt. 
	return;
}

/******************************************************************************
* Funktionen Led_toggle används för att toggla en lysdiod. För att genomföra
* detta undersöks medlemmen enabled. Om denna är true så är lysdioden tänd
* och då släcks lysdioden via anrop av funktionen Led_off (via pekaren off).
* Annars så tänds lysdioden via anrop av funktionen Led_on (via pekaren on).
******************************************************************************/

void Led_toggle(struct Led* self)
{
	if (self->enabled)
		Led_off(self);
	else
		Led_on(self);
	return;
}

void Led_blink(struct Led* self, const uint16_t delay_time)
{
	Led_toggle(self);
	delay(delay_time);
	Led_toggle(self);
	delay(delay_time);
	return;
}

/*********************************************************************************************
* Funktionen delay används för att generera fördröjning via anrop av funktionen _delay_ms
* från biblioteket util/delay.h. Eftersom denna funktion enbart kan hantera ett definierat
* heltal vid kompilering, så kan ingående argument delay_time inte används vid anropet av
* denna funktion. I stället används en for-sats, som itererar lika många varv som storleken
* på ingående argument delay_time, där en fördröjningstid på 1 millisekund genereras varje
* varv. Som exempel, ifall delay_time är 100, så genereras 100 millisekunders fördröjning
* via 100 varv * 1 millisekunds fördröjning varje varv, vilket medför 100 millisekunders
* fördröjning totalt sett.
*********************************************************************************************/

static void delay(const uint16_t delay_time)
{
	for (register uint16_t i = 0; i < delay_time; i++) _delay_ms(1); 
	return;
}
