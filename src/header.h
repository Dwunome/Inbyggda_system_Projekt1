#ifndef HEADER_H_
#define HEADER_H_

// Inkluderingsdirektiv:
#include "definitions.h"
#include "GPIO.h" 
#include "Timer.h"
#include "Serial.h"
#include "ADC.h"
#include "Vector.h"
#include "DynamicTimer.h"

// Globala variabler:
struct Led led1; 
struct Button button; 
struct Timer timer0; 
struct TempSensor tempSensor;
struct DynamicTimer timer1;

// Funktionsdeklarationer:
void setup(void);


#endif /* HEADER_H_ */