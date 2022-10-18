/******************************************************************************/
/* BLINKY.C: LED Flasher                                                      */
/******************************************************************************/
// Program zmodyfikowany do potrzeb laboratorium IISI Systemy Wbudowane
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2006 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#define uint16_t  unsigned int;

#include <AT91SAM7X256.H>                        /* AT91SAMT7X256 definitions */
#include "lcd.h"
#include "delay.h"


extern void InitSpi(void);
extern void InitLcd(void);
extern void LCDClearScreen(void);

extern void WriteSpiCommand(unsigned int data);
extern void WriteSpiData(unsigned int data);

extern void LCDClearScreen(void);
extern void LCDSetXY(int x, int y);
extern void LCDSetPixel(int  x, int  y, int  color);
extern void LCDSetLine(int x1, int y1, int x2, int y2, int color);
extern void LCDSetRect(int x0, int y0, int x1, int y1, unsigned char fill, int color);
extern void LCDSetCircle(int x0, int y0, int radius, int color);
extern void LCDPutChar(char c, int  x, int  y, int size, int fcolor, int bcolor);
extern void LCDPutString (char *lcd_string, const char *font_style, unsigned char x, unsigned char y,
                   unsigned char fcolor, unsigned char bcolor);
extern void LCDPutStr(char *pString, int  x, int  y, int Size, int fColor, int bColor);
extern void Delay (unsigned long a);

void start(void);
void rysowanie(int okoy1,int okoy2,int okox1,int okox2);


	
	// ARM7X256 Leds Definition


//maska dla pinow do ktorych podlaczone sa diody, którymi bedziemy sterowac
#define LEDS_MASK	(AT91C_PIO_PB20)
	

//konfiguracja wybranych linii PIO
												  //linie pracuja jako GPIO sterowane przez uzutkownika (a nie przez peryferia) = PIO_PER
												  //ustawienie pinow jako wyjscia = PIO_OER
												  //odblokowanie mozliwosc zapisu za pomoca rejestru ODSR = PIO_OWER
#define LED_INIT	{AT91C_BASE_PIOB->PIO_PER = LEDS_MASK; AT91C_BASE_PIOB->PIO_OER = LEDS_MASK; AT91C_BASE_PIOB->PIO_OWER = LEDS_MASK;}


//definicja ustawiania stanu niskiego wyjscia PB20 = zgaszenie podswietlenia LED wyswietlacza LCD 
#define LED_BCK_OFF		AT91C_BASE_PIOB->PIO_CODR = AT91C_PIO_PB20
//definicja ustawiania stanu wysokiego wyjscia PB20 = zalaczenie podswietlenia LED wyswietlacza LCD 
#define LED_BCK_ON		AT91C_BASE_PIOB->PIO_SODR = AT91C_PIO_PB20

	
 
#define LEDS_OFF		AT91C_BASE_PIOB->PIO_CODR = LEDS_MASK
//definicja ustawiania stanu wysokiego wyjscia PB20 = zalaczenie podswietlenia LED wyswietlacza LCD 
#define LEDS_ON		AT91C_BASE_PIOB->PIO_SODR = LEDS_MASK

//Inputs
#define LEFT_KEY_DOWN (((AT91C_BASE_PIOB->PIO_PDSR) & AT91C_PIO_PB24)==0)
#define RIGHT_KEY_DOWN (((AT91C_BASE_PIOB->PIO_PDSR) & AT91C_PIO_PB25)==0)

#define LEFT_STICK (((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_PIO_PB7)==0)
#define RIGHT_STICK (((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_PIO_PB14)==0)
#define UP_STICK (((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_PIO_PB9)==0)
#define DOWN_STICK (((AT91C_BASE_PIOA->PIO_PDSR) & AT91C_PIO_PB8)==0)

#define INPUTS_INIT {AT91C_BASE_PIOB->PIO_PER = (AT91C_PIO_PB24|AT91C_PIO_PB25); AT91C_BASE_PIOB->PIO_ODR = (AT91C_PIO_PB24|AT91C_PIO_PB25); }

