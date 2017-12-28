#ifndef time
#define time
#define uchar unsigned char 
#define uint unsigned int
#include <reg52.h>
uchar sec2,sec1,min1,min2,ms2,ms1,count,ms,sec,min;//时间数据
uchar sented_num,now_num,sent_data[80];//保存信息的数组

void init_time(void)//时间初始化
{	
	sented_num=0,now_num=0,min2=0,min1=0,sec2=0,sec1=0,ms2=0,ms1=0;
	TMOD=0x21;	//T1 M1=1,M0=0;T0 M1=0,M0=1 
	PCON=0x00;	//不加强
	SCON = 0x80;	//工作方式2
	TH0=(65536-50000)/256;//时间初值
	TL0=(65536-50000)%256;
	EA=1; //T0三个初始参数
	ET0=1;
	TR0=1;
    TH1 = 0xf9;//12MHZ晶振的波特率 
	TL1 = 0xf9;  
	TB8=P;//校验位 RSW^0; 
}

void time_show(int X,uchar keyb_Code)
{ //显示时间的函数
	set_X_Y(X,0);		
 	write_char(keyb_Code,1,1);
 	write_char(keyb_Code,1,1);
 	write_char(keyb_Code,1,1);
	set_X_Y(X,6);
	write_char(0x30+min2,1,1);
 	write_char(0x30+min1,1,1);
 	write_char(0x3A,1,1);
	write_char(0x30+sec2,1,1);
	write_char(0x30+sec1,1,1);
	write_char(0x3A,1,1);
	write_char(0x30+ms2,1,1);
	write_char(0x30+ms1,1,1);
}
void timer0() interrupt 1//时间中断
{
	TH0=(65536-50000)/256;//再次赋初值
	TL0=(65536-50000)%256;
	count++; //50ms
	if(count==20) //毫秒满1000进1
	{
		count=0;
		sec1++;
		if(sec1==9)//秒的个位
		{
			sec1=0;
			sec2++;
			if(sec2==6)//秒的十位
			{
				sec2=0;
				min1++;
				if(min1==9)//分钟的个位
				{
					min1=0;
					min2++;
					if(min2==6)//分钟的十位
					{	//时间满1小时归零
						min2=0,min1=0,sec2=0,sec1=0,ms2=0,ms1=0,count=0;
					}
				}
			}
		}
	}
	ms2=(count*5)/10;//把毫秒分解为十位和个位
	ms1=(count*5)%10;
	ms=count;
	sec=(sec2*10+sec1);//把秒和分的十位和各位组合成秒、分
	min=(min2*10+min1);
}

#endif