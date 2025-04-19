#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

#define WIN_SIZE 256

extern volatile float window[WIN_SIZE];
extern volatile bool windowReady;

#endif /*GLOBALS_H*/