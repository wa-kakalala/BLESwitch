/*
module name :  uart
author      :  wkk
create time :  2023/12/5
*/

#include "uart.h"
/*
BaudRate -> timer
SysClk   : 5.5296Mhz
Baudrate : 115200
5_529_600 / 115200 = 48           ( error )
5_529_600 / 9600   = 576
65536 - 48    = 65488  0xffd0
65536 - 48-24 = 65464  0xffb8

65536 - 576     = 64960  0xfdc0
65536 - 576-288 = 64672  0xfca0
*/


// ext 2 3 4 register
// 7  6     5    4  3 2   1      0
// - ext4 ext3 ext2 - - tlclkO t0clkO
sfr int_clkO = 0x8f;

// 1T mode or /12
//   7     6   5  4    3     2   1 0
// t0x12 t1x12 - T2R T2_C/T T2x12 - -
sfr auxr = 0x8e;

// IE2
//  7  6   5   4   3   2   1    9
//  - ET4 ET3 ES4 ES3 ET2 ESP1 ES2
sfr ie2 = 0xaf;

// timer 
sfr TH2 = 0xd6;
sfr TL2 = 0xd7;

//define uart tx/rx port
sbit uart_rx = P3^0;                  // P3.0 rx
sbit uart_tx = P3^1;                  // P3.1 tx

u8 TEND,TING,tcnt,tbuf;
u8 REND,RING,rcnt,rbuf;

u8 uart_rx_buf[UART_RXBUF_LEN];       // uart_rx_buf 
// rx flag signals
u8 rx_enable = 1;
u8 rx_len    = 0;

void Enable_Ext4(){
    int_clkO = int_clkO | 0x40;
}

void Disable_Ext4(){
    int_clkO = int_clkO & 0xbf;
}

void Timer2_config(){
    // 1T mode
    auxr = auxr & 0xf0;
    auxr = auxr | 0x04;
    // enable timer1 interrupt
    ie2 = ie2 | 0x04;
    // need to enable EA
}

void Timer2_Disable(){
    auxr = auxr & 0xef;
}

void Timer2_Enable(u8 th1,u8 tl1){
    TH2 = th1;
    TL2 = tl1;
    auxr = auxr | 0x10;
}



void Uart_Init(){
    //P3.0 work in interrupt mode ( int4 falling )
    Enable_Ext4();  
    // timer2 config 
    Timer2_config();
    Timer2_Disable();
    // need to enable EA
    
    TEND = 1;
    REND = 0;
    TING = 0;
    RING = 0;
    uart_tx = 1;
}

void Ext4_Interrupt() interrupt 16 {
    RING = 1;
    REND = 0;
    rcnt = 0;
    rbuf = 0;
    Timer2_Enable(0xfc,0xa0);
    Disable_Ext4();
}

void Uart_SendByte( u8 tx_data ) {
    tbuf = tx_data;
    tcnt = 0;
    TEND = 0;
    TING = 1;
    uart_tx = 0;
    Timer2_Enable(0xfd,0xc0);
    while( TEND != 1); // wait for tx complete!!
}

void Uart_LoopTest( ){
    if( TEND && REND == 1 ) {
        Uart_SendByte(rbuf);
        REND = 0;        
    }
}

void Timer2_Interrupt() interrupt 12  {
    if( RING ) {
        if( rcnt == 0 ) {
            Timer2_Disable();
            rbuf = rbuf >> 1;
            if( uart_rx ) rbuf = rbuf | 0x80;  
            Timer2_Enable(0xfd,0xc0);           
        }else if( rcnt == 8 ){
            Timer2_Disable();
            Enable_Ext4();
            RING = 0;
            REND = 1;
            // recv done !!!        
			if( rx_enable ) {
				if( rbuf == 0x55 || rx_len ) {
					uart_rx_buf[rx_len++] = rbuf;	
				}
				if( rx_len == UART_RX_LEN ) {
					rx_len = 0;
					if( uart_rx_buf[UART_RX_LEN-1] == 0xff ){
						rx_enable = 0;
					} 	
				}
			}
        }else {
            rbuf = rbuf >> 1;
            if( uart_rx ) rbuf = rbuf | 0x80;  
        } 
        rcnt ++;
    }else if( TING ) {
        if( tcnt == 8) {
            uart_tx = 1;
        }else if( tcnt == 9) {
            Timer2_Disable();
            TING = 0;
            TEND = 1;
            // tx done !!!   
        }else{
            uart_tx = tbuf & 0x01;
            tbuf = tbuf >> 1;
        }
        tcnt ++;
    }else{
        // other things
    }
}