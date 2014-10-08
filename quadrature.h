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

   For reference on why all the template files are included in the header
   instead of a separate .cpp file see this link
   http://www.parashift.com/c++-faq-lite/separate-template-class-defn-from-decl.html
*/

namespace Motion {
	enum motion{frwd, back, stop};

    const char res0[5] = "frwd";
    const char res1[5] = "back";
    const char res2[5] = "stop";

	inline const char* text(motion m) {
        const char* res;
            
        switch(m) {
            case frwd:
                res = res0; 
                break;
            case back:
                res = res1;
                break;
            case stop:
                res = res2;    
                break;
        }
        return res;
    }
}

namespace QEM {
    //Quadrature Encoder Matrix from OddBot
    const int qem[16] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};
}

template<int A, int B>
class Quadrature_encoder {

public:
	//Constructor
	/*
	   TODO use pre-processor commands to make the constructor
	   flexible for Uno, Mega or Due use.
	*/
	Quadrature_encoder<A, B>() {};
    
    // Must be called in the sketch Setup
    void begin();
    
    // count() return the total counts accumulated by the encoder
    long count() {return ct;}
    
    // motion() returns whether the encoder has moved forward, backward,
    // or is motionless SINCE THE LAST TIME MOTION WAS CALLED.
	Motion::motion motion();
	
private:
	
	static const int A_pin = A;
	static const int B_pin = B;
	static volatile byte Enc_A;
	static volatile byte Enc_B;
	static volatile int out_val, old_reading, new_reading;
	static volatile long ct;
    static long old_ct;
	
	// ISR's
	static void delta_A();
	static void delta_B();

};

template<int A, int B> volatile byte Quadrature_encoder<A, B>::Enc_A = LOW;
template<int A, int B> volatile byte Quadrature_encoder<A, B>::Enc_B = LOW;
template<int A, int B> volatile int Quadrature_encoder<A, B>::out_val = 0;
template<int A, int B> volatile int Quadrature_encoder<A, B>::old_reading = 0;
template<int A, int B> volatile int Quadrature_encoder<A, B>::new_reading = 0;
template<int A, int B> volatile long Quadrature_encoder<A, B>::ct = 0;
template<int A, int B> long Quadrature_encoder<A, B>::old_ct = 0;

template<int A, int B>
inline
void Quadrature_encoder<A, B>::begin()
{
	    //store the starting state for the two interrupt pins
	    pinMode(A_pin, INPUT);
	    pinMode(B_pin, INPUT);
    
	    Enc_A = digitalRead(A_pin);
	    Enc_B = digitalRead(B_pin);
    
	    //configure the interrupts
		attachInterrupt(A_pin, &Quadrature_encoder<A,B>::delta_A, CHANGE);
		attachInterrupt(B_pin, &Quadrature_encoder<A,B>::delta_B, CHANGE);
    }

template<int A, int B>
inline
Motion::motion Quadrature_encoder<A, B>::motion()
{
    Motion::motion res;
    long new_count = count();
    long delta = new_count - old_ct;
    if(delta > 0) {
        res = Motion::frwd;
    }
    else if (delta < 0){
        res = Motion::back;
    }
    else if (delta == 0){
        res = Motion::stop;
    }

    //set up for the next call
    old_ct = new_count;
    
    return res;
}

template<int A, int B>
inline
void Quadrature_encoder<A, B>::delta_A()
{
    old_reading = new_reading;
    Enc_A = !Enc_A;
    new_reading = Enc_A * 2 + Enc_B;
    out_val = QEM::qem [old_reading * 4 + new_reading];
    switch(out_val){
      case 1:
        ++ct;
        break;
      case -1:
        --ct;
        break;
    }
}

template<int A, int B>
inline
void Quadrature_encoder<A, B>::delta_B()
{
    old_reading = new_reading;
    Enc_B = !Enc_B;
    new_reading = Enc_A * 2 + Enc_B;
    out_val = QEM::qem [old_reading * 4 + new_reading];
    switch(out_val){
      case 1:
        ++ct;
        break;
      case -1:
        --ct;
        break;
    }
}

#endif