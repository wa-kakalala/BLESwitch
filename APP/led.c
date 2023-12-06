#include "led.h"

sbit led = P3^2;
// sfr : define specifical function register
sfr  P3M1 = 0xb1;
sfr  P3M0 = 0xb2;


void Led_Init(){
	// set pin to push pull mode
   	P3M1 = P3M1 & 0xfb;
	P3M0 = P3M0 | 0x04;
}

void Led_On(){
 	led = 1;
}

void Led_Off(){
 	led = 0;
}