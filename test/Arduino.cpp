#include "Arduino.h"

unsigned long _millis = 0;
 
unsigned long millis() { return _millis; }

void delay( unsigned long ms ) { _millis += ms; }

void yield() { delay(1); };
