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


void forward1s();
void backward1s();

void init_motors();
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



#endif /* MOTORCONTROL_H_ */