
#ifndef VECTOR_H_
#define VECTOR_H_

#include "definitions.h"
#include "Serial.h"

/*
Strukten Vector anv�nds f�r att implementera dynamiska arrayer f�r lagring av
osignerade heltal. Pekaren data pekar p� ett dynamiskt f�lt som lagrar samtliga
tal, medan elements indikerar antalet tal i arrayen. 
*/
struct Vector
{
	uint32_t* data;		// Pekare till dynamsikt f�lt som lagrar osignerade heltal
	size_t elements;	// R�knar antalet element (osignerade heltal) i arrayen.
};

// Externa funktioner:
struct Vector new_Vector(void); // Initieringsrutin f�r dynamiska arrayer, returnerar f�rdig vektor.
void Vector_resize(struct Vector* self, const size_t new_size);
void Vector_push(struct Vector* self, const uint32_t new_element); // L�gger till ett nytt element l�ngst bak i arrayen.
void Vector_clear(struct Vector* self); // T�mmer arrayen och frig�r minne.
void Vector_set(struct Vector* self, const size_t index, const uint32_t new_element);
uint32_t Vector_sum(const struct Vector* self); // Ber�knar summan av alla befintliga element.
double Vector_average(const struct Vector* self); // Ber�knar genomsnitt av lagrade v�rden.
void Vector_print(const struct Vector* self); // Skriver ut vektorns inneh�ll.
#endif /* VECTOR_H_ */