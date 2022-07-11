// Inkluderingsdirektiv:
#include "Timer.h"

static void init_timer(const TimerSelection timerSelection);
static inline uint32_t get_required_interrupts(const double delay_time);

/******************************************************************************
* Funktionen new_Timer används för att skapa och initiera objekt av strukten 
* Timer. Ingående argument timerSelection används för att välja vilken av
* timerkretsar Timer 0 - 2 som skall användas, medan delay_time utgörs av
* den fördröjningstid som timern skall räkna till, mätt i millisekunder.
*
* Först allokeras minne för ett nytt objekt av strukten Timer, som döps 
* till self. Om minnesallokeringen misslyckas returneras NULL direkt. Annars
* initieras struktens medlemmar; enabled sätts till false för att indikera
* att timer-kretsen är avstängd vid start, vald timerkrets sparas via 
* medlemmen timerSelection, antalet exekverade avbrott sätts till noll vid
* start, medan antalet avbrott som krävs för specificerad fördröjningstid
* beräknas via anrop av funktionen get_required_interrupts, där ingående 
* argument delay_time passeras som parameter. Returvärdet från detta anrop,
* vilket är beräknat antalet avbrott som krävs för specificerad fördröjningtid, 
* lagras sedan via medlemmen required_interrupts. Slutligen returneras det nu 
* initierade objektet self, som är redo att användas för implementering av 
* en given timerkrets.
******************************************************************************/

struct Timer new_Timer(const TimerSelection timerSelection, const double delay_time) 
{
	struct Timer self;
	 
	self.enabled = false;
	self.timerSelection = timerSelection;
	self.executed_interrupts = 0x00;
	self.required_interrupts = get_required_interrupts(delay_time);
	init_timer(self.timerSelection);
	return self;
}

/******************************************************************************
* Funktionen Timer_on används för att aktivera en given timer. Ingående
* argument self utgör en pekare till ett timerobjekt, vars medlem 
* timerSelection undersöks för att ta reda på vilken timerkrets som skall
* aktiveras. Aktuell timerkrets aktiveras, följt av att medlemmen enabled 
* sätts till true för att indikera att timern i fråga nu är aktiverad.
******************************************************************************/

void Timer_on(struct Timer* self) 
{
	if (self->timerSelection == TIMER0)
	{
		TIMSK0 = (1 << TOIE0);
	}
	
	else if (self->timerSelection == TIMER1)
	{
		TIMSK1 = (1 << OCIE1A);
	}
	
	else if (self->timerSelection == TIMER2)
	{
		TIMSK2 = (1 << TOIE2);
	}
	
	self->enabled = true;
	return;
}

/******************************************************************************
* Funktionen Timer_off används för att inaktivera en given timer. Ingående
* argument self utgör en pekare till ett timerobjekt, vars medlem
* timerSelection undersöks för att ta reda på vilken timerkrets som skall
* inaktiveras. Aktuell timerkrets inaktiveras, följt av att medlemmen enabled
* sätts till false för att indikera att timern i fråga nu är inaktiverad.
******************************************************************************/

 void Timer_off(struct Timer* self)
{
	if (self->timerSelection == TIMER0)
	{
		TIMSK0 = 0x00;
	}
	
	else if (self->timerSelection == TIMER1)
	{
		TIMSK1 = 0x00;
	}
	
	else if (self->timerSelection == TIMER2)
	{
		TIMSK2 = 0x00;
	}
	
	self->enabled = false;
	return;
}

/******************************************************************************
* Funktionen Timer_toggle används för att toggla aktivering av en given timer.
* Ingående argument self utgör en pekare till ett timerobjekt, vars medlem
* enabled undersöks för att ta reda på om timern just nu är aktiverad eller 
* inte. Om timern för tillfället är aktiverad (enabled är true), så stängs den
* av via funktionen Timer_off, som anropas via pekaren off. Däremot om timern 
* för tillfället är inaktiverad (enabled är false), så stängs den av via 
* funktionen Timer_on, som anropas via pekaren on.
******************************************************************************/
void Timer_toggle(struct Timer* self) 
{
	if (self->enabled)
	{
		Timer_off(self);
	}
	
	else
	{
		Timer_on(self);
	}
	
	return;
}

/******************************************************************************
* Funktionen Timer_count används för att räkna antalet exekverade avbrott som
* har ägt rum. Om timern i fråga är aktiverad så räknas antalet exekverade
* avbrott upp via inkrementering av medlemmen executed_interrupts. Annars
* om timern är inaktiverad sker ingen uppräkning, så att timern inte av 
* misstag skall löpa ut och orsaka avbrott när den är avstängd.
******************************************************************************/

void Timer_count(struct Timer* self) 
{
	if (self->enabled)
	{
		self->executed_interrupts++;
	}
	
	return;
}

