#include<reg52.h>                                   //����ͷ�ļ�
#include <math.h>
#include "uart.h"
#include "display.h"
#define uint unsigned int 
#define uchar unsigned char                   //�궨��
sbit ale=P2^6;
sbit start=P2^7;
sbit eoc=P3^2;
sbit oe=P3^3;                                           //����adc0809�Ŀ�������
sbit adda=P2^0;
sbit addb=P2^1;
sbit addc=P2^2;                                           //adc0809�ĵ�ַ����
//sbit rs=P2^3;
//sbit rw=P2^4;
//sbit en=P2^5;                                           //Һ���Ŀ�������
sbit key1=P3^4;
sbit key2=P3^5;
unsigned long voltdata[8]; 
uint realvolt,i,j,k,l,t,sum,a[5];         //����ȫ�ֱ���
unsigned char CH;
uchar add;                                                                         //��ַ����
uchar TxBuf[8];                //�ĸ���ʾ��λ
unsigned char t0Count;
unsigned char flagAu=1;
uchar code t1[]={"DC Voltmeter:IN "};
uchar code t2[]={" (0~20V):  .  V "};
uchar code t3[]={"      Hello!    "};
uchar code t4[]={"     Welcome!   "};                 //��ʼ����ʾ

/*
����˵��
*/ 
void addselect()                //����ѡ��
{
  CH++;                                                //ͨ���ż�
  if(CH>7)                                        //���7                  ����&����λ����ţ������ƵĶ�Ӧλ�����Ϊ1����ô����Ķ�Ӧλ��Ϊ1������Ϊ0
  CH=0;                                                //���㣬һ���˸�ͨ��0 1 2 3 4 5 6 7����Ӧ��������0000.0001.0010.0011.0100.0101.0110.0111
  adda=CH&0x01;                                //����5ͨ�����㷽������ͨ����Ӧ�Ķ�����0101&0001���������0001����ֵ��adda������ֻ�ܽ������λ�����ݣ�ֻ��1��0������ôadda���ž��Ǹߵ�ƽ
  addb=(CH&0x02)>>1;                //Ȼ��ͨ��5�Ķ�����0101&0010���������0000����Ϊ����λ����ͬ����Ȼ��������������һλ����ֵ��addb����ôaddb���ž��ǵ͵�ƽ
  addc=(CH&0x04)>>2;                 //Ȼ��ͨ��5�Ķ�����0101&0100���������0100��Ȼ����������������λ����ֵ��addc����ôaddc���ž��Ǹߵ�ƽ�����оƬ��ͨ��ѡ�����ŵ�״̬����101����ѡ��ľ���ͨ��5��
  delay(5);                                        //����ʱ
}
uint adtrans()                         //ADת��
{
        addselect();
        sum=0;                     //�˲���ʼ
        for (i=0;i<5;i++)           
        {
                ale=1;
                ale=0;
                start=1;                         //����ADת����
                start=0;
                while(eoc==0);                 //�ȴ�ת��������
                oe=1;
                a[i]=P1;             //����ת�������
                delay(5);                  ////ÿ��5usADת�����ɼ�һ�����ݣ����õ�����ֵ����������
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
        }                                  //����ð�ݷ��Ƚϣ�ȡ�м�3����ֵ
        voltdata[CH]=sum/3;
        
        delay(1);
        oe=0;
        voltdata[CH]=4*(100*voltdata[CH])/51;                   //������������
        //voltdata[CH]�Ƕ�ȡ����adֵ������100����С���������λ������4�����������ı�Ҳ����5*4=20V��Ȼ�����51���ͳ���0.0196һ�����õ�ʵ�ʵĵ�ѹֵ
        //8λAD��С�ֱ��ʾ���0.0196V
        return voltdata[CH];
}
void disvolt()                                //��ʾ����        
{
        realvolt=voltdata[add];
        writelcd_cmd(0x80);
        writelcd_dat('1'+add);
        TxBuf[1]=realvolt/1000+0x30;
        TxBuf[2]=realvolt/100%10+0x30;
        TxBuf[4]=realvolt/10%10+0x30;
        TxBuf[5]=realvolt%10+0x30;                 //����ѹֵ�ֳ�4��һλ����������ʾ
        writelcd_cmd(0x82);           //��ʾ��ѹֵ
        writelcd_dat(TxBuf[1]);
        writelcd_dat(TxBuf[2]);
        writelcd_cmd(0x85);
        writelcd_dat(TxBuf[4]);
        writelcd_dat(TxBuf[5]);
        realvolt=voltdata[add+1];
        writelcd_cmd(0x88);
        writelcd_dat('1'+add+1);
        TxBuf[1]=realvolt/1000+0x30;
        TxBuf[2]=realvolt/100%10+0x30;
        TxBuf[4]=realvolt/10%10+0x30;
        TxBuf[5]=realvolt%10+0x30;                 //����ѹֵ�ֳ�4��һλ����������ʾ
        writelcd_cmd(0x8a);           //��ʾ��ѹֵ
        writelcd_dat(TxBuf[1]);
        writelcd_dat(TxBuf[2]);
        writelcd_cmd(0x8d);
        writelcd_dat(TxBuf[4]);
        writelcd_dat(TxBuf[5]);

        realvolt=voltdata[add+2];
        writelcd_cmd(0xc0);
        writelcd_dat('1'+add+2);
        TxBuf[1]=realvolt/1000+0x30;
        TxBuf[2]=realvolt/100%10+0x30;
        TxBuf[4]=realvolt/10%10+0x30;
        TxBuf[5]=realvolt%10+0x30;                 //����ѹֵ�ֳ�4��һλ����������ʾ
        writelcd_cmd(0xc2);           //��ʾ��ѹֵ
        writelcd_dat(TxBuf[1]);
        writelcd_dat(TxBuf[2]);
        writelcd_cmd(0xc5);
        writelcd_dat(TxBuf[4]);
        writelcd_dat(TxBuf[5]);
        realvolt=voltdata[add+3];
        writelcd_cmd(0xc8);
        writelcd_dat('1'+add+3);
        TxBuf[1]=realvolt/1000+0x30;
        TxBuf[2]=realvolt/100%10+0x30;
        TxBuf[4]=realvolt/10%10+0x30;
        TxBuf[5]=realvolt%10+0x30;                 //����ѹֵ�ֳ�4��һλ����������ʾ
        writelcd_cmd(0xca);           //��ʾ��ѹֵ
        writelcd_dat(TxBuf[1]);
        writelcd_dat(TxBuf[2]);
        writelcd_cmd(0xcd);
        writelcd_dat(TxBuf[4]);
        writelcd_dat(TxBuf[5]);
}
void anjian()                                //����ɨ�躯��
{

        if(key1==0)                                //��һ����������
        {
                 delay(20);
                if(key1==0)
                {
                 if(add==0)
                 add=4;
                 else
                 add=0;
                 flagAu=0;
                 while(key1==0);
                }
        }
        if(key2==0)
        {
                 delay(20);
                if(key2==0)
                {
                 
             flagAu=1;
                 while(key2==0);
                }
        }
                                         //�л���λ
        TxBuf[6]=CH+0x31;

}
void init()                                         //��ʼ������
{

        add=0;                                
                addselect();
        lcd_init();                                //Һ����ʾ��ʼ��
    write_string(0x80,"1:  .  V2:  .  V");
        write_string(0xc0,"3:  .  V4:  .  V");
        TxBuf[0]='


;
        TxBuf[3]='.';
        TxBuf[7]='\0';
}
void main()                                  //������
{
        init();                                  //���ó�ʼ������
        Uart_init();
        while(1)                          //����whileѭ��
        {   
                anjian();                  //ɨ�谴��
                adtrans();                  //ad����
                disvolt();                  //��ʾ��ѹ
        }                           
}
 void timer0() interrupt 1
 {
  TH0=0x4c;
  TL0=0x00;
  t0Count++;
  if(t0Count>=40)
  {
   t0Count=0;
   if(flagAu==1)
   {
    if(add==0)
        add=4;
        else
        add=0;
   }
  }
 }
