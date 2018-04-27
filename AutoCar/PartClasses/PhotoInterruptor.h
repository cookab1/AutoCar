/*
 * PhotoInterruptor.h
 *
 * Created: 4/24/2018 6:10:53 PM
 *  Author: Ryan
 */ 


#ifndef PHOTOINTERRUPTOR_H_
#define PHOTOINTERRUPTOR_H_

typedef struct {
	volatile unsigned long distance;
	volatile unsigned long msPerSlot;
	volatile unsigned int slot_count;	
} rotaryDisc_t;

void init_photoInterruptors();
void reset_distance();
void reset_ldistance();
void reset_rdistance();
void get_distance(unsigned long *ldist, unsigned long *rdist);
unsigned long get_ldistance();
unsigned long get_rdistance();
void get_freq(unsigned long *lfreq, unsigned long *rfreq);
unsigned long get_lfreq();
unsigned long get_rfreq();


#endif /* PHOTOINTERRUPTOR_H_ */