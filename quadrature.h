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

namespace QEM {
    //Quadrature Encoder Matrix from OddBot
    const int qem[16] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};
}

class Quadrature_encoder {

public:
	//Constructor
	/*
	   TODO use pre-processor commands to make the constructor
	   flexible for Uno, Mega or Due use.
	*/
	Quadrature_encoder(const int Enc_A_int_pin, const int Enc_B_int_pin);
    
    // Must be called in the sketch Setup
    void begin();
    
    // count() return the total counts accumulated by the encoder
    long count() {return ct;}
    
    // motion() returns whether the encoder has moved forward, backward,
    // or is motionless SINCE THE LAST TIME MOTION WAS CALLED.
	Motion::motion motion();
	
private:
	
	const int A_pin;
	const int B_pin;
	volatile byte Enc_A;
	volatile byte Enc_B;
	volatile uint8_t out_val, old_reading, new_reading;
	volatile long ct;
    long old_ct;
	
	// ISR's
	void delta_A();
	void delta_B();

};

#endif