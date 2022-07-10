#ifndef DYNAMICTIMER_H_
#define DYNAMICTIMER_H_

#include "definitions.h"
#include "Timer.h"
#include "Vector.h"
#include "Serial.h"

#define MAX_CAPACITY 256 // Max antal element lagrad i dynamisk array.

/*
* Strukten DnamicTimer används för att implementera en dynamsisk timer
* där antalet timergenerade avbrott räknas och implementeras för att 
* skapa en genomsnittlig tid på timern.
*/
struct DynamicTimer
{
	struct Timer timer; // Timerkrets, implementerar timerfunktionalitet.
	struct Vector interrupt_vector; // Vektor, lagrar antalet interrupts mellan varje knapptryckning.
	uint32_t interrupt_counter; // Räknar anatalet timergenererade avbrott mellan knapptryckningar.
	size_t capacity; // Vektorns kapacitet.
	size_t next; // Index för nästa element som skall läggas till. 
	bool initiated; // indikerar ifall timer har blivit startad (Sker efeter första knapptryckningen).
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