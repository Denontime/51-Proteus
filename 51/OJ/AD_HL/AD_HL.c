//////////////////////////////////////////////////////////////////////////////////////////////
//ad������ѹֵ��0--5.00v��,ʹ��AD0809оƬ
//////////////////////////////////////////////////////////////////////////////////////////////
#include "reg52.h"

//AD0809���
sfr ADDR = P2;      //ͨ����ַ
sbit START = P3^4;  //����ת��
sbit EOC = P1^7;    //ת��������־
sbit OE = P3^2;    //���ʹ��
sbit ALE = P3^0;    //��ַ����

unsigned char ad_value=0 ;				//ԭʼadֵ

void delay100us(int us) //��ʱ100us�ӳ���
{
unsigned int i;
unsigned char j;
for(i=0;i<us;i++)
{ 
for(j=0;j<2;j++);
for(j=0;j<102;j++);
}
}


void delay_short(void)//����ʱ
{
unsigned char i=10;
while(i--);
}



void all_init(void);       //ȫ����ʼ��
void ad_samp(void);        //ad��������


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
//100us��ʱ����ʼ��
TMOD = 0x02;
TH0 = 0x00;
TL0 = 0x9c;
//���ж�
EA = 1;
ET1 = 1;
TR1 = 1;
}


void ad_samp(void)
{
ADDR=0x00;//ѡ��IN0��ͨ��
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
