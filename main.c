/*
 * File:   main.c
 * Author: k-soeda
 *
 * Created on 2019/01/17, 8:48
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>


#define _XTAL_FREQ  16000000

#pragma config FOSC = INTOSC    // Oscillator Selection->INTOSC oscillator: I/O function on CLKIN pin
#pragma config WDTE = OFF    // Watchdog Timer Disable
#pragma config PWRTE = ON    // Power-up Timer Enable
#pragma config MCLRE = OFF    // MCLR Pin Function Select->MCLR/VPP pin function is digital input
#pragma config CP = OFF    // Flash Program Memory Code Protection->Program memory code protection is disabled
#pragma config CPD = OFF    // Data Memory Code Protection->Data memory code protection is disabled
#pragma config BOREN = ON    // Brown-out Reset Enable->Brown-out Reset enabled
#pragma config CLKOUTEN = OFF    // Clock Out Enable->CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin
#pragma config IESO = ON    // Internal/External Switchover->Internal/External Switchover mode is enabled
#pragma config FCMEN = ON    // Fail-Safe Clock Monitor Enable->Fail-Safe Clock Monitor is enabled

// CONFIG2
#pragma config WRT = OFF    // Flash Memory Self-Write Protection->Write protection off
#pragma config VCAPEN = OFF    // Voltage Regulator Capacitor Enable->All VCAP pin functionality is disabled
#pragma config PLLEN = OFF    // PLL Enable->4x PLL disabled
#pragma config STVREN = ON    // Stack Overflow/Underflow Reset Enable->Stack Overflow or Underflow will cause a Reset
#pragma config BORV = LO    // Brown-out Reset Voltage Selection->Brown-out Reset Voltage (Vbor), low trip point selected.
#pragma config LVP = OFF    // Low-Voltage Programming Enable->High-voltage on MCLR/VPP must be used for programming


// 2 phase excitation dimension
int L_motor_cw_MP50[]    = {0x09,0x0B,0x0A,0x0E,0x06,0x07,0x05,0x0D};  //??
int L_motor_ccw_MP50[]   = {0x0D,0x05,0x07,0x06,0x0E,0x0A,0x0B,0x09};  //??



int L_motor_cw_NPM[]    = {0xf5,0xf9,0xfA,0xf6};  //??
int L_motor_ccw_NPM[]   = {0xf6,0xfA,0xf9,0xf5};  //??

/////
int pps,motor,a,rot;    //motor 1 NPM 2 ???? 3 FAN

 int i=0;//thu    
 
 ////////////
 //cac chan cua LCD duoc ket noi voi vi dieu khien
#define RS RA7                                    //RS noi vao RD1
#define EN RA6                                    //EN noi vao RD0
#define D4 RC0
#define D5 RC1                                    //D5 noi vao RD5
#define D6 RC2                                    //D6 noi vao RD6
#define D7 RC3                                    //D7 noi vao RD7
#define LCD_TRIS TRISC                  
/////


void OSCILLATOR_Initialize(void);
void wait_ms(int);
void Lcd_Port(char);
void Lcd_Cmd(char);
void Lcd_Set_Cursor(char , char );
void Lcd_Init();
void Lcd_Write_Char(char );
void Lcd_Shift_Right();
void Lcd_Shift_Left();
void Lcd_Clear();
void Lcd_Write_String(char *);
void inputpps();
void CW(int);
void CCW(int );
void motorset();
void CW_sensor(int );
void CCW_sensor(int );
void pps_set(int );
void star();
void offmotor(void);
void sensor();
void main(void) 
{
    while(1)
    {
    star();
    }
}
void wait_ms(int t)
{//delay function
    
    while(t){
        __delay_ms(1);
        t = t - 1;
    }
}
void pps_set(int p)
{
    pps = 1000 / p;
}
void OSCILLATOR_Initialize(void)
{
    // SCS FOSC; SPLLEN disabled; IRCF 16MHz_HF; 
    OSCCON = 0x78;
    // TUN 0; 
    OSCTUNE = 0x00;
    // SBOREN disabled; 
    BORCON = 0x00;
    
    //pin input  output
    TRISA = 0b00101111;         //setting I/O Ports , input or output, output 0 input 1
    
   TRISB = 0b11000000;         //setting I/O Ports , input or output
   
    TRISC = 0b11110000;         //setting I/O Ports , input or output
    
    ANSELA = 0x00;
     
    ANSELB = 0x00;
    
    OPTION_REGbits.nWPUEN = 0;
 
    //PORTA = 0xFF;               
    //PORTB = 0xFF;              
   // PORTC = 0x00;   
}

void Lcd_Port(char a)
{
	if(a & 1)
		D4 = 1;
	else
		D4 = 0;

	if(a & 2)
		D5 = 1;
	else
		D5 = 0;

	if(a & 4)
		D6 = 1;
	else
		D6 = 0;

	if(a & 8)
		D7 = 1;
	else
		D7 = 0;
}
void Lcd_Cmd(char a)
{
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
        __delay_ms(4);
        EN  = 0;             // => E = 0
}

void Lcd_Clear()
{
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b)
{
	char temp,z,y;
	if(a == 1)
	{
	  temp = 0x80 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
}

void Lcd_Init()
{
  Lcd_Port(0x00);
   __delay_ms(20);
  Lcd_Cmd(0x03);
	__delay_ms(5);
  Lcd_Cmd(0x03);
	__delay_ms(11);
  Lcd_Cmd(0x03);
  /////////////////////////////////////////////////////
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x08);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x0C);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x06);
}

void Lcd_Write_Char(char a)
{
   char temp,y;
   temp = a&0x0F;
   y = a&0xF0;
   RS = 1;             // => RS = 1
   Lcd_Port(y>>4);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
   Lcd_Port(temp);
   EN = 1;
   __delay_us(40);
   EN = 0;
}

void Lcd_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
}
void inputpps()
{
    if(RC4==0&&RC5==0)
    {
        pps=100;
        Lcd_Set_Cursor(1,1);
         Lcd_Write_String("V=100pps");
    }
    else if(RC4==1&&RC5==0)
    {
        pps=150;
        Lcd_Set_Cursor(1,1);
         Lcd_Write_String("V=150pps");
    }
    else if(RC4==0&&RC5==1)
    {
        pps=200;
        Lcd_Set_Cursor(1,1);
         Lcd_Write_String("V=200pps");
    }
    else if(RC4==1&&RC5==1)
    {
        pps=250;
        Lcd_Set_Cursor(1,1);
         Lcd_Write_String("V=250pps");
    }
}
void motorset()
{
    if(RC6==1&&RC7==0)
    {
        motor=1;
        Lcd_Set_Cursor(1,10);
        Lcd_Write_String("M=NPM");
    }
    else if(RC6==0&&RC7==1)
    {
        motor=2;
         Lcd_Set_Cursor(1,10);
        Lcd_Write_String("M=MP50");
    }
    else if(RC6==1&&RC7==1)
    {
        motor=3;
        Lcd_Set_Cursor(1,10);
        Lcd_Write_String("M=FAN");
    }
}
void CW(int a)
{
     if(a==1)
    {
    int i = 0; 
    while(RA0==0&&RA5==0)
    {
        PORTB = L_motor_cw_NPM[i];
        wait_ms(pps);
        i = i + 1;
        if(i == 4){
            i = 0;       
    }
        Lcd_Set_Cursor(2,13);
        Lcd_Write_String("CW.");
    }
    }
    else if(a=2)
    {
    int i = 0; 
    while(RA0==0&&RA5==0)
    {
        PORTB = L_motor_cw_MP50[i];
        wait_ms(pps);
        i = i + 1;
        if(i == 8){
            i = 0;       
    }
         Lcd_Set_Cursor(2,13);
        Lcd_Write_String("CW.");
    }  
    }
     
     
}
void CCW(int a)
{
    if(a==1)
    {
    int i = 0; 
    while(RA1==0&&RA5==0)
    {
        PORTB = L_motor_ccw_NPM[i];
        wait_ms(pps);
        i = i + 1;
        if(i == 4){
            i = 0;       
    }
        Lcd_Set_Cursor(2,13);
        Lcd_Write_String("CCW");
    }
    }
    else if(a=2)
    {
    int i = 0; 
    while(RA1==0&&RA5==0)
    {
        PORTB = L_motor_ccw_MP50[i];
        wait_ms(pps);
        i = i + 1;
        if(i == 8){
            i = 0;       
    }
         Lcd_Set_Cursor(2,13);
        Lcd_Write_String("CCW");
    }  
    }
    
}
void CW_sensor(int a)
{
      if(RA0==0&&RA5==1)
      {
    if(a==1)
    {
    int i = 0;
    rot = 0;
    
    while(RB6==0 || rot <= 200){
        PORTB = L_motor_cw_NPM[i];
        wait_ms(pps);
        i = i + 1;
        rot = rot + 1;

        if(i == 4){
            i = 0;
        }
         Lcd_Set_Cursor(2,13);
        Lcd_Write_String("CW.");
    }
    }
    else if (a==2)
    {
    int i = 0;
    rot = 0; 
    while(RB6==0 || rot <= 200){
        PORTB = L_motor_cw_MP50[i];
        wait_ms(pps);
        i = i + 1;
        rot = rot + 1;

        if(i == 8){
            i = 0;
        }
         Lcd_Set_Cursor(2,13);
        Lcd_Write_String("CW.");
    } 
    }
      }
}
void CCW_sensor(int a)
{
    if(RA1==0&&RA5==1)
    {
   if(a==1)
    {
    int i = 0;
    rot = 0;
    
    while(RB6==0 || rot <= 200){
        PORTB = L_motor_ccw_NPM[i];
        wait_ms(pps);
        i = i + 1;
        rot = rot + 1;

        if(i == 4){
            i = 0;
        }
         Lcd_Set_Cursor(2,13);
        Lcd_Write_String("CCW");
    }
    }
    else if (a==2)
    {
    int i = 0;
    rot = 0; 
    while(RB6==0 || rot <= 200){
        PORTB = L_motor_ccw_MP50[i];
        wait_ms(pps);
        i = i + 1;
        rot = rot + 1;

        if(i == 8){
            i = 0;
        }
         Lcd_Set_Cursor(2,13);
        Lcd_Write_String("CCW");
    } 
    }
    
    }
}
void offmotor(void)
{
    RB0=1;
    RB1=1;
    RB2=1;
    RB3=1;
}
void sensor()
{
    if(RA5==1)
    {
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("SENSOR_YES");
    }
    if(RA5==0)
    {
        Lcd_Set_Cursor(2,1);
        Lcd_Write_String("SENSOR_NO");
    }
}

void star()
{
     OSCILLATOR_Initialize();
     Lcd_Init();
     offmotor();
     Lcd_Clear();
     inputpps();
     motorset();
     sensor();
     pps_set(pps);
     
     //RA0 ??
     //RA1???
    
     while(RA2==1)
     {
         CW(motor);
         CCW(motor);
         CW_sensor(motor);
         CCW_sensor(motor);
         PORTB = 0xFF; 
     }
     Lcd_Clear();
     while(RA2==0);

}
     
    


    
