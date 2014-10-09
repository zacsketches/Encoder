#include <quadrature.h>

/*
	Because the attachInterrupt(int, function_ptr, const) requires
    a function pointer as its final argument the encoder class
    cannot use normal construction.  Each instance of 
    the Quadrature_encoder must be a singleton class constructed
    from the template with the two integer arguments of the 
    interrupt pins used for the encoder.  For example to build
    a quadrature encoder to use int0 and int on the Arduino Uno
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
   
    So to use the encoder constructed earlier you would call
    encoder.begin() in the setup() of the sketch and then you
    can call encoder.count() or encoder.motion() anywhere else
    in the sketch to read the encoder.
*/

Quadrature_encoder<51,53> enc1;

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  enc1.begin();
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
