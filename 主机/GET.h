#ifndef GET
#define GET
#include <reg52.h>
#define uchar unsigned char 
#define uint unsigned int
uchar date_get[80];	//保存传来的数据
uchar date_get_num=0;
uint X; //X=0时，代表按下键盘，同时在1602第一行显示；X=1时，抬起键盘，同时在1602第二行显示；
uchar word_temp,keyb_Code,min1,min2,sec1,sec2,ms1,ms2,min,sec,ms,Flag;  //Flag=0说明传输无错误

void initget(void)
{
  TMOD= 0x20;
  TH1=0xf9;   //12MHZ晶振的波特率
  TL1=0xf9;  
  PCON=0x00;     //不加倍
  SCON = 0x90;     //工作方式2
}
void show_data(){
  	set_X_Y(X,0);   		//X=0时，代表按下键盘，同时在1602第一行显示；X=1时，抬起键盘，同时在1602第二行显示；
  	write_char(keyb_Code,1,1);//键盘操作的字母显示3遍
  	write_char(keyb_Code,1,1);
  	write_char(keyb_Code,1,1);
  	set_X_Y(X,6);				
 	write_char(0x30+min2,1,1); //分钟的十位和个位
  	write_char(0x30+min1,1,1);
  	write_char(0x3A,1,1);
  	write_char(0x30+sec2,1,1);	//秒的十位和个位
  	write_char(0x30+sec1,1,1);
  	write_char(0x3A,1,1);
  	write_char(0x30+ms2,1,1);	//毫秒的十位和个位
  	write_char(0x30+ms1,1,1);
  	if(Flag==1){
        set_X_Y(1,15); //说明校验发现错误，在液晶屏最后显示特殊字符$
        write_char(0x24,1,1);
    }else{
    	set_X_Y(1,15); //说明校验正确，在液晶屏最后显示特殊字符%
        write_char(0x25,1,1);
    }
	set_X_Y(0,15);
	write_char(word_temp,1,1);//终端的编号
    Delay_us(5);
    
}

void save_data(){
    date_get[(date_get_num)%Array_Max]=keyb_Code; //循环队列的数据结构，如果数据溢出则从头覆盖
    date_get[(date_get_num+1)%Array_Max]=X;
    date_get[(date_get_num+2)%Array_Max]=min;
    date_get[(date_get_num+3)%Array_Max]=sec;
    date_get[(date_get_num+4)%Array_Max]=ms;
    if((date_get_num+5)>=Array_Max)  //队列满则覆盖
      date_get_num=(date_get_num+5)%Array_Max;
    else
      date_get_num+=5;
}

void get_m(){ //interrupt 4{ 
  while(1) //监视接口
      {
        Flag=0;//默认校验正确
        RI=0;  //数据传输状态软件赋值
        while(1){
          if(SBUF==0xAA)
            break;//主机借口唤醒
          while(!RI); 
          RI = 0; 
        }
        while(!RI); 
          RI = 0; 
        Delay_ms(5);
        word_temp= SBUF ;//word_temp得到终端编号
        while(!RI); 
        RI = 0;    
        if(P!=RB8) //校验位判断
          Flag=1;
        Delay_ms(5);
        keyb_Code= SBUF ;//键盘操作的数字或字母
        while(!RI);
        RI = 0;
        if(P!=RB8)
          Flag=1;
        Delay_ms(5);
        X= SBUF ;//X=0时，代表按下键盘，同时在1602第一行显示；X=1时，抬起键盘，同时在1602第二行显示；
        while(!RI);
        RI = 0;
        if(P!=RB8)
        Flag=1;
        Delay_ms(5);
        min= SBUF;//键盘操作的分钟
        while(!RI);
        RI = 0; 
        if(P!=RB8)
        Flag=1;
        Delay_ms(5);
        sec= SBUF;//键盘操作的秒
        while(!RI);
        RI = 0;
        if(P!=RB8)
        Flag=1;
        Delay_ms(5);
        ms= SBUF ;//键盘操作的毫秒
        while(!RI); 
        RI = 0;
        if(P!=RB8)
        Flag=1;
        //如果出现校验错误，Flag赋值1
        min2=min/10;
        min1=min%10;
        sec2=sec/10;
        sec1=sec%10;
        ms2=ms/10;
        ms1=ms%10;
        show_data();
        save_data();
	}      
}


#endif