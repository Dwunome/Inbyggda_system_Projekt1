#ifndef DYNAMICTIMER_H_
#define DYNAMICTIMER_H_

#include "definitions.h"
#include "Timer.h"
#include "Vector.h"
#include "Serial.h"

#define MAX_CAPACITY 256 // Max antal element lagrad i dynamisk array.

/*
* Strukten DnamicTimer anv�nds f�r att implementera en dynamsisk timer
* d�r antalet timergenerade avbrott r�knas och implementeras f�r att 
* skapa en genomsnittlig tid p� timern.
*/
struct DynamicTimer
{
	struct Timer timer; // Timerkrets, implementerar timerfunktionalitet.
	struct Vector interrupt_vector; // Vektor, lagrar antalet interrupts mellan varje knapptryckning.
	uint32_t interrupt_counter; // R�knar anatalet timergenererade avbrott mellan knapptryckningar.
	size_t capacity; // Vektorns kapacitet.
	size_t next; // Index f�r n�sta element som skall l�ggas till. 
	bool initiated; // indikerar ifall timer har blivit startad (Sker efeter f�rsta knapptryckningen).
}; // Struktens namn.	

// Externa funktioner:
struct DynamicTimer new_DynamicTimer(const TimerSelection timerSelection, const size_t capacity);
void DynamicTimer_on(struct DynamicTimer* self);
void DynamicTimer_off(struct DynamicTimer* self);
void DynamicTimer_toggle(struct DynamicTimer* self);
void DynamicTimer_count(struct DynamicTimer* self);
bool DynamicTimer_elapsed(struct DynamicTimer* self); 
void DynamicTimer_clear(struct DynamicTimer* self);
void DynamicTimer_update(struct DynamicTimer* self);
void DynamicTimer_set_capacity(struct DynamicTimer* self, const size_t new_capacity);
void DynamicTimer_print(const struct DynamicTimer* self);

#endif /* DYNAMICTIMER_H_ */