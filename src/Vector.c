#include "Vector.h"

/*
* Funktionen new_Vector anv�nds f�r att initiera en ny dynamisk array. F�rst
* deklareras ett objekt av strukten Vector, d�pt self. Sedan s�tt pekaren data
* till en nullpekare, detta d� den ej har n�gon befintlig minnesadress som den 
* kan peka p� vid start. Instansvariabeln elements s�tts till 0 vid start d� 
* arrayen �r tom vid start. 
* Slutligen returneras objektet self. 
*/
struct Vector new_Vector(void)
{
	struct Vector self; 
	self.data = NULL;
	self.elements = 0x00;
	return self;
}

/*
* Funtionen anv�nds f�r att kunna uppdatera den dynamiska vektorns storlek.
* Om den nya storleken �r noll, frig�r allt minne och avsluta funktionen, 
* via anrop av funktionen Vector_clear. Annars omallokeras f�ltet data
* med en kopia efter den nya storleken. 
* Sedan s�tts ordinarie pekare data till att peka p� det nya f�ltet och 
* d�refter uppdateras antalet element.
*/
void Vector_resize(struct Vector* self, const size_t new_size)
{
	if (!new_size)
	{
		Vector_clear(self);
		return;
	}
	
	uint32_t* copy =  (uint32_t*)realloc(self->data, sizeof(uint32_t) * new_size); 
	if (!copy) return;
	self->data = copy;
	self->elements = new_size;
	return;
}

/*
* F�rst omallokeras f�ltet s� att det rymmer ett tal till, �nv�nder 
* en kopia. Sedan l�ggs det nya elementet l�ngst bak i arrayen och 
* d�refter s�tts datapekaren till att peka p� det nya f�ltet.
*/
void Vector_push(struct Vector* self, const uint32_t new_element)
{
	uint32_t* copy = (uint32_t*)realloc(self->data, sizeof(uint32_t) * (self->elements + 1)); 
	if (!copy) return; 
	copy[self->elements++] = new_element; 
	self->data = copy; 
	return;
}

/*
* Funktionen anv�nds f�r att kunna t�mma en dynamisk array och frig�ra
* dess minne. F�rst sker en deallokering via anrop av funktionen free, 
* d�r datapekaren som pekar p� det allokerarade minnet passerar.
*/
void Vector_clear(struct Vector* self)
{
	free(self->data);
	self->data = NULL;
	self->elements = 0x00;
	return;
}

/*
* Funktionen Vector_set anv�nds f�r att skriva �ver ett gammalt element.
* Ing�ende argument index utg�r index till det element som skall skrivas �ver,
* medan new_element utg�r det nya v�rdet. Om angivet index ligger utanf�r
* arrayen s� g�rs ingen skrivning. Annars l�ggs det nya elementet p� angivet
* index.
*/
void Vector_set(struct Vector* self, const size_t index, const uint32_t new_element)
{
	if (index >= self->elements) return;
	self->data[index] = new_element;
	return;
}

/*
* Funktionen anv�nds f�r att kunna ber�kna medelv�rdet av alla lagrade tal.
* Om arrayen �r tom s� returneras noll som medelv�rde, annars ber�knas 
* summan av alla tal via anrop av funktionen Vector_sum. Slutligen s�
* returneras summan. 
*/
double Vector_average(const struct Vector* self)
{
	if (!self->elements) return 0;
	const uint32_t sum = Vector_sum(self);
	return sum / (double)(self->elements);
}

/*
* Funktionen anv�nds f�r att skriva ut inneh�llet i arrayen.
* Om arrayen �r tom s� avslutas funktionen.
* Annars initieras seriell �verf�ring med anrop av funktionen
* init_serial. Sedan skrivs antal och summa av element ut, samt det 
* avrundade v�rdet. 
*/
void Vector_print(const struct Vector* self)
{
	if (!self->elements) return;
	init_serial();
	
	serial_print("------------------------------------------------\n");
	serial_print_unsigned("Number of elements: %lu\n", self->elements); 
	serial_print_unsigned("Sum of all elements: %lu\n", Vector_sum(self));
	serial_print_integer("Rounded average: %ld\n", (uint32_t)(Vector_average(self) + 0.5));
	for (register size_t i = 0; i < self->elements; i++)
		serial_print_unsigned("%lu\n", self->data[i]);
	serial_print("------------------------------------------------\n\n");	
	return;
}

/*
* Funktionen anv�nds f�r att kunna ber�kna summa av alla befintliga 
* element i arrayen. Om arrayen �r tom s� returneras 0. Annars s�tts
* r�knaren f�r summa till v�rdet 0 vid start, d�refter k�rs en for-sats
* som r�knar upp till self->elements. Sedan returneras summan.
*/
uint32_t Vector_sum(const struct Vector* self)
{
	
	if (!self->elements) return 0;
	uint32_t sum = 0x00;
	for (register size_t i = 0; i < self->elements; i++)
		sum += self->data[i]; 
	return sum; 
}