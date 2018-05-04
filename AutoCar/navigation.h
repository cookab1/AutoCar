/*
 * navigation.h
 *
 * Created: 5/1/2018 2:28:23 PM
 *  Author: Ryan
 */ 


#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#define FRWD 0
#define BKWD 1

void go_straight(unsigned int cm, uint8_t speed, uint8_t direction);
void pivot (int);


#endif /* NAVIGATION_H_ */