#include "DynamicTimer.h"

static inline size_t check_capacity(const size_t capacity);

/************************************************************************
* Funktionen används för att implementera en ny dynamisk timer.
* Ingående argument timerSelection utgörs av vald timerkrets och
* capacity utgör maxstorleken på arrayen som lagrar antalet exekverade 
* avbrott mellan varje knapptryckning.
************************************************************************/
struct DynamicTimer new_DynamicTimer(const TimerSelection timerSelection, const size_t capacity)
{
	struct DynamicTimer self;				// Skapar objektet self av strukten DynamicTimer.
	self.timer = new_Timer(timerSelection, 0x00);		// Initierar timern, av vid start.
	self.interrupt_vector = new_Vector();			// Initierar tom dynamisk array.
	self.interrupt_counter = 0x00;				// Avbrottsräknaren startar på noll.
	self.capacity = check_capacity(capacity);		// Sparar kontrollerad kapacitet.
	self.next = 0x00;					// Nästa element är första element vid start, på index noll.
	self.initiated = false;					// Timerns startvärde är false (av). Har ej startat förrän vi trycker på knappen.
	return self;						// Returnerar det färdiga objektet.
}

/************************************************************************
* DynamicTimer_check_capacity används för att kontrollera angiven kapacitet
* gällande antalet element som ska lagras av en dynamisk timer, 
* så att denna kapacitet ej överstiger maximalt tillåtna storlek.
* Det ingående argumentet capacity utgörs av angiven kapacitet. 
* Om angiven kapacitet överstiger högsta tillåtna storlek så returneras
* maxkapaciteten så att timerns kapacitet sätts till högsta tillåtna.
* Annars returneras angiven kapacitet.
************************************************************************/
static inline size_t check_capacity(const size_t capacity)
{
	if (capacity > MAX_CAPACITY) return MAX_CAPACITY;		// Om kapaciteten är över MAX_CAPACITY, så returneras MAX_CAPACITY.
	return capacity;						// Annars returneras angivet värde.
}

/************************************************************************
* DynamicTimer_on är en wrapper för funktionen Timer_on och används
* för att sätta på en timer. I detta fall sätts den dynamiska 
* timerkretsen på. 
************************************************************************/
void DynamicTimer_on(struct DynamicTimer* self)
{
	Timer_on(&self->timer); 
	return;
}

void DynamicTimer_off(struct DynamicTimer* self)		// Wrapper för att stänga av timerkretsen.
{
	Timer_off(&self->timer); 
	return;
}

void DynamicTimer_toggle(struct DynamicTimer* self)		// Wrapper för att toggla timerkretsen.
{
	Timer_toggle(&self->timer); 
}

/************************************************************************
* DynamicTimer_count används för att räkna antalet exekverade avbrott.
* Om timern är på så räknas antalet exekverade avbrott samt antalet
* passerade avbrott sedan föregående knapptryckning.
************************************************************************/ 
void DynamicTimer_count(struct DynamicTimer* self)
{
	if (self->timer.enabled)
	{
		self->timer.executed_interrupts++;
		self->interrupt_counter++;
	}
}

/************************************************************************
* Indikerar ifall timern har löpt ut. 
* Om timern inte är igång så returneras false, 
* annars kontrolleras ifall timern har löpt ut.
************************************************************************/
bool DynamicTimer_elapsed(struct DynamicTimer* self)
 {
	if (!self->timer.required_interrupts) return false; 
	return Timer_elapsed(&self->timer); 
 }
 
 /************************************************************************
 * DynamicTimer_clear används för att nollställa vektorn genom att
 * frigöra allokerat minne. 
 ************************************************************************/ 
void DynamicTimer_clear(struct DynamicTimer* self)
{
	Timer_reset(&self->timer);			// Nollställer timern.
	Vector_clear(&self->interrupt_vector);		// Nollställer vektorn.
	self->interrupt_counter = 0x00;			// Nollställer avbrottsräknaren.
	self->next = 0x00;				// Nästa element vid start nollställs.
	self->initiated = false;			// Dynamiska timern är ej initierad.
	return;
}

