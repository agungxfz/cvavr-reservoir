/*****************************************************
This program was produced by the
CodeWizardAVR V2.05.3 Standard
Automatic Program Generator
� Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 26/02/2016
Author  : tyery08
Company : embeeminded.blogspot.com
Comments: 


Chip type               : ATmega16
Program type            : Application
AVR Core Clock frequency: 11,059200 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*****************************************************/

#include <mega16.h>

// Alphanumeric LCD functions
#include <alcd.h>
#include <delay.h>
// Declare your global variables here
#include <stdio.h>

#define trigger PORTA.1
#define echo PINA.0
#define rel1 PORTC.0
#define rel2 PORTC.1
#define rel3 PORTC.2
#define mot3 PORTC.3
#define mot2 PORTC.4
#define mot1 PORTC.5

#define swa1 PINB.0
#define swb1 PINB.1
#define swa2 PINB.2
#define swb2 PINB.3
#define swa3 PIND.4
#define swb3 PIND.5

unsigned char dataUS, levelAir;
int i;
char lcd[16],lcdChar[16];
unsigned int count = 0;
unsigned char jarak;

void SMSReport()
{
    printf("AT+CMGS=085649051005\r");//085649051005 putchar(13);
	printf("Laporan\rKetinggian air sekarang : %d, Level : %d",32 - dataUS, levelAir);
	putchar(26);
	delay_ms(300);
}
unsigned char Read_SRF04()
{
    count=0;
    // pemberian pemicu berupa pulsa positif minimal 10us
    jarak=0;
    delay_us(100);
    trigger=1;
    delay_us(15);
    trigger=0;
    delay_us(100);
    while(!echo);
        for (i=0;i<=500;i++)
        {
            if (echo) 
            {  
                jarak++;
            }
        delay_us(54);
        }
    return jarak;
}

void main(void)
{
// Declare your local variables here

// Input/Output Ports initialization
// Port A initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=Out Func0=In 
// State7=T State6=T State5=T State4=T State3=T State2=T State1=0 State0=P 
PORTA=0x01;
DDRA=0x02;

// Port B initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=T State4=T State3=P State2=P State1=P State0=P 
PORTB=0x0F;
DDRB=0x00;

// Port C initialization
// Func7=Out Func6=Out Func5=Out Func4=Out Func3=Out Func2=Out Func1=Out Func0=Out 
// State7=0 State6=0 State5=0 State4=0 State3=0 State2=0 State1=0 State0=0 
PORTC=0x00;
DDRC=0xFF;

// Port D initialization
// Func7=In Func6=In Func5=In Func4=In Func3=In Func2=In Func1=In Func0=In 
// State7=T State6=T State5=P State4=P State3=T State2=T State1=T State0=T 
PORTD=0x30;
DDRD=0x00;

// USART initialization
UCSRA=0x00;
UCSRB=0x18;
UCSRC=0x86;
UBRRH=0x00;
UBRRL=0x05;

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTD Bit 6
// RD - PORTD Bit 3
// EN - PORTD Bit 2
// D4 - PORTB Bit 4
// D5 - PORTB Bit 5
// D6 - PORTB Bit 6
// D7 - PORTB Bit 7
// Characters/line: 16
lcd_init(16);

levelAir = 0;
lcd_clear();
lcd_gotoxy(0,0);
lcd_puts("   Loading...   ");

for(i=0;i<=15;i++)
{
    lcd_gotoxy(i,1);
    lcd_putchar(255); 
    delay_ms(100);
} 

lcd_clear();

while (1)
      {
      int countSMS = 0;
      // Membaca data SFR04
      dataUS = Read_SRF04();
      
      // Menampilkan tinggi air
      lcd_gotoxy(0,1);
      sprintf(lcd,"Tinggi %d cm  ",32 - dataUS);
      lcd_puts(lcd);
      
      // Konversi nilai tinggi ke nilai level
      if (dataUS > 0 && dataUS <=8)
      { 
        levelAir = 3;
        if(countSMS == 0){
            SMSReport();
            countSMS = 1;
        } else if(countSMS == 1){
            countSMS = 0;
        }
      }      
	  else if (dataUS > 9 && dataUS <= 16)
	  {
        levelAir = 2;
        if(countSMS == 0){
            SMSReport();
            countSMS = 1;
        } else if(countSMS == 1){
            countSMS = 0;
        }
      }
	  else if (dataUS > 17 && dataUS <= 24)
	  {
        levelAir = 1;
        if(countSMS == 0){
            SMSReport();
            countSMS = 1;
        } else if(countSMS == 1){
            countSMS = 0;
        }
      }      
	  else if (dataUS > 25)
      {
        levelAir = 0;
      }
      
      // Menampilkan level air      
      lcd_gotoxy(0,0);
      sprintf(lcdChar,"Level: %d ",levelAir);
      lcd_puts(lcdChar);
      
      if(levelAir == 0)
      {
        mot2 = 0; rel2 = 0;
        mot3 = 0; rel3 = 0;
      }
      
      //=============level 1=============//
      if (levelAir == 1)
      {
        mot1 = 1; rel1 = 1;
        if(!swb1)
        {
            mot1 = 0; rel1 = 0;
        }  
      } else if (levelAir == 0 && !swb1)
      {
        mot1 = 0; rel1 = 1;
      } else if(!swa1)
      {
        mot1 = 0; rel1 = 0;
      }
      
      //=============level 2=============//
      if(levelAir == 2)
      {
        mot2 = 1; rel2 = 1;
        if(!swb2)
        {
            mot2 = 0; rel2 = 0;
        }  
      } else if(levelAir == 1 && !swb2)
      {
        mot2 = 0; rel2 = 1;
      } else if(!swa2)
      {
        mot2 = 0; rel2 = 0;
      }
      
      //=============level 3=============//
      if(levelAir == 3)
      {
        mot3 = 1; rel3 = 1;
        if(!swb3)
        {
            mot3 = 0; rel3 = 0;
        }  
      } else if(levelAir == 2 && !swb3)
      {
        mot3 = 0; rel3 = 1;        
      } else if(!swa3)
      {
            mot3 = 0; rel3 = 0;
      }
      }
}
