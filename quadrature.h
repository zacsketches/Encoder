#ifndef QUADRATURE_H
#define QUADRATURE_H OCT_2014

#include <arduino.h>

/*
   A quadrature encoder library for Arduino that takes
   advantage of inturrept processing to track direction
   and counts for a shaft.

   See the link below for the best explanation of quadrature I've found 
   on the web. 
   http://letsmakerobots.com/node/24031
   
   I've adapted the code from OdddBot to be interupt driven, and added 
   a counter.
*/

namespace Motion {
	enum motion{frwd, back, stop};
}

class Quadrature_encoder {

public:
	Quadrature_endoder(cont int Enc_A_int_pin, const int Enc_B_int_pin);
	long count();
	Motion::motion motion();
	
private:
	static const int QEM[16] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};
	
	const int A_pin;
	const int B_pin;
	volatile byte Enc_A;
	volatile byte Enc_B;
	volatile uint8_t out_val, old_reading, new_reading;
	volatile long count;
	
	// ISR's
	void delta_A();
	void delta_B();

};

#endif