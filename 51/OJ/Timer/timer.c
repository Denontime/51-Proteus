#include<reg51.h>
typedef unsigned int u16;
typedef unsigned char u8;
unsigned char code smgduan[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};          //显示0~F的值
unsigned char Time;                                                                                     //用来存放定时时间

//--声明全局函数--//
void DigDisplay();                                                                           //动态显示函数
void DigDisplay1();                                                                          //动态显示函数
void Timer0Cofig(void);
void delay(u16 i);
u8 m,f,s;
u8 nian=19,yue=3,ri=17;
sbit K4=P1^3;
sbit K3=P1^2;
sbit K2=P1^1;
sbit K1=P1^0;
u8 x;                                                                                        //切换显示界面
u8 x1;                                                                                       //暂停时间，进入调时
void main(void)
{               
        P2=0X00;
        Timer0Cofig();
        P1=0XFF;
        while(1)
        {
      if(!K4)                                                                                     //切换显示界面
          {
          delay(10);
          if(!K4)
          x=!x;
          while(!K4);                                                                            //等待按键松开
          }
     if(!K1)
          {
          delay(10);
          if(!K1)
          {
          TR0=!TR0;
          x1=!x1;
          }
          while(!K1);                                                                             //等待按键松开
          }
           if(!K2&&x1)
          {
          delay(10);
          if(!K2&&x1)
          f++;
          while(!K2&&x1);                                                                         //等待按键松开
          }
           if(!K3&&x1)
          {
          delay(10);
          if(!K3&&x1)
          s++;
          while(!K3&&x1);                                                                         //等待按键松开
          }
                if(m==60)                                                                             //秒进位
                {
                m=0;
                f++;
                }
                if(f==60)                                                                             //分钟进位
                {
                f=0;
                s++;
                }
                if(s==24)                                                                             //小时进位
                {
                s=0;
                ri++;
                }
                if(!x)
                DigDisplay();
                if(x)
                DigDisplay1();                                               
   }
}
void delay(u16 i)
{
        while(i--);       
}
void DigDisplay()                                                                            //显示秒分时函数
{
   u16 i,i1;                                                                                 //显示秒
   i=m%10;
   i1=(m/10)%10;
   P2=0X80;
   P0=smgduan[i];
   delay(1);                                                                                 //间隔一段时间扫描
   P2=0X40;
   P0=smgduan[i1];
   delay(1);                                                                                 //间隔一段时间扫描
   P2=0X20;                                                                                         //分隔符
   P0=0xBF;
   delay(1);
//////////////////////////////显示分钟
   i=f%10;
   i1=(f/10)%10;
   P2=0X10;
   P0=smgduan[i];
   delay(1);                                                                                 //间隔一段时间扫描
   P2=0X08;
   P0=smgduan[i1];
   delay(1);                                                                                 //间隔一段时间扫描
   P2=0X04;          
   P0=0xBF;
   delay(1);
   //////////////////////////////显示小时
   i=s%10;
   i1=(s/10)%10;
   P2=0X02;
   P0=smgduan[i];
   delay(1);                                                                                 //间隔一段时间扫描
   P2=0X01;
   P0=smgduan[i1];
   delay(1);
///////////////////////////////
        delay(80);                                                                               //间隔一段时间扫描       
}
void DigDisplay1()                                                                           //显示年月日函数
{
   u16 i,i1;                                                                                 //显示日
   i=ri%10;
   i1=(ri/10)%10;
   P2=0X80;
   P0=smgduan[i];
   delay(1);                                                                                 //间隔一段时间扫描
   P2=0X40;
   P0=smgduan[i1];
   delay(1);                                                                                 //间隔一段时间扫描
   P2=0X20;                                                                                        
   P0=0xBF;
   delay(1);
//////////////////////////////显示月
   i=yue%10;
   i1=(yue/10)%10;
   P2=0X10;
   P0=smgduan[i];
   delay(1);                                                                                 //间隔一段时间扫描
   P2=0X08;
   P0=smgduan[i1];
   delay(1);                                                                                 //间隔一段时间扫描
   P2=0X04;       
   P0=0xBF;
   delay(1);
   //////////////////////////////显示年
   i=nian%10;
   i1=(nian/10)%10;
   P2=0X02;
   P0=smgduan[i];
   delay(1);                                                                                //间隔一段时间扫描
   P2=0X01;
   P0=smgduan[i1];
   delay(1);
///////////////////////////////
        delay(80);                                                                              //间隔一段时间扫描       
}
void Timer0Cofig(void)
{
        TMOD = 0x01;     //定时器0选择工作方式1
    TH0  = 0x3C;         //设置初始值,定时50MS
    TL0  = 0xB0;
    EA   = 1;             //打开总中断
    ET0  = 1;                 //打开定时器0中断
    TR0  = 1;                 //启动定时器0       
}
void Timer0() interrupt 1
{
        TH0 = 0x3C;             //设置初始值
        TL0 = 0xB0;
        Time++;
        if(Time == 20)
        {
                m++;
                Time = 0;
        }
}
