#include <reg52.h>                                   //����ͷ�ļ�
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
//sbit oe=P3^4;                                             //����adc0809�Ŀ�������
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
//sbit addc=P2^2;                                           //adc0809�ĵ�ַ����

//sbit clk=P3^7;                                            //adc0809ʱ������

sbit X9313W_INC  = P3^0;               //������������ˣ��½��ش���
sbit X9313W_UPDN = P3^1;               //���򣬸ߵ�ƽ�ӡ��͵�ƽ��
sbit X9313W_CS   = P3^2;               //Ƭѡ,�͵�ƽѡ�п�ʼ����

# define LCD_DB        P1
        sbit         LCD_RS=P2^4;
        sbit         LCD_RW=P2^5;
        sbit         LCD_E=P2^3;
/************************************************************************************************************/

unsigned int  real,i,j,t,sum,a[5];         //����ȫ�ֱ���
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
void LCD_init(void);//��ʼ������
void LCD_write_command(uchar command);//дָ���
void LCD_write_data(uchar dat);//д���ݺ���
//void LCD_disp_char(uchar x,uchar y,uchar dat);//��ĳ����Ļλ������ʾһ���ַ�,X��0-16),y(1-2)
void delay_n40us(uint n);//��ʱ����
//********************************
//*******��ʼ������***************
void LCD_init(void)
{
LCD_write_command(0x38);//����8λ��ʽ��2�У�5x7
LCD_write_command(0x0c);//������ʾ���ع�꣬����˸
LCD_write_command(0x06);//�趨���뷽ʽ����������λ
LCD_write_command(0x01);//�����Ļ��ʾ
delay_n40us(100);
}
//********************************
//********дָ���************
void LCD_write_command(uchar dat)
{
LCD_DB=dat;
LCD_RS=0;//ָ��
LCD_RW=0;//д��
LCD_E=1;//����
LCD_E=0;
delay_n40us(1);//ʵ��֤�����ҵ�LCD1602�ϣ���forѭ��1�ξ��������ͨдָ�
}
//*******************************
//********д���ݺ���*************
void LCD_write_data(uchar dat)
{
LCD_DB=dat;
LCD_RS=1;//����
LCD_RW=0;//д��
LCD_E=1;//����
LCD_E=0;
delay_n40us(1);
}
//********************************
/*******��ʾһ���ַ�����*********
void LCD_disp_char(uchar x,uchar y,uchar dat)  ////���������У��С�0��һ�У�1�ڶ��С����ַ���
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
//���������У��С�0�ڶ��У�1��һ�С����ַ������ַ������ȣ�
{
unsigned char i;   
    for(i=0;i<count;i++)
    {
		uchar address;
        if(y==1)  
			address=0x80+x; //��Ҫ��ʾ��һ��ʱ��ַ��+0x80;
        else 
			address=0xc0+x;; //�ڵڶ�����ʾ�ǵ�ַ��+0xC0;

        LCD_write_command(address);
		LCD_write_data(*p); //���͵�ַ��
                            //����Ҫ��ʾ���ַ�����
        x++;
        p++;
    }       
}   


/******************************/
//********��ʱ����***************
void delay_n40us(uint n)
{ uint i;
      uchar j;            
        for(i=n;i>0;i--)
           for(j=0;j<2;j++);          //�������ʱѭ����������ֻ����2��ѭ����
}                                         //ʵ��֤���ҵ�LCD1602����ͨ��ָ��ֻ��1��ѭ�����ܿɿ���ɡ�

/************************************************************************************************************/


void delay( int us)                      //��ʱ����
{
	int aus;
	for(aus=0;aus<us;aus++)
		_nop_();
}


void ADC()                         //�ɼ�����
/*{
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


/***********��������*****************/

/*************************************/
/*   �������赽��RNumber����ֵ     */
/*��ڣ�RNumber������ֵ0��31         */
/*���ڣ���                           */
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

*/

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
		ale  = 1;                  //AD������·ѡ��
	*/
		//X9313W_SetVol(15);                  //����������Ϊ�м�ֵ
	
		delay(1000);
}

void main()                               //������
{
		init();             //��ʼ��
		LCD_init();         //��ʼ��Һ����
		DisplayListChar(0,1,"Current:",8);    //д���һ����Ϣ����ѭ���в��ٸ��Ĵ���Ϣ��������while֮ǰд��
    //DisplayListChar(8,0,tem,5);
    //DisplayListChar(7,0,".",1);
    //DisplayListChar(8,0,cu,4);
    DisplayListChar(14,0,"mA",2);
        while(1)                          //����whileѭ��
        { 
					ADC();
					DisplayListChar(8,0,tem,5);
					DisplayListChar(14,0,"mA",2);
					/*if(voltdata>128)                //�ж�����
					{
					       //X9313W_IncVol(voltdata-128);               //���������
					       DisplayListChar(3,0,tem,12);                //��ʾ��Һ����
					}
					else
					{
					       //X9313W_DecVol(128-voltdata);               //��С������
					       DisplayListChar(3,0,tem,12);                //��ʾ��Һ����
					}*/
        }                           
}

/*void timer0() interrupt 1          // ���500kHz ���� 
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
 
