#include <reg52.h>                                   //包含头文件
#include <intrins.h>
#include <stdio.h>
//#include <stdlib.h>
//#include <AT89x51.h>
//#include <1602.h>
//#include <delay.h>

#define uint unsigned int
#define uchar unsigned char

sbit P1_0=P3^5;
uint PERIOD=2000;
uint HI_NUM=200;
uint LOW_NUM;
uchar HIGAO,HIDI;
uchar LOWGAO,LOWDI;

//sbit ale=P2^6;
//sbit start=P2^7;
//sbit eoc=P3^3;
//sbit oe=P3^4;                                             //定义adc0809的控制引脚
/*************************************************************************************************************/

sbit OE =P3^4;
sbit EOC=P3^3;
sbit START=P2^7;
sbit CLK=P3^7;

//sbit CS0=P2^0;
//sbit CS1=P2^1;
//sbit CS2=P2^2;
//sbit CS3=P2^3;

/*************************************************************************************************************/
//sbit adda=P2^0;
//sbit addb=P2^1;
//sbit addc=P2^2;                                           //adc0809的地址引脚

//sbit clk=P3^7;                                            //adc0809时钟引脚

sbit X9313W_INC  = P3^0;               //计数脉冲输入端，下降沿触发
sbit X9313W_UPDN = P3^1;               //方向，高电平加、低电平减
sbit X9313W_CS   = P3^2;               //片选,低电平选中开始操作

# define LCD_DB        P1
        sbit         LCD_RS=P2^4;
        sbit         LCD_RW=P2^5;
        sbit         LCD_E=P2^3;
/************************************************************************************************************/

