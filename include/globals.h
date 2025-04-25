#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

#define WIN_SIZE 256

extern volatile float window1[WIN_SIZE];
extern volatile float window2[WIN_SIZE];
extern volatile bool window_1_ready;
extern volatile bool window_2_ready;
extern volatile bool window_1_processing;
extern volatile bool window_2_processing;
extern volatile bool window_1_waiting;
extern volatile bool window_2_waiting;
extern volatile bool isSignalProcessing;

#endif /*GLOBALS_H*/