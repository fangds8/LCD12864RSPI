//Demo LCD12864 spi
//www.dfrobot.com
//edit by savage at 2014-02-28

#include "LCD12864RSPI.h"

#include "Arduino.h"
#include <wiring.h> 

LCD12864RSPI::LCD12864RSPI(int _latchPin,int _dataPin,int _clockPin):
	latchPin(_latchPin),dataPin(_dataPin),clockPin(_clockPin)
{

    pinMode(latchPin, OUTPUT);     
    pinMode(clockPin, OUTPUT);    
    pinMode(dataPin, OUTPUT);
    digitalWrite(latchPin, LOW);
    WriteCommand(0x30);        //�����趨������
    WriteCommand(0x0c);        //��ʾ���ؿ�����
    WriteCommand(0x01);        //�����Ļ������
    WriteCommand(0x06);        //�����趨�������

} 

//��дwrite
inline size_t LCD12864RSPI::write(uint8_t value) {
	WriteData(value);
}


void LCD12864RSPI::WriteByte(uint8_t dat)
{
    digitalWrite(latchPin, HIGH);
    shiftOut(dataPin, clockPin, MSBFIRST, dat);
    digitalWrite(latchPin, LOW);
}


void LCD12864RSPI::WriteCommand(uint8_t CMD)
{
   int H_data,L_data;
   H_data = CMD;
   H_data &= 0xf0;           //���ε�4λ������
   L_data = CMD;             //xxxx0000��ʽ
   L_data &= 0x0f;           //���θ�4λ������
   L_data <<= 4;             //xxxx0000��ʽ
   WriteByte(0xf8);          //RS=0��д�����ָ�
   WriteByte(H_data);
   WriteByte(L_data);
}


void LCD12864RSPI::WriteData(uint8_t CMD)
{
   int H_data,L_data;
   H_data = CMD;
   H_data &= 0xf0;           //���ε�4λ������
   L_data = CMD;             //xxxx0000��ʽ
   L_data &= 0x0f;           //���θ�4λ������
   L_data <<= 4;             //xxxx0000��ʽ
   WriteByte(0xfa);          //RS=1��д���������
   WriteByte(H_data);
   WriteByte(L_data);
}


void LCD12864RSPI::clear(void)
{  
    WriteCommand(0x30);//
    WriteCommand(0x01);//�����ʾ
    delay(1);//ָ����Ҫʱ�䷴Ӧ
}

void LCD12864RSPI::chinese(uchar *ptr,int dat)
{
  int i;
  for(i=0;i<dat;i++)
    { 
      WriteData(ptr[i]);//��ʾ����ʱע����ֵ�������������ʾһ������
    }
}



void LCD12864RSPI::setCursor(uint8_t X,uint8_t Y)
{
  switch(X)
   {
     case 0:  Y|=0x80;break;

     case 1:  Y|=0x90;break;

     case 2:  Y|=0x88;break;

     case 3:  Y|=0x98;break;

     default: break;
   }
  WriteCommand(Y); // ��λ��ʾ��ʼ��ַ
}


void LCD12864RSPI::DrawFullScreen(uchar *p)
{
      int ygroup,x,y,i;
      int temp;
      int tmp;
             
      for(ygroup=0;ygroup<64;ygroup++)           //д��Һ���ϰ�ͼ�󲿷�
        {                           //д������
           if(ygroup<32)
            {
             x=0x80;
             y=ygroup+0x80;
            }
           else 
            {
              x=0x88;
              y=ygroup-32+0x80;    
            }         
           WriteCommand(0x34);        //д������ָ������
           WriteCommand(y);           //д��y������
           WriteCommand(x);           //д��x������
           WriteCommand(0x30);        //д�����ָ������
           tmp=ygroup*16;
           for(i=0;i<16;i++)
		 {
		    temp=p[tmp++];
		    WriteData(temp);
               }
          }
        WriteCommand(0x34);        //д������ָ������
        WriteCommand(0x36);        //��ʾͼ��
}