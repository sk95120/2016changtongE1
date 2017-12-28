#include <reg52.h>
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int
#define Array_Max 80
#include "LCD1602.h"
#include "GET.h"

void main(void){
	init();
	initget();
	get_m();
	while(1){
		;
	}
}
