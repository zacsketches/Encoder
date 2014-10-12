/*
  This example shows how the Quadrature Encoder class is 
  derived from both gw::Node and a pure vertual class called
  Encoder.  See quadrature.h to inspect both classes.

  The purpose of this additional abstraction is so that
  vectors of Encoder* can be built that access all the functions
  of the full Quadrature_encoder<A,B> class.  Because each
  template instantiation of Quadrature_encoder<A, B> is its 
  own type, you cannot build a Vector<Quadrature_encoder*>.

  To see the benefit of this in action run the script as is
  with a quadrature encoder connected on pins 4 and 5.  You 
  will see the sketch successfully compile and when you 
  open the serial monitor you will see the counts coming 
  from the sketch.  Notice in line 82 that the
  Quadrature_encoder<5,4> is being accessed through the
  abstract class Encoder* that is stored in the Vector.

  Now uncomment line 57, and try to compile the sketch, and
  you will find that it won't compile because the template
  arguments are not provided.  After it fails to compile,
  comment line 57 out again.

  Next uncomment line 58, 67 and 68. Try to comile the sketch.  This 
  time the error comes because you can't push back the
  second encoder.  Specifically, a Quadrature_encoder<5,4> is
  a different type than a Quadrature_encoder<7,6>.  So you
  can't store a <7,6> in a Vector designed to hold <5,4>'s.

  Now comment out 58, 67 and 68.  Compile the sketch 
  again and you can see the benefit of the abstract class
  because you can have a Vector<Encoder*> that points to 
  different types of Quadrature_encoder<A, B>'s.  This is possible
  they share the same base class of Encoder.

  Note: this sketch is meant for Arduino Due only because
  it can run all four interrupts on any pin.  The sketch could
  also be run on an Arduino Mega, but you will need to change
  the interrupt numbers in lines x and x.
*/

#include <clearinghouse.h>
#include <quadrature.h>
#include <Vector.h>
#include <Pair.h>

//Clearinhouse setup
gw::Clearinghouse ch;
int gw::Message::msg_count = 0;
int gw::Node::node_count = 0;

Quadrature_encoder<5,4> rt_encoder(Position::rt, "rt_enc");
Quadrature_encoder<7,6> lt_encoder(Position::lt, "lt_enc");

Vector<Encoder*> encoders;
//Vector<Quadrature_encoder*> encoders_2;
//Vector<Quadrature_encoder<5,4>* > encoders_3;

void setup() {
	Serial.begin(57600);
	Serial.println();
	        
	encoders.push_back(&rt_encoder);
	encoders.push_back(&lt_encoder);
	
//	encoders_3.push_back(&rt_encoder);
//	encoders_3.push_back(&lt_encoder);
	

        for(int i=0; i<encoders.size(); ++i) {
          encoders[i]->begin(); 
        }
        
        rt_encoder.reverse();
	
	rt_encoder.print();
	lt_encoder.print();
}

void loop() {
  for (int i=0; i<encoders.size(); ++i) {
    long ct = encoders[i]->count(); 
    const char* name = encoders[i]->name();
    char buf[50];
    sprintf(buf, "%s count is: %d", name, ct);
    Serial.println(buf);    
  }
  Serial.println("-----------------------------------");
  
  delay(100);
	
}