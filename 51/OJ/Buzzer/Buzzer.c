#include <reg52.h>

#define uint unsigned int
#define uchar unsigned char

void delay_ms(uint);//mS����������ʱ��������

sbit Buzzer=P1^5;

void main()
{
    while(1)
    {
        Buzzer = 1;
        delay_ms(500);
        Buzzer = 0;
    }
}

void delay_ms(uint z) //��ʱ�ӳ���

{

    uint x,y;

    for(x=z; x>0; x--)

        for(y=110; y>0; y--);

}