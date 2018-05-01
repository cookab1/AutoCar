/*
 * PhotoInterruptor.h
 *
 * Created: 4/24/2018 6:10:53 PM
 *  Author: Ryan
 */ 


#ifndef PHOTOINTERRUPTOR_H_
#define PHOTOINTERRUPTOR_H_

void init_photoInterruptors();
void reset_steps();
void reset_lsteps();
void reset_rsteps();
void get_steps(unsigned int *left, unsigned int *right);
unsigned int get_lsteps();
unsigned int get_rsteps();
unsigned int stepsToCm(unsigned int steps);
unsigned int cmToSteps(unsigned int cm);


#endif /* PHOTOINTERRUPTOR_H_ */