#include <quadrature.h>

/*
	Because the attachInterrupt(int, function_ptr, const) requires
    a function pointer as its final argument the encoder class
    cannot use normal construction.  Each instance of 
    the Quadrature_encoder must be a singleton class constructed
    from the template with the two integer arguments of the 
    interrupt pins used for the encoder.

	The following enum is provided in the header file:
    Motion::motion {frwd, back, stop}

    The public methods of Quadrature_encoder are:
		begin() - must be called in setup
		count() - returns a long of the encoder count
		motion() - returns a value from the Motion::motion enum
			corresponding to what direction the shaft has turned
			since the last time motion() was called.
*/

Quadrature_encoder<51,53> enc1;

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  enc1.begin();
}

void loop() {
  char buf[50];
  sprintf(buf, "enc1 count is: %d", enc1.count());
  Serial.println(buf);
  delay(100);
  
  Motion::motion m = enc1.motion();
  Serial.println(text(m));

}
