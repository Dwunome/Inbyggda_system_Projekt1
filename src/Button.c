// Inkluderingsdirektiv:
#include "GPIO.h"

/*
* EDIT: Tagit bort minnesallokeringen f�r instansvariabeln / objektet self.
* Lagt till instansvariabeln self i strukten.
* Tagit bort pekare.
* Lagt till ettst�llning av I-flaggan i SREG f�r avbrott.
* �ndrat bit-makro vid button_enable_interrupts fr�n ASSIGN till SET_BIT.
*/

/******************************************************************************
* Initieringsrutin f�r tryckknappar. * Ing�ende argument PIN utg�r aktuellt 
* PIN-nummer sett till Arduino Uno (PIN 0 - 13), som �r ekvivalent med f�ljande:
*
*******************************************************************************
* PIN (Arduino Uno)          I/O-port          PIN (ATmega328P)               *
*     0 - 7                     D         Samma som PIN p� Arduino Uno        *
*     8 - 13                    B            PIN p� Arduino Uno - 8           *
*******************************************************************************
*
* F�rst deklareras ett nytt objekt av strukten Button som d�ps till self.
* D�refter unders�ks vilket PIN-nummer som tryckknappen �r ansluten till.
* Om aktuellt PIN-nummer ligger mellan 0 - 7, �r knappen ansluten till samma
* PIN p� I/O-port D. Annars om PIN-nummret ligger mellan 8 - 13, s� �r
* tryckknappen ansluten till PIN 0 - 5 p� I/O-port B. En intern pullup-resistor
* p� tryckknappens PIN aktiveras via ettst�llning av motsvarande bit i aktuellt 
* PORT-register. Detta medf�r att tryckknappens insignal alltid �r h�g eller l�g.
******************************************************************************/
struct Button new_Button(const uint8_t PIN)
{
	struct Button self;
	
	self.interrupt_enabled = false; // S�tter instansvariabeln interrupt_enabled till false. (inga PCI avbrott / avbrottsvektorer �r m�jliggjorda vid start).
	
	if (PIN >= 0 && PIN <= 7)		// Om ing�ende parameter (inmatad & �nskad PIN vid detta funktionsanrop) �r mellan 0 & 7 s� sker raderna nedan:
	{
		self.io_port = IO_PORTD;	// Ger instansvariabeln / objektet self.io_port v�rdet av enumerationen("makrot") IO_port -> IO_PORTD.
		self.PIN = PIN;
		PORTD |= (1 << self.PIN);	// S�tter �nskad pin fr�n self.PIN till "h�g insignal" i PORTD.
	}
	
	else if (PIN >= 8 && PIN <= 13)	// Annars om ing�ende parameter (inmatad & �nskad PIN vid detta funktionsanrop) �r mellan 8 & 13 s� sker raderna nedan:
	{
		self.io_port = IO_PORTB;	// Ger instansvariabeln / objektet self.io_port v�rdet av enumerationen("makrot") IO_port -> IO_PORTB.
		self.PIN = PIN - 8;			
		PORTB |= (1 << self.PIN);	// S�tter �nskad pin fr�n self.PIN till "h�g insignal" i PORTB.
	}
	return self;
}

/******************************************************************************
* Funktionen Button_is_pressed anv�nds f�r att indikera ifall en given 
* tryckknapp �r nedtryckt. Om tryckknappen �r ansluten till I/O-port B, s�
* l�ses motsvarande PIN fr�n registret PINB och returneras. Annars om 
* tryckknappen �r ansluten till I/O-port D, s� l�ses motsvarande PIN fr�n 
* registret PIND och returneras. Annars vid fel s� returneras false.
******************************************************************************/
bool Button_is_pressed(struct Button* self)
{
	if (self->io_port == IO_PORTB)			/*/ Om instansvariabeln / objektet self.io_port �r = IO_PORTB utf�rs raden nedan: */
	{
		return (PINB & (1 << self->PIN));	/*/ L�ser av respektive PIN som ligger lagrad i self.PIN, fr�n registret PINB och returnerar detta. */
	}
	
	else if (self->io_port == IO_PORTD)		/*/ Om instansvariabeln / objektet self.io_port �r = IO_PORTD utf�rs raden nedan: */
	{
		return (PIND & (1 << self->PIN));	/*/ L�ser av respektive PIN som ligger lagrad i self.PIN, fr�n registret PIND och returnerar detta. */
	}
	
	return false;		/*/ Vid fel / villkoren ovan ej uppfylls returneras false. */
}

/******************************************************************************
* Funktionen Button_enable_interrupt anv�nds f�r att aktivera PCI-avbrott p�
* en given PIN som en tryckknapp �r ansluten till. F�rst aktiveras avbrott
* globalt via ettst�llning av bit I (Interrupt Flag) i statusregistret SREG.
* D�refter unders�ks anv�nd I/O-port. Om tryckknappen �r ansluten till
* I/O-port B, s� ettst�lls biten PCIE0 (PIN Change Interrupt Enable 0) i
* kontrollregistret PCICR (PIN Change Interrupt Control Register), f�ljt av
* att motsvarande PCI-avbrott aktiveras i maskregistret PCMSK0 (PIN Change
* Mask Register 0). Annars om tryckknappen �r ansluten till I/O-port D,
* s� ettst�lls biten PCIE2 (PIN Change Interrupt Enable 2) i kontrollregistret
* PCICR (PIN Change Interrupt Control Register), f�ljt av
* att motsvarande PCI-avbrott aktiveras i maskregistret PCMSK2 (PIN Change
* Mask Register 2). Slutligen s�tts instansvariabeln interrupt_enabled till
* true f�r att indikera att abrott nu �r aktiverat.
******************************************************************************/
void Button_enable_interrupt(struct Button* self)
{
	asm("SEI");
	
	if (self->io_port == IO_PORTB)
	{
		PCICR |= (1 << PCIE0); 
		PCMSK0 |= (1 << self->PIN);
	}
	
	else if (self->io_port == IO_PORTD)
	{
		PCICR |= (1 << PCIE2); 
		PCMSK2 |= (1 << self->PIN);
	}
	
	self->interrupt_enabled = true;
	return;
}

/******************************************************************************
* Funktionen Button_disable_interrupt anv�nds f�r att inaktivera avbrott f�r
* en given PIN, d�r en tryckknapp �r ansluten. Detta �stadkommes via
* nollst�llning av motsvarande bit i n�gon av registren PCMSK0 (I/O-port B)
* eller PCMSK2 (I/O-port D).
******************************************************************************/
void Button_disable_interrupt(struct Button* self)
{
	if (self->io_port == IO_PORTB)
		PCMSK0 &= ~(1 << self->PIN);
	
	else if (self->io_port == IO_PORTD)
		PCMSK2 &= ~(1 << self->PIN);
	
	self->interrupt_enabled = false;
	return;
}
