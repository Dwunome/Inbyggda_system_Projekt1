// Inkluderingsdirektiv:
#include "header.h"

/******************************************************************************
* Avbrottsrutin för PCI-avbrott för I/O-port B. Vid aktivering av denna
* avbrottsrutin inaktiveras PCI-avbrott på tryckknappens PIN 13, vilket i
* detta fall är enda källan till PCI-avbrott på I/O-porten i fråga. Detta görs
* för att förhindra påverkan av kontaktstudsar, som annars kan medför att 
* multipla avbrott äger rum kort efter varandra när knappen studsar. Timer 0
* aktiveras för att efter 300 ms återaktivera PCI-avbrott på PIN 13. Ifall
* nedtryckning av tryckknappen orsakade aktuellt avbrott, så mäts aktuell
* rumstemperatur och skrivs ut i en seriell terminal. För att indikera att
* temperaturavläsning genomförs så togglas led1. Samtidigt nollställs Timer 1.
******************************************************************************/
ISR (PCINT0_vect)
{
	Button_disable_interrupt(&button); 
	Timer_on(&timer0); 
	
	if (Button_is_pressed(&button)) 
	{
		DynamicTimer_update(&timer1);
		print_temperature(&tempSensor);		
		Led_toggle(&led1);	
	}
	
	return;
}

/******************************************************************************
* Avbrottsrutin för Timer 0 i Normal Mode, vilket sker var 0.016:e millisekund
* då timern i fråga är aktiverad. Denna avbrottsrutin används för att generera 
* en bouncetid på 300 ms, där PCI-avbrott på PIN 13 hålls inaktiverat efter ett 
* givet avbrott för att förhindra att multipla äger rum på grund av 
* kontakstudsar. När tillräckligt många avbrott har ägt rum så att timern har 
* löpt ut, så inaktiveras Timer 0 och PCI-avbrott på PIN 13 återaktiveras.
******************************************************************************/
ISR (TIMER0_OVF_vect)
{
	Timer_count(&timer0);
	
	if (Timer_elapsed(&timer0)) 
	{
		Timer_off(&timer0); 
		Button_enable_interrupt(&button); 	
	}
	
	return;
}

/******************************************************************************
* Avbrottsrutin för Timer 1 i CTC Mode, vilket sker var 0.016:e millisekund då
* timern i fråga är aktiverad. Denna avbrottsrutin används för att mäta
* rumstemperaturen var 60:e sekund, alternativt 60 sekunder efter senaste
* knapptryckning. Varje gång denna rutin aktiveras så räknas antalet exekverade 
* avbrott upp. När tillräckligt många avbrott har ägt rum så att timern har löpt 
* ut, så mäts rumstemperaturen och skrivs ut i terminalen. För att indikera att 
* temperaturmätning genomförs så togglas lysdioden. 
******************************************************************************/
ISR (TIMER1_COMPA_vect)
{
	DynamicTimer_count(&timer1);	
	
	if (DynamicTimer_elapsed(&timer1)) 
	{
		print_temperature(&tempSensor); 
		Led_toggle(&led1); 
	}
	return;
}