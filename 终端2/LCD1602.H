#ifndef LCD1602
#define LCD1602
#define uchar unsigned char 
#define uint unsigned int
#include <reg52.h>
#include <string.h>
sbit   RS_p=P3^5; //1602液晶屏引脚
sbit   RW_p=P3^6;
sbit   EN_p=P3^4;
#define data_p P0

void Delay_us(uchar t){
	while(t--);
}
void Delay_ms(uchar t){
	while(t--)
		Delay_us(453);
}

void busy(void)	//忙检测，使接口不忙
{
	data_p=0xff;//作为数据口
	RS_p=0;
	RW_p=1;
	EN_p=1;
	while(data_p & 0x80){
 		EN_p=0;  
 		EN_p=1;  
	}
	EN_p=0;
}
void write_char(uchar Data,bit data_cmd,bit Check){
	if(Check) 
		busy();
	RS_p=data_cmd;
	RW_p=0;
	data_p=Data;
	EN_p=1;
	_nop_();
	EN_p=0;
}
void write_com(uchar com)//写命令 相当于write_char(X,0,1);
{
	RS_p=0;
	EN_p=0;
	data_p=com;
	Delay_ms(5);
	EN_p=1;
	Delay_ms(5);
	EN_p=0;
}

void set_X_Y(uchar X,uchar Y) //设置行列，1602x:0-1 y：0-15
{
	X&=0x01;
	Y&=0x0f;
	if(X)
		write_char(0x80+0x40+Y,0,1);
	else
 		write_char(0x80+Y,0,1);
}

void init(void)
{ //初始化操作
	RW_p=0;
	Delay_ms(15);
	write_char(0x38,0,0); //不忙时
	Delay_ms(5);
	write_char(0x38,0,0);
	Delay_ms(5);
	write_char(0x38,0,0);
	write_char(0x38,0,1);//液晶屏初始化操作
	write_char(0x08,0,1);
	write_char(0x01,0,1);
	write_char(0x06,0,1);
	write_char(0x0c,0,1);
}

#endif