//zmienne

				int wielkosc = 4;
				int snake_x[80] = {81,77,73,69};
			  int snake_y[80] = {60,60,60,60};
				int apple_x = 45;
				int apple_y = 72;
				
		    int i,zmienna = 1;
				
				int opcja = 1;
				int wybor = 0, back = 0;
				
				int START = 0;
				int szybkosc=50;
				int punkty = 0;
				char tablica[10];



void menugl()
{
				if( UP_STICK ) opcja = opcja-1;
        if( DOWN_STICK ) opcja = opcja+1;
	if (opcja == 3) opcja = 1;
	if (opcja == 0) opcja = 2;
	if (opcja == 1){
				LCDPutStr("SNAKE",40,20,LARGE,BLUE,WHITE);
				LCDPutStr("LOVE SNAKE",60,20,LARGE,BLUE,BLACK);
				back = 1;
	}	
	if (opcja == 2){
				LCDPutStr("SNAKE",40,20,LARGE,BLUE,BLACK);
				LCDPutStr("LOVE SNAKE",60,20,LARGE,BLUE,WHITE);
				back =2;
	}
				LCDSetRect(115,0,131,129,FILL,BLUE);
				LCDPutStr("WYBIERZ GRE:",10,10,LARGE,WHITE,BLUE);
				LCDPutStr("WYBIERAM",120,4,SMALL,WHITE,BLUE);
				//LCDPutStr("COFNIJ",120,92,SMALL,WHITE,BLUE);
				//Strzalka
				LCDSetPixel(120,65,WHITE);
				LCDSetPixel(121,64,WHITE);LCDSetPixel(121,65,WHITE);LCDSetPixel(121,66,WHITE);
				LCDSetPixel(122,63,WHITE);LCDSetPixel(122,64,WHITE);LCDSetPixel(122,65,WHITE);LCDSetPixel(122,66,WHITE);LCDSetPixel(122,67,WHITE);
				LCDSetPixel(124,63,WHITE);LCDSetPixel(124,64,WHITE);LCDSetPixel(124,65,WHITE);LCDSetPixel(124,66,WHITE);LCDSetPixel(124,67,WHITE);
				LCDSetPixel(125,64,WHITE);LCDSetPixel(125,65,WHITE);LCDSetPixel(125,66,WHITE);
				LCDSetPixel(126,65,WHITE);
				delay_ms(50);
}
void menu()
{
				
				if( UP_STICK ) opcja = opcja-1;
        if( DOWN_STICK ) opcja = opcja+1;
	if (opcja == 4) opcja = 1;
	if (opcja == 0) opcja = 3;
	if (opcja == 1){
				LCDPutStr("DLA DZIECI",40,20,LARGE,GREEN,WHITE);
				LCDPutStr("NORMAL",60,20,LARGE,GREEN,BLACK);
				LCDPutStr("DLA KOZAKA",80,20,LARGE,GREEN,BLACK);
				back = 10;
	}	
	if (opcja == 2){
				LCDPutStr("DLA DZIECI",40,20,LARGE,GREEN,BLACK);
				LCDPutStr("NORMAL",60,20,LARGE,GREEN,WHITE);
				LCDPutStr("DLA KOZAKA",80,20,LARGE,GREEN,BLACK);
				back = 20;
	}
	if (opcja == 3) {
				LCDPutStr("DLA DZIECI",40,20,LARGE,GREEN,BLACK);
				LCDPutStr("NORMAL",60,20,LARGE,GREEN,BLACK);
				LCDPutStr("DLA KOZAKA",80,20,LARGE,GREEN,WHITE);
				back = 30;
	}
				LCDSetRect(115,0,131,129,FILL,GREEN);
				LCDPutStr("WYBIERZ TRYB:",10,10,LARGE,BLACK,GREEN);
				LCDPutStr("WYBIERAM",120,4,SMALL,BLACK,GREEN);
				LCDPutStr("COFNIJ",120,92,SMALL,BLACK,GREEN);
				//Strzalka
				LCDSetPixel(120,65,BLACK);
				LCDSetPixel(121,64,BLACK);LCDSetPixel(121,65,BLACK);LCDSetPixel(121,66,BLACK);
				LCDSetPixel(122,63,BLACK);LCDSetPixel(122,64,BLACK);LCDSetPixel(122,65,BLACK);LCDSetPixel(122,66,BLACK);LCDSetPixel(122,67,BLACK);
				LCDSetPixel(124,63,BLACK);LCDSetPixel(124,64,BLACK);LCDSetPixel(124,65,BLACK);LCDSetPixel(124,66,BLACK);LCDSetPixel(124,67,BLACK);
				LCDSetPixel(125,64,BLACK);LCDSetPixel(125,65,BLACK);LCDSetPixel(125,66,BLACK);
				LCDSetPixel(126,65,BLACK);
				delay_ms(50);
}
void menu2()
{
				
				if( UP_STICK ) opcja = opcja-1;
        if( DOWN_STICK ) opcja = opcja+1;
	if (opcja == 4) opcja = 1;
	if (opcja == 0) opcja = 3;
	if (opcja == 1){
				LCDPutStr("WOLNIUTKO",40,20,LARGE,RED,WHITE);
				LCDPutStr("NORMAL",60,20,LARGE,RED,BLACK);
				LCDPutStr("SZYBCIUTKO",80,20,LARGE,RED,BLACK);
				back = 100;
	}	
	if (opcja == 2){
				LCDPutStr("WOLNIUTKO",40,20,LARGE,RED,BLACK);
				LCDPutStr("NORMAL",60,20,LARGE,RED,WHITE);
				LCDPutStr("SZYBCIUTKO",80,20,LARGE,RED,BLACK);
				back = 200;
	}
	if (opcja == 3) {
				LCDPutStr("WOLNIUTKO",40,20,LARGE,RED,BLACK);
				LCDPutStr("NORMAL",60,20,LARGE,RED,BLACK);
				LCDPutStr("SZYBCIUTKO",80,20,LARGE,RED,WHITE);
				back = 300;
	}
				LCDSetRect(115,0,131,129,FILL,WHITE);
				LCDPutStr("WYBIERZ TRYB:",10,10,LARGE,RED,WHITE);
				LCDPutStr("WYBIERAM",120,4,SMALL,RED,WHITE);
				LCDPutStr("COFNIJ",120,92,SMALL,RED,WHITE);
				//Strzalka
				LCDSetPixel(120,65,BLACK);
				LCDSetPixel(121,64,BLACK);LCDSetPixel(121,65,BLACK);LCDSetPixel(121,66,BLACK);
				LCDSetPixel(122,63,BLACK);LCDSetPixel(122,64,BLACK);LCDSetPixel(122,65,BLACK);LCDSetPixel(122,66,BLACK);LCDSetPixel(122,67,BLACK);
				LCDSetPixel(124,63,BLACK);LCDSetPixel(124,64,BLACK);LCDSetPixel(124,65,BLACK);LCDSetPixel(124,66,BLACK);LCDSetPixel(124,67,BLACK);
				LCDSetPixel(125,64,BLACK);LCDSetPixel(125,65,BLACK);LCDSetPixel(125,66,BLACK);
				LCDSetPixel(126,65,BLACK);
				delay_ms(50);
}
void gameover()
{
				 wielkosc = 4;
				 //snake_x[] = {81,77,73,69};
			   //snake_y[] = {60,60,60,60};
				 apple_x = 45;
				 apple_y = 72;
				
		     i=1,zmienna = 1;
{
wybor=0;
LCDClearScreen();
	LCDPutStr("GAME OVER",40,20,LARGE,RED,WHITE);
	sprintf(tablica,"PUNKTOW: %d",punkty);
	LCDPutStr(tablica,80,20,LARGE,RED,WHITE);
	delay_ms(1000);
	LCDClearScreen();
	punkty = 0;
start();
}
}
//rysowanie ciala
void rysowanie(int okoy1,int okoy2,int okox1,int okox2)
{
	//przedostatni
LCDSetRect(snake_y[1],snake_x[1],snake_y[1]+4,snake_x[1]+4,FILL,BLACK);
LCDSetPixel(snake_y[1],snake_x[1],YELLOW);
LCDSetPixel(snake_y[1]+1,snake_x[1]+1,YELLOW);
LCDSetPixel(snake_y[1]+2,snake_x[1]+2,YELLOW);
LCDSetPixel(snake_y[1]+3,snake_x[1]+3,YELLOW);
LCDSetPixel(snake_y[1]+4,snake_x[1]+4,YELLOW);
LCDSetPixel(snake_y[1]+4,snake_x[1],YELLOW);
LCDSetPixel(snake_y[1],snake_x[1]+4,YELLOW);
//pierwszy glowa	
//rysowanie kwadrat weza
LCDSetRect(snake_y[0],snake_x[0],snake_y[0]+4,snake_x[0]+4,FILL,BLACK);
LCDSetPixel(snake_y[0],snake_x[0],YELLOW);
LCDSetPixel(snake_y[0]+4,snake_x[0]+4,YELLOW);
LCDSetPixel(snake_y[0]+4,snake_x[0],YELLOW);
LCDSetPixel(snake_y[0],snake_x[0]+4,YELLOW);
LCDSetPixel(snake_y[0]+okoy1,snake_x[0]+okox1,YELLOW);
LCDSetPixel(snake_y[0]+okoy2,snake_x[0]+okox2,YELLOW);
}
void snake(int szybkosc)
{
// clear the screen
   LCDClearScreen();
				snake_x[0] = 81;
			  snake_y[0] = 60;
				apple_x = 45;
				apple_y = 72;
//pole gry
LCDSetRect(27,4,124,125,NOFILL,WHITE);
LCDSetRect(28,5,123,124,FILL,GREEN);
LCDPutStr("PUNKTOW: 0",5,5,LARGE,BLUE,WHITE);	

while (1) {
				if( RIGHT_STICK && zmienna != 2) zmienna = 1;
				if( LEFT_STICK && zmienna != 1) zmienna = 2;
        if( UP_STICK && zmienna !=4) zmienna = 3;
        if( DOWN_STICK && zmienna !=3) zmienna = 4;
if (snake_x[wielkosc-1]==apple_x && snake_y[wielkosc-1]==apple_y)
{
	wielkosc++;
	punkty++;
	sprintf(tablica,"PUNKTOW: %d",punkty);
	LCDPutStr(tablica,5,5,LARGE,BLUE,WHITE);
	apple_x = (4*(rand()%29+1))+1;
	apple_y = (4*(rand()%23+1))+24;
}
else 
	{
	//zakrywanie
	LCDSetRect(snake_y[wielkosc-1],snake_x[wielkosc-1],snake_y[wielkosc-1]+3,snake_x[wielkosc-1]+3,FILL,GREEN);
	
	}
				for (i = wielkosc-1; i > 0; i--) 
				{
				snake_y[i] = snake_y[i - 1];
  			snake_x[i] = snake_x[i - 1];
  			}
if(zmienna == 1)  //w prawo				
{			
			snake_x[0]=snake_x[0]+4; //right
			if (snake_x[0]==125) snake_x[0]=5;

}
if(zmienna == 2)  //w lewo
{
			snake_x[0]=snake_x[0]-4; //left
			if (snake_x[0]==1) snake_x[0]=121;
}
if(zmienna == 3)  //w gore
{
			snake_y[0]=snake_y[0]-4; //up
			if (snake_y[0]==24) snake_y[0]=120;
}
if(zmienna == 4)  //w dol
{
			snake_y[0]=snake_y[0]+4; //down
			if (snake_y[0]==124) snake_y[0]=28;
}
for (i=1;i<wielkosc;i++)
//Game Over
if (snake_x[0]==snake_x[i] && snake_y[0]==snake_y[i])	
gameover();
//rysowanie
				LCDSetRect(snake_y[0],snake_x[0],snake_y[0]+3,snake_x[0]+3,FILL,BROWN);
				//apple
				LCDSetRect(apple_y,apple_x,apple_y+3,apple_x+3,FILL,YELLOW);
			delay_ms(szybkosc);
		}
}
void lsnake(int szybkosc)
{
// clear the screen
   LCDClearScreen();
	snake_x[0]=80;
	snake_y[0]=58;
	apple_x = 50;
	apple_y = 68;
//pole gry
LCDSetRect(27,4,128,125,NOFILL,WHITE);
LCDSetRect(28,5,127,124,FILL,YELLOW);
LCDPutStr("PUNKTOW: 0",5,5,LARGE,BLUE,WHITE);	

while (1) {
				if( RIGHT_STICK && zmienna != 2) zmienna = 1;
				if( LEFT_STICK && zmienna != 1) zmienna = 2;
        if( UP_STICK && zmienna !=4) zmienna = 3;
        if( DOWN_STICK && zmienna !=3) zmienna = 4;
if (snake_x[wielkosc-1]==apple_x && snake_y[wielkosc-1]==apple_y)
{
	wielkosc++;
	punkty++;
	sprintf(tablica,"PUNKTOW: %d",punkty);
	LCDPutStr(tablica,5,5,LARGE,BLUE,WHITE);
	//LCDSetRect(5,90,20,100, FILL, WHITE);
	apple_x = (5*(rand()%24+1));
	apple_y = (5*(rand()%19+1)+23);
}
else 
	{
	//zakrywanie
	LCDSetRect(snake_y[wielkosc-1],snake_x[wielkosc-1],snake_y[wielkosc-1]+4,snake_x[wielkosc-1]+4,FILL,YELLOW);
	}
				for (i = wielkosc-1; i > 0; i--) 
				{
				snake_y[i] = snake_y[i - 1];
  			snake_x[i] = snake_x[i - 1];
  			}
if(zmienna == 1)  //w prawo				
{			
			snake_x[0]=snake_x[0]+5; //right
			if (snake_x[0]==125) snake_x[0]=5;
	//jan wezyk
rysowanie(1,3,2,2);
}
if(zmienna == 2)  //w lewo
{
			snake_x[0]=snake_x[0]-5; //left
			if (snake_x[0]==0) snake_x[0]=120;
		//jan wezyk
rysowanie(1,3,2,2);
}
if(zmienna == 3)  //w gore
{
			snake_y[0]=snake_y[0]-5; //up
			if (snake_y[0]==23) snake_y[0]=123;
		//jan wezyk
rysowanie(2,2,1,3);	
}
if(zmienna == 4)  //w dol
{
			snake_y[0]=snake_y[0]+5; //down
			if (snake_y[0]==128) snake_y[0]=28;
			//jan wezyk
rysowanie(2,2,1,3);	
}
for (i=1;i<wielkosc;i++)
//Game Over
if (snake_x[0]==snake_x[i] && snake_y[0]==snake_y[i])	
gameover();

//apple
//LCDSetRect(apple_y,apple_x,apple_y+4,apple_x+4,FILL,RED);
//Serce
LCDSetPixel(apple_y,apple_x+1,RED);
LCDSetPixel(apple_y,apple_x+3,RED);
LCDSetRect(apple_y+1,apple_x,apple_y+2,apple_x+4,FILL,RED);
LCDSetRect(apple_y+3,apple_x+1,apple_y+3,apple_x+3,FILL,RED);
LCDSetPixel(apple_y+4,apple_x+2,RED);
			delay_ms(szybkosc);
		}
}
void start()
{
while(1){
				switch(wybor)
				{
					case  0: menugl(); break;
					case  1: menu(); break;
					case  2: menu2(); break;
					case 10: snake(90); break;
					case 20: snake(60); break;
					case 30: snake(30); break;
					case 100: lsnake(90); break;
					case 200: lsnake(60); break;
					case 300: lsnake(40); break;
				}
				if(LEFT_KEY_DOWN)
				{
					
					wybor=back;
					LCDClearScreen();
					//break;
				}
				if(RIGHT_KEY_DOWN)
				{
					
					wybor=0;
					LCDClearScreen();
					//break;
				}
			}	
		}
// Main Program

int main (void) {


						 
  // Enable the Clock of the PIO
  AT91C_BASE_PMC->PMC_PCER  = (1 << AT91C_ID_PIOB) | (1 << AT91C_ID_PIOA);
	
	LED_INIT;

	AT91C_BASE_PIOB->PIO_ODR = (AT91C_PIO_PB24|AT91C_PIO_PB25); //keys SW1 i SW2 - jako wejscia
	// Initialize SPI interface to LCD
   InitSpi();

  // Init LCD
   InitLcd();

  // Voltage control (contrast setting)
	WriteSpiCommand(VOLCTR);
	WriteSpiData(-20);    // P1 = 32  volume value (experiment with this value to get the best contrast)
  WriteSpiData(3);   // P2 = 3   resistance ratio (only value that works)

	// clear the screen
   LCDClearScreen();

    while (1) {
			start();
			
			}
  }
