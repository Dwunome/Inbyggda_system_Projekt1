#include "DynamicTimer.h"

static inline size_t check_capacity(const size_t capacity);

/*
* Funktionen används för att implementera en ny dynamisk timer.
* Ingående argument timerSelection utgörs av vald timerkrets och
* capacity utgör maxstorleken på arrayen som lagrar antalet exekverade 
* avbrott mellan varje knapptryckning.
*/
struct DynamicTimer new_DynamicTimer(const TimerSelection timerSelection, const size_t capacity)
{
	struct DynamicTimer self;
	self.timer = new_Timer(timerSelection, 0x00); // Initierar timern, av vid start.
	self.interrupt_vector = new_Vector(); // Initierar tom dynamisk array.
	self.interrupt_counter = 0x00; // Räknaren startar på noll.
	self.capacity = check_capacity(capacity); // sparar kontrollerad kapacitet
	self.next = 0x00; // Nästa element är första element vid start, så index noll.
	self.initiated = false; // Timern har ej startat förrän vi trycker på knappen.
	return self; // Returnerar det färdiga objektet.
}

/*
* check_capacity används för att kontrollera angiven kapacitet
* gällande antalet element som ska lagras av en dynamisk tiemr, så
* att denna kapacitet inte överstiger maximalt tillåtna storlek.
* Det ingående argumentet capacity utgörs av angiven kapacitet. 
* Om angiven kapacitet överstiger högsta tillåtna storlek så returneras
* maxkapaciteten så att timerns kapacitet sätts till högsta tillåtna.
* Annars returneras angiven kapacitet.
*/
static inline size_t check_capacity(const size_t capacity)
{
	// Om kapaciteten är över MAX_CAPACITY, så returneras MAX_CAPACITY:
	if (capacity > MAX_CAPACITY) return MAX_CAPACITY; 
	return capacity; // Annars returneras angivet värde
}

/*
* DynamicTimer_on är en wrapper för funktionen Timer_on och används
* för att sätta på en timer. I detta fall sätts den dynamiska 
* timerkretsen på. 
*/
void DynamicTimer_on(struct DynamicTimer* self)
{
	Timer_on(&self->timer); 
	return;
}
// Wrapper för att stänga av timerkretsen:
void DynamicTimer_off(struct DynamicTimer* self)
{
	Timer_off(&self->timer); 
	return;
}
// Wrapper för att toggla timerkretsen:
void DynamicTimer_toggle(struct DynamicTimer* self)
{
	Timer_toggle(&self->timer); 
}

/*
* DynamicTimer_count används för att räkna antalet exekverade avbrott.
* Om timern är på så räknas antalet exekverade avbrott samt antalet
* passerade avbrott sedan föregående knapptryckning.
*/ 
void DynamicTimer_count(struct DynamicTimer* self)
{
	if (self->timer.enabled)
	{
		self->timer.executed_interrupts++;
		self->interrupt_counter++;
	}
}

/*
* Indikerar ifall timern har löpt ut. 
* Om timern inte är igång så returneras false, 
* annars kontrolleras ifall timern har löpt ut.
*/
bool DynamicTimer_elapsed(struct DynamicTimer* self)
 {
	if (!self->timer.required_interrupts) return false; 
	return Timer_elapsed(&self->timer); 
 }
 
 /*
 * _clear används för att nollställa vektorn genom att frigöra 
 * allokerat minne. 
 */ 
void DynamicTimer_clear(struct DynamicTimer* self)
{
	Timer_reset(&self->timer); // Nollställer timern
	Vector_clear(&self->interrupt_vector); // Nollställer vektorn
	self->interrupt_counter = 0x00; // Nollställer avbrottsräknaren
	self->next = 0x00; // nästa element vid start blir då noll
	self->initiated = false; // ej initierad
	return;
}

