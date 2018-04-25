/*
 * RangingSensor.h
 *
 * Created: 4/13/2018 7:15:29 PM
 *  Author: Ryan
 */ 


#ifndef RANGINGSENSOR_H_
#define RANGINGSENSOR_H_

#define DIST_TO_FLOAT(d)	((d)/10.0)
#define DIST_TO_INT(d)		((d)/10)

void init_sensor();
uint16_t read_sensor();
uint16_t read_avg_sensor(char num_readings);
uint16_t read_min_sensor(char num_readings);


#endif /* RANGINGSENSOR_H_ */