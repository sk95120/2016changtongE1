#ifndef PS2
#define PS2
#define uchar unsigned char 
#define uint unsigned int
#include <reg52.h>
sbit   keyb_clk_p=P2^3;//P3^5;//时轲的开发板
sbit   keyb_data_p=P2^4;//P3^6; //时轲的开发板

uchar keyb_Scan(void)
{ //扫描电位
	uchar i,temp;
	keyb_clk_p=1; //当有信号输入
	keyb_data_p=1;
	temp=0;
	while(keyb_clk_p); //第一次为0
	for(i=0;i<8;i++){
 		temp>>=1;
 		while(!keyb_clk_p); //下沿,第一位
 		while(keyb_clk_p);
 		_nop_();
 		if(keyb_data_p)
 			temp|=0x80; //低位在前
	}
	while(!keyb_clk_p); //校验位
	while(keyb_clk_p);
	while(!keyb_clk_p); //停止位
	while(keyb_clk_p);
	while(!keyb_clk_p);
	return temp;
}
uchar keyb_Get(void) //通码找值
{
	uchar keyb_Code;
	keyb_Code=keyb_Scan(); 
	switch(keyb_Code){
		case 0x1c: return 'A';break;
		case 0x32: return 'B';break;
		case 0x21: return 'C';break;
		case 0x23: return 'D';break;
 		case 0x24: return 'E';break;
 		case 0x2b: return 'F';break;
 		case 0x34: return 'G';break;
 		case 0x33: return 'H';break;
 		case 0x43: return 'I';break;
 		case 0x3b: return 'J';break;
 		case 0x42: return 'K';break;
 		case 0x4b: return 'L';break;
 		case 0x3a: return 'M';break;
 		case 0x31: return 'N';break;
 		case 0x44: return 'O';break;
 		case 0x4d: return 'P';break;
 		case 0x15: return 'Q';break;
 		case 0x2d: return 'R';break;
 		case 0x1b: return 'S';break;
 		case 0x2c: return 'T';break;
 		case 0x3c: return 'U';break;
 		case 0x2a: return 'V';break;
 		case 0x1d: return 'W';break;
 		case 0x22: return 'X';break;
 		case 0x35: return 'Y';break;
 		case 0x1a: return 'Z';break;
 		case 0x45: return '0';break;
 		case 0x16: return '1';break;
 		case 0x1e: return '2';break;
 		case 0x26: return '3';break;
 		case 0x25: return '4';break;
 		case 0x2e: return '5';break;
 		case 0x36: return '6';break;
 		case 0x3d: return '7';break;
 		case 0x3e: return '8';break;
 		case 0x46: return '9';break;
 		case 0xf0: return '@';break; //FO断码标志
 		default: return 0xff; break;
	}
}
#endif