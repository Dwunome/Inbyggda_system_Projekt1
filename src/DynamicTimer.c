#include "DynamicTimer.h"

static inline size_t check_capacity(const size_t capacity);

/*
* Funktionen anv�nds f�r att implementera en ny dynamisk timer.
* Ing�ende argument timerSelection utg�rs av vald timerkrets och
* capacity utg�r maxstorleken p� arrayen som lagrar antalet exekverade 
* avbrott mellan varje knapptryckning.
*/
struct DynamicTimer new_DynamicTimer(const TimerSelection timerSelection, const size_t capacity)
{
	struct DynamicTimer self;
	self.timer = new_Timer(timerSelection, 0x00); // Initierar timern, av vid start.
	self.interrupt_vector = new_Vector(); // Initierar tom dynamisk array.
	self.interrupt_counter = 0x00; // R�knaren startar p� noll.
	self.capacity = check_capacity(capacity); // sparar kontrollerad kapacitet
	self.next = 0x00; // N�sta element �r f�rsta element vid start, s� index noll.
	self.initiated = false; // Timern har ej startat f�rr�n vi trycker p� knappen.
	return self; // Returnerar det f�rdiga objektet.
}

/*
* check_capacity anv�nds f�r att kontrollera angiven kapacitet
* g�llande antalet element som ska lagras av en dynamisk tiemr, s�
* att denna kapacitet inte �verstiger maximalt till�tna storlek.
* Det ing�ende argumentet capacity utg�rs av angiven kapacitet. 
* Om angiven kapacitet �verstiger h�gsta till�tna storlek s� returneras
* maxkapaciteten s� att timerns kapacitet s�tts till h�gsta till�tna.
* Annars returneras angiven kapacitet.
*/
static inline size_t check_capacity(const size_t capacity)
{
	// Om kapaciteten �r �ver MAX_CAPACITY, s� returneras MAX_CAPACITY:
	if (capacity > MAX_CAPACITY) return MAX_CAPACITY; 
	return capacity; // Annars returneras angivet v�rde
}

/*
* DynamicTimer_on �r en wrapper f�r funktionen Timer_on och anv�nds
* f�r att s�tta p� en timer. I detta fall s�tts den dynamiska 
* timerkretsen p�. 
*/
void DynamicTimer_on(struct DynamicTimer* self)
{
	Timer_on(&self->timer); 
	return;
}
// Wrapper f�r att st�nga av timerkretsen:
void DynamicTimer_off(struct DynamicTimer* self)
{
	Timer_off(&self->timer); 
	return;
}
// Wrapper f�r att toggla timerkretsen:
void DynamicTimer_toggle(struct DynamicTimer* self)
{
	Timer_toggle(&self->timer); 
}

/*
* DynamicTimer_count anv�nds f�r att r�kna antalet exekverade avbrott.
* Om timern �r p� s� r�knas antalet exekverade avbrott samt antalet
* passerade avbrott sedan f�reg�ende knapptryckning.
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
* Indikerar ifall timern har l�pt ut. 
* Om timern inte �r ig�ng s� returneras false, 
* annars kontrolleras ifall timern har l�pt ut.
*/
bool DynamicTimer_elapsed(struct DynamicTimer* self)
 {
	if (!self->timer.required_interrupts) return false; 
	return Timer_elapsed(&self->timer); 
 }
 
 /*
 * _clear anv�nds f�r att nollst�lla vektorn genom att frig�ra 
 * allokerat minne. 
 */ 
void DynamicTimer_clear(struct DynamicTimer* self)
{
	Timer_reset(&self->timer); // Nollst�ller timern
	Vector_clear(&self->interrupt_vector); // Nollst�ller vektorn
	self->interrupt_counter = 0x00; // Nollst�ller avbrottsr�knaren
	self->next = 0x00; // n�sta element vid start blir d� noll
	self->initiated = false; // ej initierad
	return;
}

