//////////////////////////////////////////////////////////////////////////////////////////////
//ad采样电压值（0--5.00v）,使用AD0809芯片
//////////////////////////////////////////////////////////////////////////////////////////////
#include "reg52.h"

//AD0809相关
sfr ADDR = P2;      //通道地址
sbit START = P3^4;  //启动转换
sbit EOC = P1^7;    //转换结束标志
sbit OE = P3^2;    //输出使能
sbit ALE = P3^0;    //地址锁存

unsigned char ad_value=0 ;				//原始ad值

void delay100us(int us) //延时100us子程序
{
unsigned int i;
unsigned char j;
for(i=0;i<us;i++)
{ 
for(j=0;j<2;j++);
for(j=0;j<102;j++);
}
}


void delay_short(void)//短延时
{
unsigned char i=10;
while(i--);
}



void all_init(void);       //全部初始化
void ad_samp(void);        //ad采样函数


void main(void)
{
all_init();
delay100us(10000);
while(1)
{
ad_samp();

pwm_deal();   
delay100us(10000);
};
}


void all_init(void)
{
//100us定时器初始化
TMOD = 0x02;
TH0 = 0x00;
TL0 = 0x9c;
//开中断
EA = 1;
ET1 = 1;
TR1 = 1;
}


void ad_samp(void)
{
ADDR=0x00;//选中IN0号通道
ALE=1;    delay_short();
START=1;    delay_short();
ALE=0;    delay_short();      
START=0;    delay_short();
while(EOC==1);
delay_short();
while(EOC==0);
delay_short();
OE=1;delay_short();
ad_value=P0;
OE=0;delay_short();
}
