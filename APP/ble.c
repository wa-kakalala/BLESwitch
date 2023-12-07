#include "ble.h"

// decode data from ble
BLE_CMD Ble_Decode(){
 	if( rx_enable == 0 ) {
		BLE_DATA_STRUCT	* ble_structor =  (BLE_DATA_STRUCT	*)uart_rx_buf;
		if( ble_structor-> src_addr != wifiswitch_addr ){rx_enable = 1; return BLE_NOP;}
		if( ble_structor-> dst_addr != doorswitch_addr ){rx_enable = 1; return BLE_NOP;}
		if( ble_structor-> cmd_t != doorcontrol_type ) {rx_enable = 1; return BLE_NOP;}

		if( ble_structor -> cmd_d == dooropen_code ){
			rx_enable = 1; 
			return BLE_OPEN_DOOR;
		}else if( ble_structor -> cmd_d == 0x00 ){
		    rx_enable = 1; 
			return BLE_CLOSE_DOOR;
		}else{
			rx_enable = 1; 
		 	return BLE_NOP;
		}
	}
}