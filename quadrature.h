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
   EDIT 3/17/2016: This link is no longer active.
   
   See the following link for a similar description, but Oddbot doesn't seem to get a credit
   at this link althought it looks just like his work.
   http://www.robotshop.com/media/files/PDF/tutorial-how-to-use-a-quadrature-encoder-rs011a.pdf
   
   I've adapted the code from OdddBot to be interupt driven, and added 
   a counter.
   
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

namespace Board {
	enum board{uno, due};
}

namespace QEM {
    //Quadrature Encoder Matrix from OddBot
    const int qem[16] = {0,-1,1,2,1,0,2,-1,-1,2,0,1,2,1,-1,0};
}

template<int A, int B>
class Quadrature_encoder {

public:
	//Constructor
	Quadrature_encoder<A, B>(Board::board b = Board::uno) {};
    
    // Must be called in the sketch Setup
    void begin();
    
    // count() return the total counts accumulated by the encoder
    long count() {return ct;}
    
    // motion() returns whether the encoder has moved forward, backward,
    // or is motionless SINCE THE LAST TIME MOTION WAS CALLED.
	Motion::motion motion();
	
	//invert the output of .motion for the same direction of 
	//physical rotation of the shaft.  The simplest way to achieve
	//this is to swap the two encoder pins.  However, if you have the
	//shaft encoder pins plugged into a PCB or header that prevents
	//swapping the input pins then calling .reverse() in the setup
	//of your sketch will give you the correct output from subsequent
	//calls to .motion()
	void reverse() { r = !r; }
	
private:
	
	Board::board b;
	
	static const int A_pin = A;
	static const int B_pin = B;
	static volatile byte Enc_A;
	static volatile byte Enc_B;
	static volatile int out_val, old_reading, new_reading;
	static volatile long ct;
    static long old_ct;
	static bool r;
	
	int pin_A_digital;
	int pin_B_digital;
	
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
template<int A, int B> bool Quadrature_encoder<A, B>::r = false;

template<int A, int B>
inline
void Quadrature_encoder<A, B>::begin()
{
	    //store the starting state for the two interrupt pins
		if(b == Board::uno) {
			if (A_pin == 0) {
				pin_A_digital = 2;
				pin_B_digital = 3;
			} else {
				pin_A_digital = 3;
				pin_B_digital = 2;
			}
		}
		if(b == Board::due) {
			pin_A_digital = A_pin;
			pin_B_digital = B_pin;
		}
		
		/*
			TODO: Add mega support
		*/
		
	    pinMode(pin_A_digital, INPUT);
	    pinMode(pin_B_digital, INPUT);
    
	    Enc_A = digitalRead(pin_A_digital);
	    Enc_B = digitalRead(pin_B_digital);
    
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
	if(r){
	    new_reading = Enc_A * 2 + Enc_B;
    }
	else {
	    new_reading = Enc_B * 2 + Enc_A;
	}
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
	if(r){
	    new_reading = Enc_A * 2 + Enc_B;
    }
	else {
	    new_reading = Enc_B * 2 + Enc_A;
	}
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
