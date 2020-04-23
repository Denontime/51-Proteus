#include<reg52.h> //头文件

#define uint unsigned int
#define uchar unsigned char

void delay_ms(uint);//mS级带参数延时函数声明

sbit LED1=P1^0;

sbit LED2=P1^1;

sbit LED3=P1^2;

sbit LED4=P1^3;

sbit LED5=P1^4;

sbit LED6=P1^5;

sbit LED7=P1^6;

sbit LED8=P1^7;


void main()

{

    while(1)

    {

        LED1=0;

        delay_ms(500);


        LED2=0;

        delay_ms(500);


        LED3=0;

        delay_ms(500);


        LED4=0;

        delay_ms(500);

 

        LED5=0;

        delay_ms(500);


        LED6=0;

        delay_ms(500);


        LED7=0;

        delay_ms(500);


        LED8=0;

        delay_ms(500);

				LED1=1;
				
				delay_ms(500);

    }

}

void delay_ms(uint z) //延时子程序

{

    uint x,y;

    for(x=z; x>0; x--)

        for(y=110; y>0; y--);

}