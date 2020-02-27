/*	Author: steven
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCD_16x2_H_file.h"
#include "ADC_H.h"
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;  

unsigned long _avr_timer_M = 1; 
unsigned long _avr_timer_cntcurr = 0; 

void TimerOn() {
  TCCR1B = 0x0B;
  OCR1A = 125;
  TIMSK1 = 0x02;
  TCNT1 = 0;
  _avr_timer_cntcurr = _avr_timer_M;
  SREG |= 0x80;
}

void TimerOff() {
  TCCR1B = 0x00;
}

void TimerISR() {        
  TimerFlag = 1;
}  

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--; 
	if (_avr_timer_cntcurr == 0) { 
		TimerISR(); 
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	char buffer[20];
	int ADC_Value;
	TimerSet(1);
	TimerOn();
	
	ADC_Init();							/* ADC initialize function */
	LCD_Init();							/* LCD initialize function */

	while(1)
	{
		ADC_Value = ADC_Read(0);		/* Read the status on X-OUT pin using channel 0 */
		sprintf(buffer, "X=%d   ", ADC_Value);
		LCD_String_xy(1, 0, buffer);
		
		ADC_Value = ADC_Read(1);		/* Read the status on Y-OUT pin using channel 0 */
		sprintf(buffer, "Y=%d   ", ADC_Value);
		LCD_String_xy(1, 8, buffer);

		ADC_Value = ADC_Read(2);		/* Read the status on SWITCH pin using channel 0 */
		/*if(ADC_Value < 600)
			LCD_String_xy(2, 0, "Switch pressed   ");
		else
			LCD_String_xy(2, 0, "Switch open      ");
			*/
		sprintf(buffer, "Z=%d   ", ADC_Value);
		LCD_String_xy(2, 0, buffer);
		if (ADC_Read(0) >= 0){
			PORTB = 0x04; //right
		}

		//else if (ADC_Read(0) < ){
		//	PORTB = 0x08; //left
		//}

		else {
			PORTB = 0x00;
		}

		/*else if (ADC_Read(1) < 520){
			PORTB = 0x02;
		}

		else if (ADC_Read(1) > 520){
			PORTB = 0x01;
		} */
	}
}
