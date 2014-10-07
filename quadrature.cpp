
#include "quadrature.h"

//************************************************************************
//*                     CLASS QUADRATURE_ENCODER
//************************************************************************
Quadrature_encoder::Quadrature_encoder(const int Enc_A_int_pin, 
    const int Enc_B_int_pin)
    : A_pin(Enc_A_int_pin), B_pin(Enc_B_int_pin)
    {}

void Quadrature_encoder::begin() 
{
    //store the starting state for the two interrupt pins
    pinMode(A_pin, INPUT);
    pinMode(B_pin, INPUT);
    
    Enc_A = digitalRead(A_pin);
    Enc_B = digitalRead(B_pin);
    
    //configure the interrupts
    attachInterrupt(A_pin, delta_A, CHANGE);
//    attachInterrupt(B_pin, delta_B, CHANGE);
}

Motion::motion Quadrature_encoder::motion()
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

//*********************************************************************************
//                               ISR's
//*********************************************************************************

void Quadrature_encoder::delta_A()
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

void Quadrature_encoder::delta_B()
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
