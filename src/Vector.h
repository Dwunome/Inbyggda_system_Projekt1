
#ifndef VECTOR_H_
#define VECTOR_H_

#include "definitions.h"
#include "Serial.h"

/*
Strukten Vector används för att implementera dynamiska arrayer för lagring av
osignerade heltal. Pekaren data pekar på ett dynamiskt fält som lagrar samtliga
tal, medan elements indikerar antalet tal i arrayen. 
*/
struct Vector
{
	uint32_t* data;		// Pekare till dynamsikt fält som lagrar osignerade heltal
	size_t elements;	// Räknar antalet element (osignerade heltal) i arrayen.
};

// Externa funktioner:
struct Vector new_Vector(void); // Initieringsrutin för dynamiska arrayer, returnerar färdig vektor.
void Vector_resize(struct Vector* self, const size_t new_size);
void Vector_push(struct Vector* self, const uint32_t new_element); // Lägger till ett nytt element längst bak i arrayen.
void Vector_clear(struct Vector* self); // Tömmer arrayen och frigör minne.
void Vector_set(struct Vector* self, const size_t index, const uint32_t new_element);
uint32_t Vector_sum(const struct Vector* self); // Beräknar summan av alla befintliga element.
double Vector_average(const struct Vector* self); // Beräknar genomsnitt av lagrade värden.
void Vector_print(const struct Vector* self); // Skriver ut vektorns innehåll.
#endif /* VECTOR_H_ */