#include <reg52.h>                                   //包含头文件
#include <intrins.h>

#define uint unsigned int
#define uchar unsigned char

sbit P1_0=P3^5;
uint PERIOD=2000;
uint HI_NUM=200;
uint LOW_NUM;
uchar HIGAO,HIDI;
uchar LOWGAO,LOWDI;

sbit ale=P2^6;
sbit start=P2^7;
sbit eoc=P3^3;
sbit oe=P3^4;                                             //定义adc0809的控制引脚

sbit adda=P2^0;
sbit addb=P2^1;
sbit addc=P2^2;                                           //adc0809的地址引脚

sbit clk=P3^7;                                            //adc0809时钟引脚

sbit X9313W_INC  = P3^0;               //计数脉冲输入端，下降沿触发
sbit X9313W_UPDN = P3^1;               //方向，高电平加、低电平减
sbit X9313W_CS   = P3^2;               //片选,低电平选中开始操作

sbit LCD1602_E   = P2^3;
sbit LCD1602_RS  = P2^4;
sbit LCD1602_RW  = P2^5;
sbit LCD1602_DATAPINS = P1;

unsigned int  voltdata,realvolt1,realvolt,i,j,t,sum,a[5];         //定义全局变量

void delay( int us)                      //延时函数
{
	int aus;
	for(aus=0;aus<us;aus++)
		_nop_();
}

void Lcd1602_Delayms(int ms)		//@11.0592MHz
{
	unsigned char i, j, k;
for(k=ms;k>0;k--)
{
	_nop_();
	i = 2;
	j = 199;
	do
	{
		while (--j);
	} while (--i);
}
}


int ADC()                         //采集数据
{
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
        return voltdata;
}
/***********调整电阻*****************/

/*************************************/
/*   调整电阻到第RNumber阶阻值     */
/*入口：RNumber：阶阻值0～31         */
/*出口：无                           */
/*************************************/
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
/*************************************/

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
/*************************************/

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
	
		start = 0;
		oe = 0;
		clk = 0;
	
		adda=0;
		addb=0;
		addc=0;
		ale  = 0;
		ale  = 1;                  //AD输入线路选择
	
		X9313W_SetVol(15);                  //将电阻器设为中间值
		delay(1000);
}

void LcdWriteCom(uchar com)      //写入命令
{
      LCD1602_E = 0;         //使能清零

      LCD1602_RS = 0;        //选择发送命令
      LCD1602_RW = 0;        //选择写入

      LCD1602_DATAPINS = com;    //放入命令
      Lcd1602_Delayms(1);                   //等待数据稳定


      LCD1602_E = 1;             //写入时序，一个下降沿，将命令传达到LCD
      Lcd1602_Delayms(5);        //保持时间
      LCD1602_E = 0;
}

void LcdWriteData(uchar dat) //写入数据

{

LCD1602_E = 0; //使能清零
LCD1602_RS = 1; //选择输入数据
LCD1602_RW = 0; //选择写入
LCD1602_DATAPINS = dat; //写入数据
Lcd1602_Delayms(1);
LCD1602_E = 1; //写入时序，一个下降沿
Lcd1602_Delayms(5); //保持时间
LCD1602_E = 0;

}

void  LCD1602_display()

{

      unsigned int i;
      unsigned char  cu[9]="Current:";


      for(i=0;i<8;i++) //第一行显示‘current’

     {
                LcdWriteData(cu[i]);
      }

      LcdWriteCom(0x80+0x40);

      LcdWriteData(voltdata);

}

void LcdInit()                                         //LCD初始化子程序

{
     LcdWriteCom(0x38);  //功能设定 0011 1000 （8位数据线、显示2行、每个字符为5*7点阵）

      //
      LcdWriteCom(0x0c);  //显示开关控制 0000 1100（或+光标不闪烁0x0d：0000 1101）（开显示、不开光标、光标闪烁或不闪烁）
      LcdWriteCom(0x06);  //进入模式设置 0000 0110（写入数据后光标右移一位及显示屏不动）
      LcdWriteCom(0x01);  //清屏
      LcdWriteCom(0x80);  //设置数据指针起点 //DB7为高电平

}



void main()                               //主程序
{
		init();
		LcdInit();
        while(1)                          //进入while循环
        { 
					ADC();
					if(voltdata>128)                //判断输入
					{
					       X9313W_IncVol(voltdata-128);               //调大变阻器
					       LCD1602_display();                         //显示
					}
					else
					{
					       X9313W_DecVol(128-voltdata);               //调小变阻器
					       LCD1602_display();                         //显示
					}
        }                           
}

void timer0() interrupt 1          // 输出500kHz 方波 
{
    if(P1_0==1)
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
}
