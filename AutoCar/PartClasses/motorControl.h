/*
 * motorControl.h
 *
 * Created: 4/26/2018 1:04:10 PM
 *  Author: mattm
 */ 


#ifndef MOTORCONTROL_H_
#define MOTORCONTROL_H_

#define IN1 PINC0
#define IN2 PINC1
#define IN3 PINC2
#define IN4 PINC3
#define ENA PINB4
#define ENB PINE3

#define MIN_SPEED 0x90
#define MED_SPEED 0xC8	/* mean of MIN_SPEED and MAX_SPEED */
#define MAX_SPEED 0xFF


void init_motors();
void setDirectionForward();
void setDirectionBackward();
void setSpeed(uint8_t);
void stop();

void leftDirectionForward();
void rightDirectionForward();
void leftDirectionBackward();
void rightDirectionBackward();
void setLeftSpeed(uint8_t);
void setRightSpeed(uint8_t);
uint8_t getLeftSpeed();
uint8_t getRightSpeed();
void leftStop();
void rightStop();



#endif /* MOTORCONTROL_H_ */