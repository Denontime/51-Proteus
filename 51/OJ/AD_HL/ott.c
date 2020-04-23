#include<reg52.h>
sbit ALE = P3^1;
sbit ST = P3^0;
sbit EOC = P3^2;
sbit OE = P3^6;
sbit CLK = P3^7;
sbit wexuan1=P0^0;
sbit wexuan2=P0^1;
//sbit IN1 = P0^5;
//sbit IN2 = P0^6;
//sbit IN3 = P0^7;
unsigned int code table[]={
0xc0,0xf9,0xa4,0xb0,
0x99,0x92,0x82,0xf8,
0x80,0x90}; //段码表
unsigned int n=0,flag1=1;flag=1,ad_data,num1=0,num2=0;
void delay(xms)
{
unsigned int x,y;
for(x=xms;x>0;x--)
for(y=110;y>0;y--);
}
void display()//2位数码管显示
{
float a;
a=(float)ad_data/256*5*1000;
num1=(int)a/1000;
num2=(int)a/100-num1*10;
P1=table[num1];
wexuan1=1;
delay(1);
wexuan1=0;
P1=0xff;
P1=0x7f; //小数点
wexuan1=1;
delay(1);
wexuan1=0;
P1=0xff;
P1=table[num2];
wexuan2=1;
delay(1);
wexuan2=0;
P1=0xff;
}
void init()
{
ST = 0;
ALE = 0;
OE = 0;
CLK = 0;
// _EOC= 1;
TMOD=0x12;IP=0x09;
TH0=(65536-22)/256;
TL0=(65536-22)%256;
TH1=(65536-45872)/256;
TL1=(65536-45872)%256;
EA=1;
ET0=1;ET1=1;
TR0=1;TR1=1;
EX0=1;//打开外部中断
IT0=1;//从高到低的负跳变有效
P0=0x00;
}
void start() //int a,int b,int c) //选择通道
{
// IN1 = a;IN2 = b;IN3 = c;
ALE = 0;ALE = 1;
ST = 0;ST = 1;
ALE = 0;ST = 0;
display();
delay(10);
}
int read()
{
unsigned int dat;
P2 = 0xff;
OE = 1;
display();
dat = P2;
OE = 0;
return (dat);
}
void main()
{
init();
while(1)
{ ST = 0;
ST = 1;
ST = 0;
if(n==20)
{
flag=1;
// display();
// start();
// while(EOC==0);//转换结束(EOC=1)读出数据后显示
// _EOC=0;
ad_data=read();
flag=0;
display();
//
n = 0;
// display();
// _EOC = 1;
}
display();//显示的是if 中的电压值
}
}
void INTR_0() interrupt 0
{
while(1){
ad_data = read();
flag = 0;
display();}
}
void timer0() interrupt 1 // 输出500kHz 方波 
{
CLK=~CLK;
}
void timer1() interrupt 3
{
TH1=(65536-45872)/256;
TL1=(65536-45872)%256;
n++;
flag1=0;
}