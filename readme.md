# BLESwitch

![image-20231206133514032](pic/image-20231206133514032.png)

![image-20231206133636743](pic/image-20231206133636743.png)

## protocol

### frame format

[![image-20231122110725883](pic/image-20231122110725883.png)](https://github.com/wa-kakalala/NanoClient/blob/main/pic/image-20231122110725883.png)

- 0x55 : frame start flag
- src-addr : sender's address
- dst-addr: receiver's address
- cmd-t : command type
- cmd-d: command data
- 0xff: frame end flag

### device address

- cell phone : 0b000xxxxx
- wifi switch : 0b001xxxxx
- ble door : 0b010xxxxx

### cmd-t & cmd-d

0b00000000 type is retained

- switch control type : 0b00000001
	- turn on code : 0b00000001
	- turn off code : 0b00000000
- door control type : 0b00000010
	- open code : 0b00000001