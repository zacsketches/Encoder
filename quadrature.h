#ifndef QUADRATURE_H
#define QUADRATURE_H OCT_2014

#include <arduino.h>
#include <clearinghouse.h>

//debug control
#define INCLUDE_ENCODER_PRINT 1

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

//************************************************************************
//*                   PURE ABSTRACT ENCODER CLASS
//* This class provides an interface to built vectors of templated
//* Quadrature Encoders, but access them through their common base.
//* For example, in the multiple_inheritance example the two quadrature
//* encoders are put into a Vector<Encoder*> and then each quadrature
//* encoder can be accessed by iterating over the Vector.
//************************************************************************


class Encoder : public gw::Node{
public:
	
	Encoder(const char* name) : Node (name) {}
	
    virtual long count() = 0;
    
	virtual Motion::motion motion() = 0;
	
	virtual void reverse() = 0;
	
	virtual Position::position pos() = 0;
	
};

template<int A, int B>
class Quadrature_encoder : public Encoder {
public:
	//Constructor
	/*
	   TODO use pre-processor commands to make the constructor
	   flexible for Uno, Mega or Due use.
	*/
	Quadrature_encoder<A, B>(Position::position pos = Position::none) 
		: Encoder("Encoder"), p(pos) {};

	Quadrature_encoder<A, B>(Position::position pos, const char* name) 
		: Encoder(name), p(pos) {};

	// From Base class
    //    char* name()
	//    int id()

	/* Virtual Void functions that must be overwritten */
	
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
	
	Position::position pos() { return p; }
	
	#if INCLUDE_ENCODER_PRINT == 1
		void print() {
			Serial.print(F("Node id: "));
			Serial.print(id());
			Serial.print("\t");
			Serial.print(name());   
			char buf[50];
			sprintf(buf, ". Inturrupts on pin %d and %d", A_pin, B_pin);
			Serial.println(buf);
		}
	#endif
	
private:
	Position::position p;	//Position::lt or Position::rt
	static const int A_pin = A;
	static const int B_pin = B;
	static volatile byte Enc_A;
	static volatile byte Enc_B;
	static volatile int out_val, old_reading, new_reading;
	static volatile long ct;
    static long old_ct;
	static bool r;
	
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