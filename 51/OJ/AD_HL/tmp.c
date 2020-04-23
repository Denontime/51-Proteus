#include <reg52.h>                                   //����ͷ�ļ�
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
sbit oe=P3^4;                                             //����adc0809�Ŀ�������

sbit adda=P2^0;
sbit addb=P2^1;
sbit addc=P2^2;                                           //adc0809�ĵ�ַ����

sbit clk=P3^7;                                            //adc0809ʱ������

sbit X9313W_INC  = P3^0;               //������������ˣ��½��ش���
sbit X9313W_UPDN = P3^1;               //���򣬸ߵ�ƽ�ӡ��͵�ƽ��
sbit X9313W_CS   = P3^2;               //Ƭѡ,�͵�ƽѡ�п�ʼ����

sbit LCD1602_E   = P2^3;
sbit LCD1602_RS  = P2^4;
sbit LCD1602_RW  = P2^5;
sbit LCD1602_DATAPINS = P1;

unsigned int  voltdata,realvolt1,realvolt,i,j,t,sum,a[5];         //����ȫ�ֱ���

void delay( int us)                      //��ʱ����
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


int ADC()                         //�ɼ�����
{
        sum=0;                     //�˲���ʼ
        for (i=0;i<5;i++)           
        {
                start=1;                         //����ADת����
                start=0;
                while(eoc==0);                   //�ȴ�ת��������
                oe=1;
                a[i]=P1;                         //����ת�������
                delay(50);                       //ÿ��5usADת�����ɼ�һ�����ݣ����õ�����ֵ����������
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

        voltdata=sum/3;
        delay(1);
        oe=0;
        return voltdata;
}
/***********��������*****************/

/*************************************/
/*   �������赽��RNumber����ֵ     */
/*��ڣ�RNumber������ֵ0��31         */
/*���ڣ���                           */
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
    X9313W_UPDN = 0;                    //��Ϊ��

    for(i=0;i<32;i++)
    {
        X9313W_INC = 1;
        _nop_();
        _nop_();
        X9313W_INC = 0;
        _nop_();
        _nop_();
    }                                   //����0

    X9313W_UPDN = 1;                    //��Ϊ��
    for(i=0;i<RNumber;i++){
        X9313W_INC=1;
        _nop_();
        _nop_();
        X9313W_INC=0;
        _nop_();
        _nop_();
    }                                   //�ӵ�RNumber
     
    X9313W_INC = 1;                     //����ֵ����
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
/*     ������������RNumber����ֵ   */
/*��ڣ�RNumber������ֵ0��31         */
/*���ڣ���                           */
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
    X9313W_UPDN = 1;                  //��RNumber��ָ��ֵ
    
    for(i=0;i<RNumber;i++)
		{
        X9313W_INC=1;
        _nop_();
        _nop_();
        X9313W_INC=0;
        _nop_();
        _nop_();
    }
     
    X9313W_INC = 1;                    //����ֵ����
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
/*     �����������RNumber����ֵ      */
/*��ڣ�RNumber������ֵ0��31          */
/*���ڣ���                            */
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
    X9313W_UPDN = 0;                    //��RNumber��ָ��ֵ
    for(i=0;i<RNumber;i++){
        X9313W_INC=1;
        _nop_();
        _nop_();
        X9313W_INC=0;
        _nop_();
        _nop_();
    }
     
    X9313W_INC = 1;                    //����ֵ����
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

void init()                             //��ʼ������
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
	
		EA =1;                              //��CPU���ж�
		ET0=1;                              //��ʱ��T0������ж�����
		ET1=1;                              //��ʱ��T1������ж�����
		TR0=1;
		TR1=1;                              
		EX0=1;                              //�ⲿ�ж�0����
		IT0=1;                              //�Ӹߵ��͵ĸ�������Ч
	
		start = 0;
		oe = 0;
		clk = 0;
	
		adda=0;
		addb=0;
		addc=0;
		ale  = 0;
		ale  = 1;                  //AD������·ѡ��
	
		X9313W_SetVol(15);                  //����������Ϊ�м�ֵ
		delay(1000);
}

void LcdWriteCom(uchar com)      //д������
{
      LCD1602_E = 0;         //ʹ������

      LCD1602_RS = 0;        //ѡ��������
      LCD1602_RW = 0;        //ѡ��д��

      LCD1602_DATAPINS = com;    //��������
      Lcd1602_Delayms(1);                   //�ȴ������ȶ�


      LCD1602_E = 1;             //д��ʱ��һ���½��أ�������ﵽLCD
      Lcd1602_Delayms(5);        //����ʱ��
      LCD1602_E = 0;
}

void LcdWriteData(uchar dat) //д������

{

LCD1602_E = 0; //ʹ������
LCD1602_RS = 1; //ѡ����������
LCD1602_RW = 0; //ѡ��д��
LCD1602_DATAPINS = dat; //д������
Lcd1602_Delayms(1);
LCD1602_E = 1; //д��ʱ��һ���½���
Lcd1602_Delayms(5); //����ʱ��
LCD1602_E = 0;

}

void  LCD1602_display()

{

      unsigned int i;
      unsigned char  cu[9]="Current:";


      for(i=0;i<8;i++) //��һ����ʾ��current��

     {
                LcdWriteData(cu[i]);
      }

      LcdWriteCom(0x80+0x40);

      LcdWriteData(voltdata);

}

void LcdInit()                                         //LCD��ʼ���ӳ���

{
     LcdWriteCom(0x38);  //�����趨 0011 1000 ��8λ�����ߡ���ʾ2�С�ÿ���ַ�Ϊ5*7����

      //
      LcdWriteCom(0x0c);  //��ʾ���ؿ��� 0000 1100����+��겻��˸0x0d��0000 1101��������ʾ��������ꡢ�����˸����˸��
      LcdWriteCom(0x06);  //����ģʽ���� 0000 0110��д�����ݺ�������һλ����ʾ��������
      LcdWriteCom(0x01);  //����
      LcdWriteCom(0x80);  //��������ָ����� //DB7Ϊ�ߵ�ƽ

}



void main()                               //������
{
		init();
		LcdInit();
        while(1)                          //����whileѭ��
        { 
					ADC();
					if(voltdata>128)                //�ж�����
					{
					       X9313W_IncVol(voltdata-128);               //���������
					       LCD1602_display();                         //��ʾ
					}
					else
					{
					       X9313W_DecVol(128-voltdata);               //��С������
					       LCD1602_display();                         //��ʾ
					}
        }                           
}

void timer0() interrupt 1          // ���500kHz ���� 
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