unsigned int  real,i,j,t,sum,a[5];         //定义全局变量
float voltdata;
unsigned char tem[10];
static char table[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

/************************************************************************************************************/


void num2char()
{
    uint i;
    int temp = voltdata/1;
    double t2 = 0.0;
 
        if (temp==0)
            tem[0] = table[0];
        else
            tem[0] = table[temp%10];

    tem[1] = '.';
    temp = 0;
    t2 = voltdata;
    for(i=1; i<=4; i++)
    {
        temp = t2*10;
        tem[1+i] = table[temp%10];
        t2 = t2*10;
    }
    tem[7] = '\0';
}
 


/***********************************************************************************************************/
void LCD_init(void);//初始化函数
void LCD_write_command(uchar command);//写指令函数
void LCD_write_data(uchar dat);//写数据函数
//void LCD_disp_char(uchar x,uchar y,uchar dat);//在某个屏幕位置上显示一个字符,X（0-16),y(1-2)
void delay_n40us(uint n);//延时函数
//********************************
//*******初始化函数***************
void LCD_init(void)
{
LCD_write_command(0x38);//设置8位格式，2行，5x7
LCD_write_command(0x0c);//整体显示，关光标，不闪烁
LCD_write_command(0x06);//设定输入方式，增量不移位
LCD_write_command(0x01);//清除屏幕显示
delay_n40us(100);
}
//********************************
//********写指令函数************
void LCD_write_command(uchar dat)
{
LCD_DB=dat;
LCD_RS=0;//指令
LCD_RW=0;//写入
LCD_E=1;//允许
LCD_E=0;
delay_n40us(1);//实践证明，我的LCD1602上，用for循环1次就能完成普通写指令。
}
//*******************************
//********写数据函数*************
void LCD_write_data(uchar dat)
{
LCD_DB=dat;
LCD_RS=1;//数据
LCD_RW=0;//写入
LCD_E=1;//允许
LCD_E=0;
delay_n40us(1);
}
//********************************
/*******显示一个字符函数*********
void LCD_disp_char(uchar x,uchar y,uchar dat)  ////参数：（列，行【0第一行，1第二行】，字符）
{
uchar address;
if(y==1)
         address=0x80+x;
else
         address=0xc0+x;
LCD_write_command(address);
LCD_write_data(dat);
}
*/
void DisplayListChar (unsigned char x,unsigned char y,unsigned char *p,unsigned char count)
//参数：（列，行【0第二行，1第一行】，字符串，字符串长度）
{
unsigned char i;   
    for(i=0;i<count;i++)
    {
		uchar address;
        if(y==1)  
			address=0x80+x; //当要显示第一行时地址码+0x80;
        else 
			address=0xc0+x;; //在第二行显示是地址码+0xC0;

        LCD_write_command(address);
		LCD_write_data(*p); //发送地址码
                            //发送要显示的字符编码
        x++;
        p++;
    }       
}   


/******************************/
//********延时函数***************
void delay_n40us(uint n)
{ uint i;
      uchar j;            
        for(i=n;i>0;i--)
           for(j=0;j<2;j++);          //在这个延时循环函数中我只做了2次循环，
}                                         //实践证明我的LCD1602上普通的指令只需1次循环就能可靠完成。

/************************************************************************************************************/


void delay( int us)                      //延时函数
{
	int aus;
	for(aus=0;aus<us;aus++)
		_nop_();
}


void ADC()                         //采集数据
/*{
        sum=0;                     //滤波开始
        for (i=0;i<5;i++)           
        {
                start=1;                         //启动AD转换。
                start=0;
                while(eoc==0);                   //等待转换结束。
                oe=1;
                a[i]=P1;                         //输入转换结果。
                delay(50);                       //每隔5usAD转换并采集一次数据，将得到的数值存入数组中
        }
        for (j=0;j<4;j++)                                
                for(i=0;i<4-j;i++)
                        if(a[i]>a[i+1])
                        {
                                t=a[i];
                                a[i]=a[i+1];
                                a[i+1]=t;
                        }
        for(i=1;i<4;i++)
        {
                sum+=a[i];
        }                                  //采用冒泡法比较，取中间3个数值

        voltdata=sum/3;
        delay(1);
        oe=0;
				voltdata=3.4523;
				num2char();
				//sprintf(temp,"%1d",voltdata);
				//sprintf(cu,"%4d",voltdata%1000);
}*/
{	
	START=0;
	START=1;
	START=0;
		   while(EOC==0);
		   OE=1;
		   real=P0;
		   OE=0;
	voltdata=real*0.0196;
	
	num2char();
}


/***********调整电阻*****************/

/*************************************/
/*   调整电阻到第RNumber阶阻值     */
/*入口：RNumber：阶阻值0～31         */
/*出口：无                           */
/*************************************
void X9313W_SetVol(unsigned char RNumber)
{ 
    unsigned char i;

    X9313W_INC = 1;                   
    _nop_();
    _nop_();
    X9313W_CS = 0;
    _nop_();
    _nop_();
    X9313W_UPDN = 0;                    //设为减

    for(i=0;i<32;i++)
    {
        X9313W_INC = 1;
        _nop_();
        _nop_();
        X9313W_INC = 0;
        _nop_();
        _nop_();
    }                                   //减到0

    X9313W_UPDN = 1;                    //设为加
    for(i=0;i<RNumber;i++){
        X9313W_INC=1;
        _nop_();
        _nop_();
        X9313W_INC=0;
        _nop_();
        _nop_();
    }                                   //加到RNumber
     
    X9313W_INC = 1;                     //电阻值保存
    _nop_();
    _nop_();
    X9313W_CS = 1;
    _nop_();
    _nop_();
    X9313W_UPDN = 1;
    _nop_();
    _nop_();
    X9313W_INC = 1;
}

/*************************************/
/*     调整电阻增加RNumber阶阻值   */
/*入口：RNumber：阶阻值0～31         */
/*出口：无                           */
/*************************************

void X9313W_IncVol(unsigned char RNumber)
{ 
    unsigned char i;
     
    X9313W_INC = 1;                   
    _nop_();
    _nop_();
    X9313W_CS = 0;
    _nop_();
    _nop_();
    X9313W_UPDN = 1;                  //加RNumber个指定值
    
    for(i=0;i<RNumber;i++)
		{
        X9313W_INC=1;
        _nop_();
        _nop_();
        X9313W_INC=0;
        _nop_();
        _nop_();
    }
     
    X9313W_INC = 1;                    //电阻值保存
    _nop_();
    _nop_();
    X9313W_CS = 1;
    _nop_();
    _nop_();
    X9313W_UPDN = 1;
    _nop_();
    _nop_();
    X9313W_INC = 1;
}
/*************************************/
/*     调整电阻减少RNumber阶阻值      */
/*入口：RNumber：阶阻值0～31          */
/*出口：无                            */
/*************************************

void X9313W_DecVol(unsigned char RNumber)
{
    unsigned char i;
    
    X9313W_INC = 1;                   
    _nop_();
    _nop_();
    X9313W_CS = 0;
    _nop_();
    _nop_();
    X9313W_UPDN = 0;                    //减RNumber个指定值
    for(i=0;i<RNumber;i++){
        X9313W_INC=1;
        _nop_();
        _nop_();
        X9313W_INC=0;
        _nop_();
        _nop_();
    }
     
    X9313W_INC = 1;                    //电阻值保存
    _nop_();
    _nop_();
    X9313W_CS = 1;
    _nop_();
    _nop_();
    X9313W_UPDN = 1;
    _nop_();
    _nop_();
    X9313W_INC = 1;
}

*/

void init()                             //初始化程序
{
		P1_0=0;
		LOW_NUM=PERIOD-HI_NUM;
		HIGAO=(65536-HI_NUM)/256;
		HIDI=(65536-HI_NUM)%256;
		LOWGAO=(65536-LOW_NUM)/256;
		LOWDI=(65536-LOW_NUM)%256;

		TMOD=0x01;
		TH0=HIGAO;
		TL0=HIDI;
	
		EA =1;                              //打开CPU总中断
		ET0=1;                              //定时器T0的溢出中断允许
		ET1=1;                              //定时器T1的溢出中断允许
		TR0=1;
		TR1=1;                              
		EX0=1;                              //外部中断0允许
		IT0=1;                              //从高到低的负跳变有效
	
		TMOD=0x02;
	TH0=0x14;
	TL0=0x00;
	TR0=1;
	ET0=1;
	EA=1;
	
	/*
		start = 0;
		oe = 0;
		clk = 0;
	
		adda=0;
		addb=0;
		addc=0;
		ale  = 0;
		ale  = 1;                  //AD输入线路选择
	*/
		//X9313W_SetVol(15);                  //将电阻器设为中间值
	
		delay(1000);
}

void main()                               //主程序
{
		init();             //初始化
		LCD_init();         //初始化液晶屏
		DisplayListChar(0,1,"Current:",8);    //写入第一行信息，主循环中不再更改此信息，所以在while之前写入
    //DisplayListChar(8,0,tem,5);
    //DisplayListChar(7,0,".",1);
    //DisplayListChar(8,0,cu,4);
    DisplayListChar(14,0,"mA",2);
        while(1)                          //进入while循环
        { 
					ADC();
					DisplayListChar(8,0,tem,5);
					DisplayListChar(14,0,"mA",2);
					/*if(voltdata>128)                //判断输入
					{
					       //X9313W_IncVol(voltdata-128);               //调大变阻器
					       DisplayListChar(3,0,tem,12);                //显示到液晶屏
					}
					else
					{
					       //X9313W_DecVol(128-voltdata);               //调小变阻器
					       DisplayListChar(3,0,tem,12);                //显示到液晶屏
					}*/
        }                           
}

/*void timer0() interrupt 1          // 输出500kHz 方波 
{
  /*  if(P1_0==1)
		{
		   P1_0=0;
		   TH0=LOWGAO;
		   TL0=LOWDI;
		}
		else
		{
		   P1_0=1;
		   TH0=HIGAO;
		   TL0=HIDI;
		}
	clk=~clk;
}*/

void t0(void) interrupt 1 using 0
{
CLK=~CLK;
}
 
