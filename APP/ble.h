#ifndef _BLE_H_
#define _BLE_H_
#include "uart.h"

#define wifiswitch_addr  (0x20)
#define doorswitch_addr  (0x40)

#define doorcontrol_type (0x02)
#define dooropen_code    (0x01)

typedef struct BLE_DATA_STRUCT {
	u8 start_flag;
	u8 src_addr  ;
	u8 dst_addr  ;
	u8 cmd_t     ;
	u8 cmd_d     ;
	u8 end_flag  ;	
}BLE_DATA_STRUCT;

typedef enum BLE_CMD {
  	BLE_NOP,
	BLE_OPEN_DOOR,
	BLE_CLOSE_DOOR
}BLE_CMD;

BLE_CMD Ble_Decode(void);
#endif