
/*
 * sw_delay.s
 *
 * Created: 1/25/2018 2:53:17 PM
 *  Author: Ryan Tarravechia tarravechiarj
 */ 
		.section .text
		.global delay_usec

/*  delay_usec function: busy waits for of a specified number of microseconds,
 *  using 16 clock cycles per specified ms. Accepts an unsigned int with value 
 *  ranging from 1 to 65536; 65536 is represented by 0.
 * 
 *  Assumes an initial overhead of 6 clock cycles, 4 for an rcall and 2 for  
 *  loading an immediate argument.
 */
delay_usec:
		rjmp test
loop:
		nop				; delay for last 4 cycles of previous ms
		nop				
		nop				
		nop				; delay for first 8 cycles of next ms
		nop							
		nop							
		nop							
		nop							
		nop							
		nop							
		nop							
		nop				
test:
		sbiw r24, 1		; decrement ms count before testing for 0
		brne loop		; not-taken branch uses 1 clock cycle; taken uses 2
		ret				; ret uses last 5 clock cycles 
