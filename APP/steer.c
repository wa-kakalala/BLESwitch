#include "steer.h"
sbit steer = P3^3;
// sfr 定义特殊功能寄存器
sfr  P3M1 = 0xb1;
sfr  P3M0 = 0xb2;


void Steer_Init(){
	// 设置引脚模式为强推挽模式
   	P3M1 = P3M1 & 0xf7;
	P3M0 = P3M0 | 0x08;
}

void Steer_On(){
 	steer = 1;
}

void Steer_Off(){
 	steer = 0;
}