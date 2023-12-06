#include <reg51.h>
#include "uart.h"
#include "led.h"
#include "ble.h"

void Sys_Init(){
 	Led_Init();
	Uart_Init();
	EA = 1;
}

void main(){
	BLE_CMD ble_cmd;
	unsigned int led_count = 0;
	u8 led_state = 0;
	Sys_Init();
	
	while(1) {
		ble_cmd = Ble_Decode();
		switch( ble_cmd ) {
			case BLE_OPEN_DOOR:
				break;
	
		    default: break;
	    }

		// monitor system's running
		led_count ++ ;
		if( led_count == 50000 ) {
	 		if( led_state == 0 ){
		 		Led_On();
				led_state = 1;
			}else{
		 		Led_Off();
				led_state = 0;
			}	
			led_count = 0;
		}
	}
}

