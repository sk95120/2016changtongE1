#define uchar unsigned char
#define uint unsigned int
#define Machine '2' //终端编号2
#define Array_Max 80
#include <reg52.h>
#include <intrins.h>
#include "PS2.h"
#include "LCD1602.h"
#include "TIME.h"
#include "SENT.h"

void main(void){
	uchar keyb_Code,code_Temp='@';
	init();		//液晶屏屏、时间初始化
	init_time();

	while(1){
 		keyb_Code=keyb_Get();
 		if(keyb_Code!=0xff){ // 无视乱电位
 			if(keyb_Code=='@'){ //0xf0的断码提示 
 				keyb_Code=keyb_Get(); 
 				time_show(1,keyb_Code);
 				sent_m(1,keyb_Code); //将数据保存到数组
 				now_num+=5;			//数组的内容增加
 				if(now_num>Array_Max)	//如果数组溢出，采用循环队列数据结构逐个覆盖原数据
 					now_num=Array_Max;
 			}else{
 				if(keyb_Code!=code_Temp){
 					time_show(0,keyb_Code);
 					code_Temp=keyb_Code;
 					sent_m(0,keyb_Code);
 					now_num+=5;
 					if(now_num>Array_Max)
 						now_num=Array_Max;
 				}
 				
 			}
 		}
 		if(sec==30&&ms==30) //定时自动上传数据
 			auto_sent();				 		
	}
}