/*
* _update anv�nds f�r att uppdatera tiden p� en dynamisk timer.
*/
void DynamicTimer_update(struct DynamicTimer* self)
{
	// Om timern ej �r startad �n s� startar den och blir nollst�lld:
	if (!self->initiated)
	{
		self->interrupt_counter = 0x00; // R�kningen b�rjar fr�n och med nu.
		self->initiated = true; // Indikerar att timern nu �r ig�ng p� riktigt.
		serial_print("Dynamic timer initiated!\n");
		return;
	}
	// Om vektorn inte �r full, l�gg till det nya elementet l�ngst bak (push):
	if (self->interrupt_vector.elements < self->capacity)
		Vector_push(&self->interrupt_vector, self->interrupt_counter);
	else // Annars skrivs det �ldsta elementet �ver:	
		 Vector_set(&self->interrupt_vector, self->next, self->interrupt_counter);	
	self->interrupt_counter = 0x00; // Nollst�ller inf�r n�sta uppr�kning.
	
	// Inkrementera next, kolla dess v�rde. Om nytt index hamnar utanf�r array, b�rja om fr�n noll:
	if (++self->next > self->capacity) self->next = 0x00;
	
	// Ber�knar genomsnittligt antal interrupt mellan knapptryckningar, avrundas till n�rmsta heltal:
	const uint32_t average_interrupts = ((uint32_t)Vector_average(&self->interrupt_vector) + 0.5);
	self->timer.required_interrupts = average_interrupts;
	
	serial_print("Dynamic timer updated!\n");
	DynamicTimer_print(self); // Skriver ut all information.
	
	if (self->interrupt_vector.elements > 9)
		DynamicTimer_set_capacity(self, 10);
	return;
}

/*
* _set_capacity anv�nds f�r att st�lla in kapaciteten p� en dynamisk timer.
* Det ing�ende argumentet new_capacity utg�r angiven ny kapacitet, som
* kontrolleras via ett anrop av funktionen check_capacity. Dess returv�rde 
* tilldelas till instansvariabeln capacity. Om den nya kapaciteten understiger
* storleken p� den dynamiska timerns vektor, s� placeras de nyaste v�rderna l�ngst
* fram i vektorn f�ljt av en omallokering till den nya storleken.
*/
void DynamicTimer_set_capacity(struct DynamicTimer* self, const size_t new_capacity)
{
	// Om den nya kapaciteten �r noll, g�r inget (avsluta):
	if (!new_capacity) return; 
	// Om den nya kapaciteten �r st�rre �n aktuell kapacitet, uppdatera medlemmen capacity:
	else if (new_capacity >= self->capacity)
	{
		self->capacity = new_capacity;
		serial_print_unsigned("Vector capacity resized to %lu elements!\n", self->capacity);
		return;
	}
	// Annars om den nya kapaciteten �r mindre �n tidigare, flytta de nyaste elementen l�ngst fram i vecktorn och �ndra sedan storleken:
	const size_t last = self->interrupt_vector.elements - 1; // Sista index i arrayen.
	// Om next pekar p� index p� senare halvan av arrayen, s� flyttas elementet:
	if (self->next >= self->interrupt_vector.elements / 2)
	{
		for (register size_t i = 0; i < self->interrupt_vector.elements / 2; i++)
		self->interrupt_vector.data[i] = self->interrupt_vector.data[last - i];
	}
	// �ndrar vektorns storlek till den nya kapaciteten:
	Vector_resize(&self->interrupt_vector, new_capacity);
	// Uppdaterar kapaciteten till den nya:
	self->capacity = new_capacity;
	serial_print_unsigned("Vector capacity resized to %lu elements!\n", self->capacity);
	
	return;
}

/*
* _print anv�nds f�r att skriva ut information om en dynamisk timer,
* bland annat aktuell f�rdr�jningstid, antal lagrade element med �ven 
* summan och genomsnittet av dessa.
*/
void DynamicTimer_print(const struct DynamicTimer* self)
{
	serial_print("----------------------------------------------------------------------------------------------------------\n");
	// skriver ut kapaciteten:
	serial_print_unsigned("Capacity: %lu\n", self->capacity); 
	// Skriver ut antalet element i vektor:
	serial_print_unsigned("Number of elements: %lu\n", self->interrupt_vector.elements);
	// Skriver ut index f�r n�sta element:
	serial_print_unsigned("Index of next element: %lu\n", self->next); 
	// Skriver ut summan av alla element:
	serial_print_unsigned("Sum of stored elements: %lu\n", Vector_sum(&self->interrupt_vector)); 
	// Skriver ut genomsnittet av alla element. Avrundar till n�rmsta heltal:
	serial_print_unsigned("Average of stored elements: %lu\n", (uint32_t)(Vector_average(&self->interrupt_vector) + 0.5)); 
	// Skriver ut f�rdr�jningstiden:
	serial_print_unsigned("Delay time: %lu ms\n", self->timer.required_interrupts * INTERRUPT_TIME);
	serial_print("---------------------------------------------------------------------------------------------------------\n\n");
	return;
}