/******************************************************************************
* Funktionen Timer_elapsed används för att undersöka ifall en given timer 
* har löpt ut och i så fall nollställa timern. Ingående argument self utgör en
* pekare till ett timerobjekt. För att ta reda på ifall timern har löpt ut så
* undersöks antalet exekverade avbrott, vilket lagras av timerobjektets medlem 
* executed_interrupts) mot antalet avbrott som krävs för aktuell fördröjning 
* som timern skall medföra, vilket lagras av medlemmen required_interrupts. 
*
* Om timern har löpt ut så kommer antalet exekverade avbrott överstiga eller 
* vara lika med antalet avbrott som krävs för fördröjningen. Om detta är fallet
* så nollställs antalet exekverade avbrott, vilket motsvarar att timern
* nollställs, följt av att true returneras för att indikera att timern har 
* har löpt ut. Annars om timern inte har löpt ut så returneras false utan att
* timern nollställs. 
******************************************************************************/

bool Timer_elapsed(struct Timer* self)
{
	if (self->executed_interrupts >= self->required_interrupts)
	{
		self->executed_interrupts = 0x00;
		return true;
	}
	
	return false;
}

/******************************************************************************
* Funktionen Timer_clear används för att nollställa en given timer. Ingående
* argument self utgör en pekare till ett timerobjekt, vars medlem 
* executed_interrupts nollställs för att därigenom nollställs timerkretsen.
******************************************************************************/

void Timer_clear(struct Timer* self)
{
	self->executed_interrupts = 0x00;
	return;
}


/******************************************************************************
* Funktionen Timer_reset används för att återställa en given timer, vilket
* innebär att timern stängs av och nollställs. Först stängs timern av via
* funktionen Timer_off, som anropas via pekaren off. Därefter nollställs antalet
* exekverade avbrott för att nollställs timern, vilket sker via nollställning
* av timerobjektets medlem executed_interrupts.
******************************************************************************/

void Timer_reset(struct Timer* self) 
{
	Timer_off(self);
	self->executed_interrupts = 0x00;
	return;
}

/******************************************************************************
* Funktionen Timer_set används för att uppdatera fördröjningstiden på en given
* timer. Ingående argument self utgörs av en pekare till ett timerobjekt, 
* medan delay_time utgör den nya fördröjningstiden, mätt i millisekunder.
* Timerns fördröjningstid, som lagras av medlemmen required_interrupts, 
* uppdateras via anrop av funktionen get_required_interrupts, där ingående
* argument delay_time passeras som ingående argument.
******************************************************************************/

void Timer_set(struct Timer* self, const double delay_time) 
{
	self->required_interrupts = get_required_interrupts(delay_time);
	return;
}

/******************************************************************************
* Funktionen init_timer används för att initiera en given timerkrets för en
* uppräkningstid där ett avbrott sker var 0.016:e millisekund vid uppräkning
* till 256, då overflow- eller CTC-avbrott sker, beroende på använd timerkrets. 
* Ingående argument timerSelection indikerar vilken timer som skall initieras.
*
* Först aktiveras avbrott globalt via ettställning av I-flaggan i statusregistret
* SREG. Denna bit måste alltid vara ettställd för att timergenererade avbrott
* skall kunna implementeras. Sedan initeras aktuell timerkrets, där de 8-bitars 
* timerkretsarna Timer 0 samt Timer 2 initieras i Normal Mode, vilket innebär 
* uppräkning tills overflow sker (efter uppräkning till 256, som inte ryms i 
* 8-bitars register).
* 
* För den 16-bitars timerkretsen Timer 1 används CTC Mode (Clear Timer On 
* Compare), där maxvärdet för uppräkning sätts till 256 för att matcha de
* andra två timerkretsarna, så att avbrott sker lika ofta för respektive 
* timer. Eftersom Timer 1 vid uppräkning till 256 är långt från full används
* därmed CTC Mode, där timern nollställs automatiskt vid uppräkning till
* förvalt maxvärde, vilket i detta fall är 256.
******************************************************************************/

static void init_timer(const TimerSelection timerSelection) 
{
	asm("SEI"); 
	
	if (timerSelection == TIMER0) 
	{
		TCCR0B = (1 << CS00); 
	}
	
	else if (timerSelection == TIMER1)
	{
		TCCR1B = (1 << WGM11);
		OCR1A = 256;
	}
	
	else if (timerSelection == TIMER2)
	{
		TCCR2B = (1 << CS20); 
	}
	
	return;
}

/******************************************************************************
* Funktionen get_required_interrupts används för att beräkna och antalet
* avbrott som krävs för en given fördröjningstid. Ingående argument delay_time
* utgörs av specificerad fördröjningstid mätt i millisekunder. Antalet avbrott
* som krävs för aktuell fördröjning beräknas som kvoten av specificerad 
* fördröjningstid genom tiden mellan varje avbrott, vilket i detta fall är
* satt till 0.016 ms, här implementerat via makrot INTERRUPT_TIME. Antalet
* avbrott som krävs för specificerad fördröjningstid avrundas till närmaste
* heltal via anrop av funktionen round_to_integer och returneras sedan.
******************************************************************************/

static inline uint32_t get_required_interrupts(const double delay_time)
{
	return (uint32_t)(delay_time / INTERRUPT_TIME + 0.5);
}