/************************************************************************
* DynamicTimer_update används för att uppdatera tiden på en dynamisk timer.
************************************************************************/
void DynamicTimer_update(struct DynamicTimer* self)
{
	
	if (!self->initiated)					// Om timern ej är startad, så nollställs samt startas den.
	{
		self->interrupt_counter = 0x00;			// Nollställer räknaren.
		self->initiated = true;				// Indikerar att timern är igång.
		serial_print("Dynamic timer initiated!\n");
		return;
	}
	
	if (self->interrupt_vector.elements < self->capacity)	// Om vektorn inte är full, lägg till det nya elementet längst bak (push).
	{
		Vector_push(&self->interrupt_vector, self->interrupt_counter);
	}
	else							// Annars skrivs det äldsta elementet över:	
	{
		Vector_set(&self->interrupt_vector, self->next, self->interrupt_counter);	
		self->interrupt_counter = 0x00;			// Nollställer inför nästa uppräkning.
	}
	
	
	if (++self->next > self->capacity) // Inkrementerar next, om dess värde är större än kapacieteten (Om nytt index hamnar utanför array), börja om från noll:
	{
		self->next = 0x00;	
	}
	
	
	// Beräknar genomsnittligt antal interrupt mellan knapptryckningar, avrundas till närmsta heltal:
	const uint32_t average_interrupts = ((uint32_t)Vector_average(&self->interrupt_vector) + 0.5);
	self->timer.required_interrupts = average_interrupts;
	
	serial_print("Dynamic timer updated!\n");
	DynamicTimer_print(self);				// Skriver ut all information.
	
	if (self->interrupt_vector.elements > 9)
		DynamicTimer_set_capacity(self, 10);
	return;
}

/************************************************************************
* _set_capacity används för att ställa in kapaciteten på en dynamisk timer.
* Det ingående argumentet new_capacity utgör angiven ny kapacitet, som
* kontrolleras via ett anrop av funktionen check_capacity. Dess returvärde 
* tilldelas till instansvariabeln capacity. Om den nya kapaciteten understiger
* storleken på den dynamiska timerns vektor, så placeras de nyaste värderna
* längst fram i vektorn följt av en omallokering till den nya storleken.
************************************************************************/
void DynamicTimer_set_capacity(struct DynamicTimer* self, const size_t new_capacity)
{
	
	if (!new_capacity) return;			// Om den nya kapaciteten är noll, gör inget (avsluta):
	
	else if (new_capacity >= self->capacity)	// Om den nya kapaciteten är större än aktuell kapacitet, uppdatera medlemmen capacity:
	{
		self->capacity = new_capacity;
		serial_print_unsigned("Vector capacity resized to %lu elements!\n", self->capacity);
		return;
	}
	// Annars om den nya kapaciteten är mindre än tidigare, flytta de nyaste elementen längst fram i vektorn och ändra sedan storleken:
	const size_t last = self->interrupt_vector.elements - 1; 	// Sista index i arrayen.
	
	if (self->next >= self->interrupt_vector.elements / 2)		// Om next pekar på index på senare halvan av arrayen, så flyttas elementet.
	{
		for (register size_t i = 0; i < self->interrupt_vector.elements / 2; i++)
		self->interrupt_vector.data[i] = self->interrupt_vector.data[last - i];
	}
	
	Vector_resize(&self->interrupt_vector, new_capacity);		// Ändrar vektorns storlek till den nya kapaciteten.
	
	self->capacity = new_capacity;					// Uppdaterar kapaciteten till den nya:
	serial_print_unsigned("Vector capacity resized to %lu elements!\n", self->capacity);
	
	return;
}

/************************************************************************
* DynamicTimer_print används för att skriva ut information om en dynamisk
* timer, bland annat aktuell fördröjningstid, antal lagrade element med 
* även summan och genomsnittet av dessa.
************************************************************************/
void DynamicTimer_print(const struct DynamicTimer* self)
{
	serial_print("----------------------------------------------------------------------------------------------------------\n");
	serial_print_unsigned("Capacity: %lu\n", self->capacity);						// skriver ut kapaciteten:
	serial_print_unsigned("Number of elements: %lu\n", self->interrupt_vector.elements);			// Skriver ut antalet element i vektor:
	serial_print_unsigned("Index of next element: %lu\n", self->next);					// Skriver ut index för nästa element:
	serial_print_unsigned("Sum of stored elements: %lu\n", Vector_sum(&self->interrupt_vector));		// Skriver ut summan av alla element:
	serial_print_unsigned("Average of stored elements: %lu\n", (uint32_t)(Vector_average(&self->interrupt_vector) + 0.5)); // Skriver ut genomsnittet av alla element. Avrundar till närmsta heltal:
	serial_print_unsigned("Delay time: %lu ms\n", self->timer.required_interrupts * INTERRUPT_TIME);	// Skriver ut fördröjningstiden:
	serial_print("---------------------------------------------------------------------------------------------------------\n\n");
	return;
}
