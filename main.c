		 
#include "reg51.h" 
#include <intrins.h>
#define uchar unsigned char 
#define uint unsigned int

uchar code normal[]="normal  ";
uchar code abnormal[]="abnormal";

sbit ST_ALE=P3^0;   //START and ALE both connect to P3.0 

sbit EOC=P3^1;   //EOC connect to P3.1      

sbit OE=P3^2;    //OE conect to P3.2 

sbit CLK=P3^3; 

sbit lcdrs=P2^4;
sbit lcdrw=P2^5;
sbit lcden=P2^6;

sbit beep=P3^4;
sbit led=P3^5;

unsigned char CHANNEL; //AD channel variable 
uchar code num[]="0123456789";
 /*
void delay(unsigned char time) 

{ 

 unsigned char i,j; 

 for(i=time;i>0;i--) 

  for(j=250;j>0;j--)  ; 

} 
*/
 void delay(uint x)
{
	uint i;
	for(;x>0;x--)
		for(i=110;i>0;i--);	
}

void write_com(uchar com )
{
	lcden = 0;
	lcdrs = 0;//±íÊ¾ÃüÁîÑ¡Ôñ
	P1 = com;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;
		
}


void write_data(uchar date)
{
	lcden = 0;
	lcdrs = 1;//n.1 write into the data;
	P1 = date;
	delay(5);
	lcden=1;
	delay(5);
	lcden=0;	
}
 
void init()
{  
	lcden=0;
	lcdrw=0;
	write_com(0x38);//lcdÏÔÊ¾Ä£Ê½£¬16x02£»8½Ó¿Ú
	write_com(0x0c);//¿ªÏÔÊ¾£¬ÏÔÊ¾¹â±ê£¬¹â±êÉÁË¸
	write_com(0x06);//µØÖ·×Ô¼Ó£¬×Ö·û²»ÕûÆÁÒÆ¶¯£»
	write_com(0x01);//ÇåÆÁ£» 

}

void display(uchar channel) 

{ 

 unsigned long temp; 

 unsigned char DATA,count,digit[4]; 

 unsigned int k=0,i=0; 

 DATA=P0;     //save AD data 

 temp=DATA;    //the temporary variable 

 temp=temp*1000/51;   //AD formula:D=A*5/255  
 
 digit[0]=temp/1000;   //the thousands place 

 digit[1]=temp%1000/100;     //the hundreds place 

 digit[2]=temp%1000%100/10;  //the tens place 

 digit[3]=temp%1000%100%10; //the ones place 

	if(channel==0)
	{
	
  	write_com(0x80);
   	write_data('V');
		write_data('1');
		write_data('=');
		write_data(num[digit[0]]);
		write_data('.');
		write_data(num[digit[1]]);
		write_data(num[digit[2]]);
		write_data(num[digit[3]]);
		write_com(0x80+0x08);
		if(DATA>0x40)
		{
				
			for(count=0;count<8;count++)
			{
				write_data(abnormal[count]);
			}
			beep=1;
		}
		else 
		{
				
			for(count=0;count<8;count++)
			{
				write_data(normal[count]);
			}
		    beep=0;	
		}
	}
	if(channel==1)
	{
  	write_com(0x80+0x40);
   	write_data('V');
	write_data('2');
	write_data('=');
	write_data(num[digit[0]]);
	write_data('.');
	write_data(num[digit[1]]);
	write_data(num[digit[2]]);
	write_data(num[digit[3]]);
	write_com(0x80+0x40+0x08);
		if(DATA>0x80)
		{
				
			for(count=0;count<8;count++)
			{
				write_data(abnormal[count]);
			}
			led=1;
		}
		else 
		{
				
			for(count=0;count<8;count++)
			{
				write_data(normal[count]);
			}
		    led=0;
		}
	}


}  

 

void main() 

{ 

 CHANNEL=0xF8;  //AD channel 0 

 ST_ALE=0;   //START and ALE default 

 OE=0;    //OE default 

 TMOD=0x02;  //T0 operate in mode 2  

 TH0=0xFF;   //auto load value 

 TL0=0xFF;   //default value 

 EA=1;    //all interrupt enable 

 ET0=1;    //T0 interrupt enable 

 TR0=1;    //T0 run 

 init();

 beep=0;
 led=0;

 while(1) 

 {  
 
  P2=0x07&CHANNEL;  //choose AD channel 

  ST_ALE=1;  //lock the AD channel address 

  ST_ALE=0;  //AD run 

  while(EOC==0); //wait until AD over 

  OE=1;   //enable to transmit AD data 

  if(CHANNEL==0xF8)
  {
  display(0);   //view the AD result and diaplay LED  
  }
  if(CHANNEL==0xF9)
  {
  display(1);   //view the AD result and diaplay LED  
  }

  delay(50);     //delay to prepare for next AD channel  

  OE=0;          //forbit to transmit AD data 

  
  if(CHANNEL==0xF9) 

  CHANNEL=0xF7;             //AD channel loop 

  CHANNEL++;                //next AD channel 
  
 }  

} 

 

void T1_TIME() interrupt 1 using 0 

{ 

 CLK=~CLK;   //CLK 500 kHz 

} 








