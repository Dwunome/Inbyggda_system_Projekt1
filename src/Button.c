// Inkluderingsdirektiv:
#include "GPIO.h"

/*
* EDIT: Tagit bort minnesallokeringen för instansvariabeln / objektet self.
* Lagt till instansvariabeln self i strukten.
* Tagit bort pekare.
* Lagt till ettställning av I-flaggan i SREG för avbrott.
* Ändrat bit-makro vid button_enable_interrupts från ASSIGN till SET_BIT.
*/

/******************************************************************************
* Initieringsrutin för tryckknappar. * Ingående argument PIN utgör aktuellt 
* PIN-nummer sett till Arduino Uno (PIN 0 - 13), som är ekvivalent med följande:
*
*******************************************************************************
* PIN (Arduino Uno)          I/O-port          PIN (ATmega328P)               *
*     0 - 7                     D         Samma som PIN på Arduino Uno        *
*     8 - 13                    B            PIN på Arduino Uno - 8           *
*******************************************************************************
*
* Först deklareras ett nytt objekt av strukten Button som döps till self.
* Därefter undersöks vilket PIN-nummer som tryckknappen är ansluten till.
* Om aktuellt PIN-nummer ligger mellan 0 - 7, är knappen ansluten till samma
* PIN på I/O-port D. Annars om PIN-nummret ligger mellan 8 - 13, så är
* tryckknappen ansluten till PIN 0 - 5 på I/O-port B. En intern pullup-resistor
* på tryckknappens PIN aktiveras via ettställning av motsvarande bit i aktuellt 
* PORT-register. Detta medför att tryckknappens insignal alltid är hög eller låg.
******************************************************************************/
struct Button new_Button(const uint8_t PIN)
{
	struct Button self;
	
	self.interrupt_enabled = false; // Sätter instansvariabeln interrupt_enabled till false. (inga PCI avbrott / avbrottsvektorer är möjliggjorda vid start).
	
	if (PIN >= 0 && PIN <= 7)		// Om ingående parameter (inmatad & önskad PIN vid detta funktionsanrop) är mellan 0 & 7 så sker raderna nedan:
	{
		self.io_port = IO_PORTD;	// Ger instansvariabeln / objektet self.io_port värdet av enumerationen("makrot") IO_port -> IO_PORTD.
		self.PIN = PIN;
		PORTD |= (1 << self.PIN);	// Sätter önskad pin från self.PIN till "hög insignal" i PORTD.
	}
	
	else if (PIN >= 8 && PIN <= 13)	// Annars om ingående parameter (inmatad & önskad PIN vid detta funktionsanrop) är mellan 8 & 13 så sker raderna nedan:
	{
		self.io_port = IO_PORTB;	// Ger instansvariabeln / objektet self.io_port värdet av enumerationen("makrot") IO_port -> IO_PORTB.
		self.PIN = PIN - 8;			
		PORTB |= (1 << self.PIN);	// Sätter önskad pin från self.PIN till "hög insignal" i PORTB.
	}
	return self;
}

/******************************************************************************
* Funktionen Button_is_pressed används för att indikera ifall en given 
* tryckknapp är nedtryckt. Om tryckknappen är ansluten till I/O-port B, så
* läses motsvarande PIN från registret PINB och returneras. Annars om 
* tryckknappen är ansluten till I/O-port D, så läses motsvarande PIN från 
* registret PIND och returneras. Annars vid fel så returneras false.
******************************************************************************/
bool Button_is_pressed(struct Button* self)
{
	if (self->io_port == IO_PORTB)			/*/ Om instansvariabeln / objektet self.io_port är = IO_PORTB utförs raden nedan: */
	{
		return (PINB & (1 << self->PIN));	/*/ Läser av respektive PIN som ligger lagrad i self.PIN, från registret PINB och returnerar detta. */
	}
	
	else if (self->io_port == IO_PORTD)		/*/ Om instansvariabeln / objektet self.io_port är = IO_PORTD utförs raden nedan: */
	{
		return (PIND & (1 << self->PIN));	/*/ Läser av respektive PIN som ligger lagrad i self.PIN, från registret PIND och returnerar detta. */
	}
	
	return false;		/*/ Vid fel / villkoren ovan ej uppfylls returneras false. */
}

/******************************************************************************
* Funktionen Button_enable_interrupt används för att aktivera PCI-avbrott på
* en given PIN som en tryckknapp är ansluten till. Först aktiveras avbrott
* globalt via ettställning av bit I (Interrupt Flag) i statusregistret SREG.
* Därefter undersöks använd I/O-port. Om tryckknappen är ansluten till
* I/O-port B, så ettställs biten PCIE0 (PIN Change Interrupt Enable 0) i
* kontrollregistret PCICR (PIN Change Interrupt Control Register), följt av
* att motsvarande PCI-avbrott aktiveras i maskregistret PCMSK0 (PIN Change
* Mask Register 0). Annars om tryckknappen är ansluten till I/O-port D,
* så ettställs biten PCIE2 (PIN Change Interrupt Enable 2) i kontrollregistret
* PCICR (PIN Change Interrupt Control Register), följt av
* att motsvarande PCI-avbrott aktiveras i maskregistret PCMSK2 (PIN Change
* Mask Register 2). Slutligen sätts instansvariabeln interrupt_enabled till
* true för att indikera att abrott nu är aktiverat.
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
* Funktionen Button_disable_interrupt används för att inaktivera avbrott för
* en given PIN, där en tryckknapp är ansluten. Detta åstadkommes via
* nollställning av motsvarande bit i någon av registren PCMSK0 (I/O-port B)
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
