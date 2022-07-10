// Inkluderingsdirektiv:
#include "GPIO.h"

static void delay(const uint16_t delay_time);
/******************************************************************************
* Funktionen new_Led utg�r initieringsrutin f�r objekt av strukten Led.
* Ing�ende argument PIN utg�r aktuellt PIN-nummer sett till Arduino Uno
* (PIN 0 - 13), som �r ekvivalent med f�ljande:
*
*******************************************************************************
* PIN (Arduino Uno)          I/O-port          PIN (ATmega328P)               *
*     0 - 7                     D         Samma som PIN p� Arduino Uno        *
*     8 - 13                    B            PIN p� Arduino Uno - 8           *
*******************************************************************************
*
* F�rst allokeras minne f�r ett nytt objekt av strukten Led som d�ps till self.
* Om minnesallokeringen misslyckas s� returneras NULL direkt. Annars initieras
* objektets instansvariabler. Om aktuellt PIN-nummer ligger mellan 0 - 7, s� 
* �r lysdioden ansluten till I/O-port D, vilket lagras via instansvariabeln 
* io_port. Aktuellt PORT-nummer �r d� samma samma som PIN-numret, vilket 
* lagras via instansvariabel PIN. Denna PIN s�tts till utport genom att
* motsvarande bit i datariktningsregister DDRD (Data Direction Register D)
* ettst�lls. Bitvis OR |= anv�nds f�r att enbart ettst�lla aktuell bit utan 
* att p�verka �vriga bitar.
* 
* Motsvarande genomf�rs ifall aktuellt PIN-nummer ligger mellan 8 - 13, med
* skillnaden att I/O-porten d� utg�rs av I/O-port B, PIN-numret �r lika med
* erh�llet PIN-nummer p� Arduino Uno - 8, och motsvarande PIN s�tts till
* utport via ettst�llning av motsvarande bit i datariktningsregister DDRB
* (Data Direction Register B).
*
* Slutligen s�tts pekarna till att peka p� motsvarande funktioner, f�ljt
* av att det nu initierade objektet returneras. Kom ih�g: self.on = Led_on
* betyder att pekaren on pekar p� funktionen Led_on.
******************************************************************************/
struct Led new_Led(const uint8_t PIN) 
{
	struct Led self;				// Skapar en variabel/medlem av strukten Led som d�ps till self.
	self.enabled = false;			// LED ges startv�rdet false (sl�ckt) vid initiering/start av denna funktion.
	
	if (PIN >= 0 && PIN <= 7)		// Om ing�ende parameter (inmatad & �nskad PIN vid detta funktionsanrop) �r mellan 0 & 7 s� sker raderna nedan:
	{
		self.io_port = IO_PORTD;	// Ger instansvariabeln / objektet self.io_port v�rdet av enumerationen("makrot") IO_port -> IO_PORTD.
		self.PIN = PIN;
		DDRD |= (1 << self.PIN);	// S�tter �nskad pin fr�n self.PIN till utport i DDRD.
	}
	
	else if (PIN >= 8 && PIN <= 13)	// Annars om ing�ende parameter (inmatad & �nskad PIN vid detta funktionsanrop) �r mellan 8 & 13 s� sker raderna nedan:
	{
		self.io_port = IO_PORTB;	// Ger instansvariabeln / objektet self.io_port v�rdet av enumerationen("makrot") IO_port -> IO_PORTB.
		self.PIN = PIN - 8;
		DDRB |= (1 << self.PIN);	// S�tter �nskad pin fr�n self.PIN till utport i DDRB.
	}
	
	 return self;
}

/******************************************************************************
* Funktionen Led_on anv�nds f�r att t�nda en lysdiod. Ing�ende argument self
* utg�r en pekare till led-objektet i fr�ga. Utefter aktuell I/O-port s� 
* ettst�lls motsvarande bit i register PORTB eller PORTD.
******************************************************************************/
void Led_on(struct Led* self)
{
	if (self->io_port == IO_PORTB)		// Om struktmedlemmen Led's io_port har v�rdet IO_PORTB sker villkoret nedan:
	{
		PORTB |= (1 << self->PIN);		// Ettst�ller �nskad bit (PIN som man vill t�nda) i PORTB och t�nder d�rmed lysdioden.
	}
	
	else if (self->io_port == IO_PORTD) // Annars om struktmedlemmen Led's io_port har v�rdet IO_PORTD sker villkoret nedan:
	{
		PORTD |= (1 << self->PIN);		// Ettst�ller �nskad bit (PIN som man vill t�nda) i PORTB och t�nder d�rmed lysdioden.
	}
	
	self->enabled = true;				// S�tter slutligen enabled medlemmen till true f�r att indikera att lysdioden �r t�nd. 
	return;
}

/******************************************************************************
* Funktionen Led_off anv�nds f�r att sl�cka en lysdiod. Ing�ende argument
* self utg�r en pekare till lysdioden. Utefter aktuell I/O-port s� nollst�lls
* motsvarande bit i register PORTB eller PORTD.
******************************************************************************/
 void Led_off(struct Led* self)
{
	if (self->io_port == IO_PORTB)		// Om struktmedlemmen Led's io_port har v�rdet IO_PORTB sker villkoret nedan: 
	{
		PORTB &= ~(1 << self->PIN);		// Nollst�ller �nskad bit (PIN som man vill sl�cka) i PORTB och sl�cker d�rmed lysdioden.
	}
	
	else if (self->io_port == IO_PORTD)	// Om struktmedlemmen Led's io_port har v�rdet IO_PORTD sker villkoret nedan:
	{
		PORTD &= ~(1 << self->PIN);		// Nollst�ller �nskad bit (PIN som man vill sl�cka) i PORTD och sl�cker d�rmed lysdioden.
	}
	
	self->enabled = false;				// S�tter slutligen enabled medlemmen till false f�r att indikera att lysdioden �r sl�ckt. 
	return;
}

/******************************************************************************
* Funktionen Led_toggle anv�nds f�r att toggla en lysdiod. F�r att genomf�ra
* detta unders�ks medlemmen enabled. Om denna �r true s� �r lysdioden t�nd
* och d� sl�cks lysdioden via anrop av funktionen Led_off (via pekaren off).
* Annars s� t�nds lysdioden via anrop av funktionen Led_on (via pekaren on).
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
* Funktionen delay anv�nds f�r att generera f�rdr�jning via anrop av funktionen _delay_ms
* fr�n biblioteket util/delay.h. Eftersom denna funktion enbart kan hantera ett definierat
* heltal vid kompilering, s� kan ing�ende argument delay_time inte anv�nds vid anropet av
* denna funktion. I st�llet anv�nds en for-sats, som itererar lika m�nga varv som storleken
* p� ing�ende argument delay_time, d�r en f�rdr�jningstid p� 1 millisekund genereras varje
* varv. Som exempel, ifall delay_time �r 100, s� genereras 100 millisekunders f�rdr�jning
* via 100 varv * 1 millisekunds f�rdr�jning varje varv, vilket medf�r 100 millisekunders
* f�rdr�jning totalt sett.
*********************************************************************************************/
static void delay(const uint16_t delay_time)
{
	for (register uint16_t i = 0; i < delay_time; i++) _delay_ms(1); 
	return;
}