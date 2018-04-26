/*
 * MotorControl.c
 *
 * Timer 4 - Timer for right wheel
 * ENA - Speed for right wheel
 * IN1, IN2 - Direction for right wheel
 * 
 * IN3, IN4 - Direction for left wheel
 * ENB - Speed for left wheel
 * Timer 3 - Timer for left wheel
 *
 * Created: 4/19/2018 11:34:38 AM
 * Author : meloanmm
 */ 

#include <avr/io.h>
#include "motorControl.h"

/**
 * Direction forward for both wheels
 */
void setDirectionForward() {
	PORTC |= (1 << IN1);
	PORTC &= ~(1 << IN2);
	PORTC &= ~(1 << IN3);
	PORTC |= (1 << IN4);
}

/**
 * Direction backward for both wheels
 */
void setDirectionBackward() {
	PORTC &= ~(1 << IN1);
	PORTC |= (1 << IN2);
	PORTC |= (1 << IN3);
	PORTC &= ~(1 << IN4);
	
}

/**
 * Stop both wheels
 *
 * Changes direction setting so wheels have no direction to turn.
 * Speed setting left unchanged.
 */
void stop() {
	PORTC &= 0xF0;
}

/**
 * Setup timers and PWM settings for wheel speed control
 */
void setupMotors() {	
	//porth 3 - OC4A - timer 4 - ena - pin 6
	//porte 3 - OC3A - timer 3 - enb - pin 5
	
	
	//ena, enb set to output
	DDRH |= (1 << PINH3);
	DDRE |= (1 << PINE3);
	//in1, in2, in3, in4 set to output
	// a8, a9, a10, a11    /   dig pin 37, 36, 35, 34   / pin 53, 54, 55, 56
	DDRC &= 0x0F;
	
	
	
	//////////////// PWM

	
	// set COMnA1 : set at bottom, clear at match
	TCCR4A |= (1 << COM4A1) | (1 << WGM40);
	TCCR4B |= (1 << WGM42);
	
	TCCR3A |= (1 << COM3A1) | (1 << WGM30);
	TCCR3B |= (1 << WGM32);
	
	
	
	// no prescaling
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
	PORTC &= ~(1 << IN3);
	PORTC |= (1 << IN4);
}

void rightDirectionBackward() {
	PORTC &= ~(1 << IN1);
	PORTC |= (1 << IN2);
	
}

void leftDirectionBackward() {
	PORTC |= (1 << IN3);
	PORTC &= ~(1 << IN4);
}

/**
 * Set speed for right wheel as percentage of power.
 *
 * Accepted values for motor range from 0x90 (low) to 0xFF (max).
 * This is due to motor having a minimum accepted voltage of around 6V.
 * A minimum value just above half is to make sure inconsistency in voltage 
 * won't just stop the wheel suddenly.
 */
void rightSpeed (uint8_t percent) {
	OCR4A = percent;
}

/**
 * Set speed for left wheel as percentage of power.
 *
 * See rightSpeed description for proper use.
 */
void leftSpeed (uint8_t percent) {
	OCR3A = percent;
}

void rightStop() {
	PORTC &= 0xFC;
}

void leftStop() {
	PORTC &= 0xF3;
}