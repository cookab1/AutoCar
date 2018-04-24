/*
 * MotorControl.c
 *
 * Timer 1 - Timer for right wheel
 * ENA - Speed for right wheel
 * IN1, IN2 - Direction for right wheel
 * 
 * IN3, IN4 - Direction for left wheel
 * ENB - Speed for left wheel
 * Timer 2 - Timer for left wheel
 *
 * Created: 4/19/2018 11:34:38 AM
 * Author : mattm
 */ 

#include <util/delay.h>
#include <avr/io.h>

#define IN1 PINC0
#define IN2 PINC1
#define IN3 PINC2
#define IN4 PINC3
#define ENA PINB4
#define ENB PINE3

void setupMotors();
void setDirectionForward();
void setDirectionBackward();
void setSpeed(uint8_t);
void stop();

void leftDirectionForward();
void rightDirectionForward();
void leftDirectionBackward();
void rightDirectionBackward();
void leftSpeed(uint8_t);
void rightSpeed(uint8_t);
void leftStop();
void rightStop();


int main(void)
{
	setupMotors();
	
	// set direction
	setDirectionForward();
	
	
	
	
	
	
	
    while (1) 
    {
		
		//wait 2 secs then swap direction
		rightSpeed(0x90);
		leftSpeed(0x90);
		
		_delay_ms(2000);
		setDirectionBackward();
		_delay_ms(2000);
		setDirectionForward();
		
    }
}

void setDirectionForward() {
	PORTC |= (1 << IN1);
	PORTC &= ~(1 << IN2);
	PORTC &= ~(1 << IN3);
	PORTC |= (1 << IN4);
	
}

void setDirectionBackward() {
	PORTC &= ~(1 << IN1);
	PORTC |= (1 << IN2);
	PORTC |= (1 << IN3);
	PORTC &= ~(1 << IN4);
	
}

void stop() {
	PORTC &= 0xF0;
}

void setupMotors() {
	//portc 0-3 in
	
	//porth 3 - OC4A - timer 4 - ena - pin 6
	//porte 3 - OC3A - timer 3 - enb - pin 5
	
	
	//ena, enb set to output
	DDRH |= (1 << PINH3);
	DDRE |= (1 << PINE3);
	//in1, in2, in3, in4 set to output
	// a8, a9, a10, a11    /   dig pin 37, 36, 35, 34   / pin 53, 54, 55, 56
	DDRC &= 0x0F;
	
	
	
	//////////////// PWM

	
	// set COMnA1 to set at bottom, clear at match
	TCCR4A |= (1 << COM4A1) | (1 << WGM40);
	TCCR4B |= (1 << WGM42);
	
	TCCR3A |= (1 << COM3A1) | (1 << WGM30);
	TCCR3B |= (1 << WGM32);
	
	
	
	// no prescaling (super fast?)
	TCCR4B |= (1 << CS40);
	TCCR3B |= (1 << CS30);
	
	//set output compare to 0
	OCR4A = 0;
	OCR3A = 0;
}

void rightDirectionForward() {
	PORTC |= (1 << IN1);
	PORTC &= ~(1 << IN2);
}

void leftDirectionForward() {
	PORTC |= (1 << IN3);
	PORTC &= ~(1 << IN4);
}

void rightDirectionBackward() {
	PORTC &= ~(1 << IN1);
	PORTC |= (1 << IN2);
	
}

void leftDirectionBackward() {
	PORTC &= ~(1 << IN3);
	PORTC |= (1 << IN4);
}

void rightSpeed (uint8_t percent) {
	OCR4A = percent;
}

void leftSpeed (uint8_t percent) {
	OCR3A = percent;
}

void rightStop() {
	PORTC &= 0xFC;
}

void leftStop() {
	PORTC &= 0xF3;
}