/*
* _update används för att uppdatera tiden på en dynamisk timer.
*/
void DynamicTimer_update(struct DynamicTimer* self)
{
	// Om timern ej är startad än så startar den och blir nollställd:
	if (!self->initiated)
	{
		self->interrupt_counter = 0x00; // Räkningen börjar från och med nu.
		self->initiated = true; // Indikerar att timern nu är igång på riktigt.
		serial_print("Dynamic timer initiated!\n");
		return;
	}
	// Om vektorn inte är full, lägg till det nya elementet längst bak (push):
	if (self->interrupt_vector.elements < self->capacity)
		Vector_push(&self->interrupt_vector, self->interrupt_counter);
	else // Annars skrivs det äldsta elementet över:	
		 Vector_set(&self->interrupt_vector, self->next, self->interrupt_counter);	
	self->interrupt_counter = 0x00; // Nollställer inför nästa uppräkning.
	
	// Inkrementera next, kolla dess värde. Om nytt index hamnar utanför array, börja om från noll:
	if (++self->next > self->capacity) self->next = 0x00;
	
	// Beräknar genomsnittligt antal interrupt mellan knapptryckningar, avrundas till närmsta heltal:
	const uint32_t average_interrupts = ((uint32_t)Vector_average(&self->interrupt_vector) + 0.5);
	self->timer.required_interrupts = average_interrupts;
	
	serial_print("Dynamic timer updated!\n");
	DynamicTimer_print(self); // Skriver ut all information.
	
	if (self->interrupt_vector.elements > 9)
		DynamicTimer_set_capacity(self, 10);
	return;
}

/*
* _set_capacity används för att ställa in kapaciteten på en dynamisk timer.
* Det ingående argumentet new_capacity utgör angiven ny kapacitet, som
* kontrolleras via ett anrop av funktionen check_capacity. Dess returvärde 
* tilldelas till instansvariabeln capacity. Om den nya kapaciteten understiger
* storleken på den dynamiska timerns vektor, så placeras de nyaste värderna längst
* fram i vektorn följt av en omallokering till den nya storleken.
*/
void DynamicTimer_set_capacity(struct DynamicTimer* self, const size_t new_capacity)
{
	// Om den nya kapaciteten är noll, gör inget (avsluta):
	if (!new_capacity) return; 
	// Om den nya kapaciteten är större än aktuell kapacitet, uppdatera medlemmen capacity:
	else if (new_capacity >= self->capacity)
	{
		self->capacity = new_capacity;
		serial_print_unsigned("Vector capacity resized to %lu elements!\n", self->capacity);
		return;
	}
	// Annars om den nya kapaciteten är mindre än tidigare, flytta de nyaste elementen längst fram i vecktorn och ändra sedan storleken:
	const size_t last = self->interrupt_vector.elements - 1; // Sista index i arrayen.
	// Om next pekar på index på senare halvan av arrayen, så flyttas elementet:
	if (self->next >= self->interrupt_vector.elements / 2)
	{
		for (register size_t i = 0; i < self->interrupt_vector.elements / 2; i++)
		self->interrupt_vector.data[i] = self->interrupt_vector.data[last - i];
	}
	// Ändrar vektorns storlek till den nya kapaciteten:
	Vector_resize(&self->interrupt_vector, new_capacity);
	// Uppdaterar kapaciteten till den nya:
	self->capacity = new_capacity;
	serial_print_unsigned("Vector capacity resized to %lu elements!\n", self->capacity);
	
	return;
}

/*
* _print används för att skriva ut information om en dynamisk timer,
* bland annat aktuell fördröjningstid, antal lagrade element med även 
* summan och genomsnittet av dessa.
*/
void DynamicTimer_print(const struct DynamicTimer* self)
{
	serial_print("----------------------------------------------------------------------------------------------------------\n");
	// skriver ut kapaciteten:
	serial_print_unsigned("Capacity: %lu\n", self->capacity); 
	// Skriver ut antalet element i vektor:
	serial_print_unsigned("Number of elements: %lu\n", self->interrupt_vector.elements);
	// Skriver ut index för nästa element:
	serial_print_unsigned("Index of next element: %lu\n", self->next); 
	// Skriver ut summan av alla element:
	serial_print_unsigned("Sum of stored elements: %lu\n", Vector_sum(&self->interrupt_vector)); 
	// Skriver ut genomsnittet av alla element. Avrundar till närmsta heltal:
	serial_print_unsigned("Average of stored elements: %lu\n", (uint32_t)(Vector_average(&self->interrupt_vector) + 0.5)); 
	// Skriver ut fördröjningstiden:
	serial_print_unsigned("Delay time: %lu ms\n", self->timer.required_interrupts * INTERRUPT_TIME);
	serial_print("---------------------------------------------------------------------------------------------------------\n\n");
	return;
}