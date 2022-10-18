/*
 * main.c
 *
 *  Created on: 2011-11-11
 *      Author: JJ
 */
//  ***************************************************************************************************
//          main.c
//
//
//
//  ***************************************************************************************************

//  *******************************************************
//                Header Files
//  *******************************************************
#include "AT91SAM7X256.h"
#include "lcd.h"
#include "board.h"


//  *******************************************************
//                External References
//  *******************************************************
extern void AT91F_LowLevelInit(void);
extern void InitSpi(void);
extern void InitLcd(void);
extern void LCDClearScreen(void);


int  main (void) {

 unsigned long  j;


  unsigned int  IdleCount = 0;
  int    TempColor[11] = { WHITE, BLACK, RED, GREEN, BLUE, CYAN, MAGENTA,
       YELLOW, BROWN, ORANGE, PINK };
  char    *TempChar[11] = { "White", "Black", "Red", "Green", "Blue", "Cyan",
             "Magenta", "Yellow", "Brown", "Orange", "Pink" };

  // Initialize the Atmel AT91SAM7S256 (watchdog, PLL clock, default interrupts, etc.)
 AT91F_LowLevelInit();


  LED_R_INIT;
  LED_Y_INIT;
  // Initialize SPI interface to LCD
   InitSpi();

  // Init LCD
   InitLcd();
  // clear the screen
   LCDClearScreen();

   // ***************************************************************
  // *  color test - show boxes of different colors
   // ***************************************************************

  for (j = 0; j < 11; j++) {

    // draw a filled box
    LCDSetRect(120, 10, 25, 120, FILL, TempColor[j]);

    // label the color
    LCDPutStr("        ", 5, 40, LARGE, BLACK, BLACK);
    LCDPutStr(TempChar[j], 5, 40, LARGE, YELLOW, BLACK);

    // wait a bit
    delay_ms(100);
   }

   // ***************************************************************
  // *  character and line tests - draw lines, strings, etc.
   // ***************************************************************

  // clear the screen
   LCDClearScreen();

  // set a few pixels
   LCDSetPixel(30, 120, RED);
   LCDSetPixel(34, 120, GREEN);
   LCDSetPixel(38, 120, BLUE);
   LCDSetPixel(40, 120, WHITE);

  // draw some characters
  LCDPutChar('E', 10, 10, SMALL, WHITE, BLACK);

  // draw a string
  LCDPutStr("Hello World", 60, 5, SMALL, WHITE, BLACK);
  LCDPutStr("Hello World", 40, 5, MEDIUM, ORANGE, BLACK);
  LCDPutStr("Hello World", 20, 5, LARGE, PINK, BLACK);

  // draw a filled box
  LCDSetRect(120, 60, 80, 80, FILL, BROWN);

  // draw a empty box
  LCDSetRect(120, 85, 80, 105, NOFILL, CYAN);
  // draw some lines
  LCDSetLine(120, 10, 120, 50, YELLOW);
  LCDSetLine(120, 50, 80, 50, YELLOW);
  LCDSetLine(80, 50, 80, 10, YELLOW);
  LCDSetLine(80, 10, 120, 10, YELLOW);

  LCDSetLine(120, 85, 80, 105, YELLOW);
  LCDSetLine(80, 85, 120, 105, YELLOW);

  // draw a circle
  LCDSetCircle(65, 100, 10, RED);


   delay_ms(100);

   // ***************************************************************
    // *  bmp display test - display the Olimex photograph
   // ***************************************************************

   LCDClearScreen();

   LCDWrite130x130bmp();

    LCDPutStr("This guy is nuts", 115, 2, LARGE, BLACK, CYAN);

    // draw a filled box
    LCDSetRect(90, 70, 75, 120, FILL, YELLOW);

    LCDPutStr("HELP!", 80, 80, SMALL, BLACK, YELLOW);


    LED_Y_ON;

   // *****************************
    // *  endless blink loop
   // *****************************
    while (1) {

    	delay_ms(100);
    	LED_R_ON;
    	LED_Y_OFF;
    	delay_ms(100);
       LED_R_OFF;
 //       LED_Y_ON;

   IdleCount++;          // count # of times through the idle loop
   }
  }
