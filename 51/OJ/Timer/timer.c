#include<reg51.h>
typedef unsigned int u16;
typedef unsigned char u8;
unsigned char code smgduan[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};          //��ʾ0~F��ֵ
unsigned char Time;                                                                                     //������Ŷ�ʱʱ��

//--����ȫ�ֺ���--//
void DigDisplay();                                                                           //��̬��ʾ����
void DigDisplay1();                                                                          //��̬��ʾ����
void Timer0Cofig(void);
void delay(u16 i);
u8 m,f,s;
u8 nian=19,yue=3,ri=17;
sbit K4=P1^3;
sbit K3=P1^2;
sbit K2=P1^1;
sbit K1=P1^0;
u8 x;                                                                                        //�л���ʾ����
u8 x1;                                                                                       //��ͣʱ�䣬�����ʱ
void main(void)
{               
        P2=0X00;
        Timer0Cofig();
        P1=0XFF;
        while(1)
        {
      if(!K4)                                                                                     //�л���ʾ����
          {
          delay(10);
          if(!K4)
          x=!x;
          while(!K4);                                                                            //�ȴ������ɿ�
          }
     if(!K1)
          {
          delay(10);
          if(!K1)
          {
          TR0=!TR0;
          x1=!x1;
          }
          while(!K1);                                                                             //�ȴ������ɿ�
          }
           if(!K2&&x1)
          {
          delay(10);
          if(!K2&&x1)
          f++;
          while(!K2&&x1);                                                                         //�ȴ������ɿ�
          }
           if(!K3&&x1)
          {
          delay(10);
          if(!K3&&x1)
          s++;
          while(!K3&&x1);                                                                         //�ȴ������ɿ�
          }
                if(m==60)                                                                             //���λ
                {
                m=0;
                f++;
                }
                if(f==60)                                                                             //���ӽ�λ
                {
                f=0;
                s++;
                }
                if(s==24)                                                                             //Сʱ��λ
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
void DigDisplay()                                                                            //��ʾ���ʱ����
{
   u16 i,i1;                                                                                 //��ʾ��
   i=m%10;
   i1=(m/10)%10;
   P2=0X80;
   P0=smgduan[i];
   delay(1);                                                                                 //���һ��ʱ��ɨ��
   P2=0X40;
   P0=smgduan[i1];
   delay(1);                                                                                 //���һ��ʱ��ɨ��
   P2=0X20;                                                                                         //�ָ���
   P0=0xBF;
   delay(1);
//////////////////////////////��ʾ����
   i=f%10;
   i1=(f/10)%10;
   P2=0X10;
   P0=smgduan[i];
   delay(1);                                                                                 //���һ��ʱ��ɨ��
   P2=0X08;
   P0=smgduan[i1];
   delay(1);                                                                                 //���һ��ʱ��ɨ��
   P2=0X04;          
   P0=0xBF;
   delay(1);
   //////////////////////////////��ʾСʱ
   i=s%10;
   i1=(s/10)%10;
   P2=0X02;
   P0=smgduan[i];
   delay(1);                                                                                 //���һ��ʱ��ɨ��
   P2=0X01;
   P0=smgduan[i1];
   delay(1);
///////////////////////////////
        delay(80);                                                                               //���һ��ʱ��ɨ��       
}
void DigDisplay1()                                                                           //��ʾ�����պ���
{
   u16 i,i1;                                                                                 //��ʾ��
   i=ri%10;
   i1=(ri/10)%10;
   P2=0X80;
   P0=smgduan[i];
   delay(1);                                                                                 //���һ��ʱ��ɨ��
   P2=0X40;
   P0=smgduan[i1];
   delay(1);                                                                                 //���һ��ʱ��ɨ��
   P2=0X20;                                                                                        
   P0=0xBF;
   delay(1);
//////////////////////////////��ʾ��
   i=yue%10;
   i1=(yue/10)%10;
   P2=0X10;
   P0=smgduan[i];
   delay(1);                                                                                 //���һ��ʱ��ɨ��
   P2=0X08;
   P0=smgduan[i1];
   delay(1);                                                                                 //���һ��ʱ��ɨ��
   P2=0X04;       
   P0=0xBF;
   delay(1);
   //////////////////////////////��ʾ��
   i=nian%10;
   i1=(nian/10)%10;
   P2=0X02;
   P0=smgduan[i];
   delay(1);                                                                                //���һ��ʱ��ɨ��
   P2=0X01;
   P0=smgduan[i1];
   delay(1);
///////////////////////////////
        delay(80);                                                                              //���һ��ʱ��ɨ��       
}
void Timer0Cofig(void)
{
        TMOD = 0x01;     //��ʱ��0ѡ������ʽ1
    TH0  = 0x3C;         //���ó�ʼֵ,��ʱ50MS
    TL0  = 0xB0;
    EA   = 1;             //�����ж�
    ET0  = 1;                 //�򿪶�ʱ��0�ж�
    TR0  = 1;                 //������ʱ��0       
}
void Timer0() interrupt 1
{
        TH0 = 0x3C;             //���ó�ʼֵ
        TL0 = 0xB0;
        Time++;
        if(Time == 20)
        {
                m++;
                Time = 0;
        }
}
