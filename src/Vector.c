#include "Vector.h"

/******************************************************************************
* Funktionen new_Vector används för att initiera en ny dynamisk array. Först
* deklareras ett objekt av strukten Vector, döpt self. Sedan sätt pekaren data
* till en nullpekare, detta då den ej har någon befintlig minnesadress som den 
* kan peka på vid start. Instansvariabeln elements sätts till 0 vid start då 
* arrayen är tom vid start. 
* Slutligen returneras objektet self. 
******************************************************************************/

struct Vector new_Vector(void)
{
	struct Vector self; 
	self.data = NULL;
	self.elements = 0x00;
	return self;
}

/******************************************************************************
* Funtionen används för att kunna uppdatera den dynamiska vektorns storlek.
* Om den nya storleken är noll, frigör allt minne och avsluta funktionen, 
* via anrop av funktionen Vector_clear. Annars omallokeras fältet data
* med en kopia efter den nya storleken. 
* Sedan sätts ordinarie pekare data till att peka på det nya fältet och 
* därefter uppdateras antalet element.
******************************************************************************/

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

/******************************************************************************
* Först omallokeras fältet så att det rymmer ett tal till, använder 
* en kopia. Sedan läggs det nya elementet längst bak i arrayen och 
* därefter sätts datapekaren till att peka på det nya fältet.
******************************************************************************/

void Vector_push(struct Vector* self, const uint32_t new_element)
{
	uint32_t* copy = (uint32_t*)realloc(self->data, sizeof(uint32_t) * (self->elements + 1)); 
	if (!copy) return; 
	copy[self->elements++] = new_element; 
	self->data = copy; 
	return;
}

/******************************************************************************
* Funktionen används för att kunna tömma en dynamisk array och frigöra
* dess minne. Först sker en deallokering via anrop av funktionen free, 
* där datapekaren som pekar på det allokerarade minnet passerar.
******************************************************************************/

void Vector_clear(struct Vector* self)
{
	free(self->data);
	self->data = NULL;
	self->elements = 0x00;
	return;
}

/******************************************************************************
* Funktionen Vector_set används för att skriva över ett gammalt element.
* Ingående argument index utgör index till det element som skall skrivas över,
* medan new_element utgör det nya värdet. Om angivet index ligger utanför
* arrayen så görs ingen skrivning. Annars läggs det nya elementet på angivet
* index.
******************************************************************************/

void Vector_set(struct Vector* self, const size_t index, const uint32_t new_element)
{
	if (index >= self->elements) return;
	self->data[index] = new_element;
	return;
}

/******************************************************************************
* Funktionen används för att kunna beräkna medelvärdet av alla lagrade tal.
* Om arrayen är tom så returneras noll som medelvärde, annars beräknas 
* summan av alla tal via anrop av funktionen Vector_sum. Slutligen så
* returneras summan. 
******************************************************************************/

double Vector_average(const struct Vector* self)
{
	if (!self->elements) return 0;
	const uint32_t sum = Vector_sum(self);
	return sum / (double)(self->elements);
}

/******************************************************************************
* Funktionen används för att skriva ut innehållet i arrayen.
* Om arrayen är tom så avslutas funktionen.
* Annars initieras seriell överföring med anrop av funktionen
* init_serial. Sedan skrivs antal och summa av element ut, samt det 
* avrundade värdet. 
******************************************************************************/

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

/******************************************************************************
* Funktionen används för att kunna beräkna summa av alla befintliga 
* element i arrayen. Om arrayen är tom så returneras 0. Annars sätts
* räknaren för summa till värdet 0 vid start, därefter körs en for-sats
* som räknar upp till self->elements. Sedan returneras summan.
******************************************************************************/

uint32_t Vector_sum(const struct Vector* self)
{
	
	if (!self->elements) return 0;
	uint32_t sum = 0x00;
	for (register size_t i = 0; i < self->elements; i++)
		sum += self->data[i]; 
	return sum; 
}
