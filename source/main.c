/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


void transmit_data(unsigned char data) {
 int i;
 for (i = 0; i < 8 ; ++i) {
 // Sets SRCLR to 1 allowing data to be set
 // Also clears SRCLK in preparation of sending data
 PORTC = 0x08;
 // set SER = next bit of data to be sent.
 PORTC |= ((data >> i) & 0x01);
 // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
 PORTC |= 0x02;
 }
 // set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
 PORTC |= 0x04;
 // clears all lines in preparation of a new transmission
 PORTC = 0x00;
}


unsigned char but;
unsigned char but1;
unsigned char inc = 0x00;
unsigned char go;


enum states{start,init,wait1,wait2,both}state;

void fsm(){
	but = ~PINA & 0x01;
	but1 = ~PINA & 0x02;

	switch(state){
	case start:	
		state = init;
		break;
	case init:
		if(but){
		inc++;
		transmit_data(inc);
		state = wait1;
		}
		else if (but1){
		inc--;
		transmit_data(inc);
		state = wait2;
		}
		else if (but && but1){
		inc = 0;
		transmit_data(inc);
		state = both;
		}
		else{
		state = init;
		}
		break;

	case wait1:
		if(but){
		state = wait1;
		}
		else if (!but){
		state = init;
		}
		break;

	case wait2:
		if(but1){
		state = wait2;
		}
		else if (!but1){
		state = init;
		}
		break;
	case both:
		if(but && but1){
		state = both;
		}
		else if (!but && !but1){
		state = init;
		}
		break;
	}
	switch(state){
		default:
			break;
	}
}




int main(void) {
    /* Insert DDR and PORT initializations */
DDRA = 0x00; PORTA = 0xFF;
DDRB = 0xFF; PORTB = 0x00;
DDRC = 0xFF; PORTC = 0x00;
unsigned char test = 0xFF;
state = start;
    /* Insert your solution below */
    while (1) {
	    transmit_data(test);
	    fsm();
    }
    return 1;
}
