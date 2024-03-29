#include <stdint.h>

#ifndef KERNEL_KEYBOARD_H
#define KERNEL_KEYBOARD_H

#define CANT_KEYS 128
#define RELEASE 0x80
#define ENTER 28
#define LSHIFT 0x2A
#define RSHIFT 0x36

#define ESCAPE 1
#define ONE 2
#define TWO 3
#define CONTROL 29
#define BACKSPACE 14


void ncClear();
void keyboard_handler();
//devuelve buffer terminado en \n --> lo vamos a cambiar a string
char * getBuffer(int * writerVal);
char getCharKernel();
int getCtrlFlag();
void cleanKeyboardBuffer();

#endif //KERNEL_KEYBOARD_H
