
//super slow clock driver by Raphael Abrams
#define _XTAL_FREQ 32768
#define PULSEWIDTH 15
#define TIMESTRETCH 730
#include <htc.h>
#include <stdio.h>

//__CONFIG(0,FOSC_LP & BOREN_ON & WDTE_OFF  & MCLRE_ON &  IESO_OFF & CPD_OFF  & PLLEN_OFF & FCMEN_OFF );
//__CONFIG(1,PLLEN_OFF&BORV_19&LVP_OFF);
__CONFIG(0x0FE0);
__CONFIG(0x1CFF);  //__CONFIG(0x1403);
volatile near unsigned char tickCounter;
volatile near unsigned int stretcher=0;

void init(void){

	 //  Timer 2 interrupt enabled.
	PIE1	= 0b00000010;

	//  Peripheral interrupts enabled
	//  Global interrupt disabled during initialization
	INTCON	= 0b01000000;
	
	// Timer 2 Code 
	//  Prescale ratio set at 1:64
	//  Timer2 module activated
	//  Postscale ratio set at 1:16
	T2CON	= 0b01111111;

	 //  Period register matches on every 32 timer cycles, 
	 //which are as slow as possible with both post and prescalers maxed out
	PR2	= 8;

	TRISA = 0b11111000;
	PORTA=0b11111011;
	
	ei();	// Global interrupts enabled
}

void main(void){
	int x=0;
	init();
	tickCounter = 0;
			PORTA=0b11111010;
			__delay_ms(50);
			PORTA=0b11111011;
			__delay_ms(50);
			PORTA=0b11111010;
			__delay_ms(50);
			PORTA=0b11111011;

        while(1){x++;}
}

void interrupt isr(void){
//  Timer 2 Code
    if((TMR2IE)&&(TMR2IF)){
        stretcher++;

        if(stretcher%20==0){
        PORTA=0b11111111;
	__delay_ms(PULSEWIDTH);
	PORTA=0b11111011;
        }
        if(stretcher==TIMESTRETCH){
            stretcher=0;
                if(tickCounter == 1){
			PORTA=0b11111010;
			//__delay(5);
			PORTA=0b11111011;
			tickCounter=0;		
        	}
		else if(tickCounter == 0){
			PORTA=0b11111101;
			__delay_ms(PULSEWIDTH);
			PORTA=0b11111111;
			tickCounter=1;		
		}
	}
    }
    TMR2IF=0;	// clear event flag
}