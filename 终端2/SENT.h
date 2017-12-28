#ifndef SENT
#define SENT
#define uchar unsigned char 
#define uint unsigned int
#include <reg52.h>

void sent_m(uint X,uchar keyb_Code)//双机通信工作方式2，SCON 0X80,SM0=1,SM1=0;
{ 
        ES=0;
        TI=0;
        SBUF = 0xAA;//唤醒主机接口
         while(!TI); 
         TI = 0;    
        Delay_ms(5);
        SBUF = Machine;//SUBF接受
         while(!TI); // 等特数据传送
         TI = 0;    //数据传送结束后归零 
         Delay_ms(5);   
        SBUF = keyb_Code;
         while(!TI); 
         TI = 0;    
         Delay_ms(5);
         SBUF = X; //X=0时，代表按下键盘，同时在1602第一行显示；X=1时，抬起键盘，同时在1602第二行显示；
         while(!TI); 
         TI = 0;    
         Delay_ms(5);
         SBUF = min;
         while(!TI); 
         TI = 0;    
         Delay_ms(5);
         SBUF = sec;
         while(!TI); 
         TI = 0;    
         Delay_ms(5);
         SBUF = ms;
         while(!TI); 
         TI = 0;    
         ES=1;
         sent_data[(now_num)%Array_Max]=keyb_Code; //数据保存到循环队列，防止溢出；若溢出则覆盖
         sent_data[(now_num+1)%Array_Max]=X;
         sent_data[(now_num+2)%Array_Max]=min;
         sent_data[(now_num+3)%Array_Max]=sec;
         sent_data[(now_num+4)%Array_Max]=ms;

}

void auto_sent(void){ //每隔1分钟自动上传数据到主机
        uchar i,j;
        if(sented_num<now_num){
                for(i=0;i<=now_num;i+=5){//直到发送完成所有数据
                        TI=0;
                        SBUF = 0xAA;//唤醒主机接口
                        while(!TI); 
                        TI = 0;
                        Delay_ms(5);
                        SBUF = Machine;//发送终端编码好
                        while(!TI); // 等特数据传送
                        TI = 0;   
                        for(j=i;j<i+5;j++){
                                Delay_ms(5);   
                                SBUF = sent_data[j];//依次发送Keyb_Code,X,min,sec,ms;
                                while(!TI); // 等特数据传送(TI发送中断标志)
                                TI = 0;    
                        }
                }
                sented_num=now_num;//现有数据的个数赋值给已发送的个数
        }
}

#endif