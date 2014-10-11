#include <Vector.h>
#include <Pair.h>

#include <quadrature.h>
#include <clearinghouse.h>

/*
	Because the attachInterrupt(int, function_ptr, const) requires
    a function pointer as its final argument the encoder class
    cannot use normal construction.  Each instance of 
    the Quadrature_encoder must be a singleton class constructed
    from the template with the two integer arguments of the 
    interrupt pins used for the encoder.  For example to build
    a quadrature encoder to use int0 and int1 on the Arduino Uno
    you would construct it like this:
        Quadrature_encoder<0, 1> encoder;

	The following enum is provided in the header file:
    Motion::motion {frwd, back, stop}

    The public methods of Quadrature_encoder are:
		begin() - must be called in setup
		count() - returns a long of the encoder count
		motion() - returns a value from the Motion::motion enum
			corresponding to what direction the shaft has turned
			since the last time motion() was called.
                reverse() - changes the polarity of the decoder in case
                        you need to change the sign of the output.  Run
                        the example below with the .reverse() line commented
                        out and see the difference in output.
   
    So to use the encoder constructed earlier you would call
    encoder.begin() in the setup() of the sketch and then you
    can call encoder.count() or encoder.motion() anywhere else
    in the sketch to read the encoder.
*/

//Basics of all Glow Worm systems.
gw::Clearinghouse ch;
int gw::Message::msg_count = 0;
int gw::Node::node_count = 0;

Quadrature_encoder<5,4> enc1;

void setup() {
  Serial.begin(57600);
  Serial.println();
  enc1.print();
  
  enc1.begin();
  enc1.reverse();
}

void loop() {
  //read the encoder
  long ct = enc1.count();
  
  //format the output for printing 
  char buf[50];
  sprintf(buf, "enc1 count is: %d", ct);
  Serial.println(buf);
  
  //check the motion
  Motion::motion m = enc1.motion();
  Serial.println(text(m));

  //delay the loop for a moment.
  //Note that the encoder continues to count even during this
  //delay because it is interrupt driven!
  delay(100);

}