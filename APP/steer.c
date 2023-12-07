#include "steer.h"
#define STEER_N 10

sbit steer = P3^3;
// sfr 定义特殊功能寄存器
sfr  P3M1 = 0xb1;
sfr  P3M0 = 0xb2;

typedef unsigned char u8;

/*
  sysclk :  5.5296 Mhz
  0.1ms --> 553
  65536 - 553 = 64983 = 0xfdd7
*/

// 1T mode or /12
//   7     6   5  4    3     2   1 0
// t0x12 t1x12 - T2R T2_C/T T2x12 - -
sfr auxr = 0x8e;

void Timer0_config(){
    // 1T mode
    auxr = auxr | 0x80;
	// 16 bit reload
	TMOD = 	TMOD & 0xf0;
	TH0 = 0xfd;
	TL0 = 0xd7;
    // enable timer0 interrupt
    ET0 = 1;
	TR0 = 0;
    // need to enable EA
}

void Steer_Init(){
	// set pin to push pull mode
   	P3M1 = P3M1 & 0xf7;
	P3M0 = P3M0 | 0x08;
	Timer0_config();
}

int timer_count = 0;
void Timer0_Interrupt() interrupt 1 {
	timer_count ++;
}

void delay_100us(u8 n100us){
   timer_count = 0;
   TH0 = 0xfd;
   TL0 = 0xd7;
   TR0 = 1;
   while(!( timer_count == n100us ));
   TR0 = 0; 
}

void Open_Door(){
	int i;
 	for(  i=0;i<STEER_N;i++){
	   steer = 1;
	   delay_100us(5);
	   steer = 0;
	   delay_100us(200-12);
	}
	// delay 3s
	delay_100us(30000);

	for( i=0;i<STEER_N;i++){
	   steer = 1;
	   delay_100us(18);
	   steer = 0;
	   delay_100us(200-18);
	}
}
