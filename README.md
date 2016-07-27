# ARDUINO ENCODER

####*THE BEST QUADRATURE DECODER LIBRARY FOR ARDUINO ON GITHUB!*
####*...and yeah...we realize we probably should have called it Arduino Decoder!*

##Overview

Arduino Encoder is designed to do one thing...make it quick and easy 
to implement interrupt based quadrature decoding on your project. We 
think it's the best for a few key reassons

**- Best performance:** Interrupt driven counters with fast ISRs.
Also, lots of other libraries for motor encoders rely on polling to 
catch changes in the encoder state...this is slow and
does not scale if you're thinking of making cool robots.  Our interrupt
driven library is designed for speed and will not lose counts even if
you use delay(x) or other long routines in your main sketch.

**- Simple file structure:** With only one file to include, all you need 
at the top of your sketch is `#include <quadrature.h>`

**- Simple syntax:** there are only four public functions for an encoder,
`.begin()`, `.count()`, `.motion()` and `.reverse()`.  Everything else is 
taken care of behind the scenes.

**- Simple construction:** Quadrature relies on tracking two encoder
signals so all you need to construct a decoder is the pin number that
you are going to plug the encoder into.  For example this constructs
a quadrature decoder on Arduino Uno:
    `Quadrature_encoder<0,1> decoder;`.

##Usage

1. In the top of your sketch place the line `#include <quadrature.h.>`
2. Construct a decoder in the global space, after the `includes` and before
the `setup()` function.  For example to use interrupt 0 and interrupt 1
on the Arduino Uno you would construct a decoder like this:
    Quadrature_encoder<0,1> decoder;
3. After you call begin, the other public methods are available to you.
 - `.count()` will return the count on the encoder.  Run a the example to
 see how the count increments when the motor is going forward and 
 decrements when it is turned backwards.  I usually turn the motor by
 hand when I'm setting up a new encoder so I can keep the code simple.
 - `.motion()` will return a Motion::motion type equal to Motion::frwd, 
 Motion::back, or Motion::stop showing the total motion since the last
 time `.motion()` was called.  This is worth a little more explanation
 if you are not familiar with custom types and `enum`s.  The header
 file `quadrature.h` defines an `enum` called `motion` in a 
 `namespace` called `Motion`.  Although they are the same word, the
 compiler recognizes the different capitalization so the two names
 don't conflict.  This creates a custom type `Motion::motion`.  This
 `enum type` can have three values: `Motion::frwd`, `Motion::back` or 
 `Motion::stop`.  So when you call `your_decoder.motion()` it returns
 one of these three values.  For example the code
     `if( your_decoder.motion() == Motion::fwrd ) light_grn_led();`
 would call the function `light_grn_led()` if the encoder has moved
 in the forward direction since the last time you called `.motion()`.
4. If the library is returning `Motion::back` when you are actually
 turning the shaft forward, then simply swap the two encoder wires.
 For example most encoder wires are white and yellow.  If you have
 the white wire plugged into the Uno pin2 and the yellow wire
 plugged into the Uno pin 3, then swapping white->3 and yellow->2
 will change the way the counts are accumulated and `.motion()` will
 give you the right result.  However, if you are like my group then
 your encoders are plugged into a printed circuit board and you might
 not be able to swap wires.  In this case, call
     `your_decoder_name.reverse()`
 fter you call `your_decoder.begin()` in the sketch `setup()`
 function.  This will `.reverse()` the output you get.


Uno Note: When making the physical connections for your circuit be sure to
remember (from the Arduino reference page on the attachInterrupt function)
that int0 and int1 are connected to pin2 and pin3 and not pin0 and pin1.

Due Note: I use the Due to drive a balancing robot named Mary Lou and
monitor 4 interrupts (also possible on the Mega).  However, on the Due
you can use any pin as an interrupt so instead of placing the interrupt
number into the constructor you put the actual pin number.  For example,
I frequently use the last four pins on the horizontal Due header to monitor
Mary Lou's two motors like this:
    `Quadrature_encoder<53,51> left_motor_decoder;`
	`Quadrature_encoder<49,47> right_motor_decoder;` 

##Installation

1. `cd` into your `.../Arduino/library` folder
2. type `git clone https://github.com/zacsketches/Encoder.git` at
the command prompt and the library will download in your library.
3. Wink..Wink...click on the `star` icon at the top of our Github 
page so you can keep track of our library and be aware of any changes
or improvements.  Seriously, we want to make this the best 
quadrature library on GitHub so give us a star and help us keep 
improving.
4. Restart the Arduino IDE so it relaods all the new libraries.
5. Click on file->examples->Encoder and load the example sketch
to get started.

##Technical approach

This library uses template construction and singleton classes.  Do not 
attempt to monitor more than one shaft or more than one motor with
each instance of a decoder.

We use templates, indicated by the two integers between the `<...>`
sympbols in the constructor, because this is required to present
our users with a clean interface.  This requirement stems from the
challenges of generating a function pointer to a class method.

Specifically, in the `.begin()` function Quadrature_Encoder calls the 
Arduino function `attachInterrupt(int, function_pointer, const char*)`.  
Most other libraries do not expose a simple to use class interface 
because the function_pointer pointing at the Interrupt Service 
Routine (ISR) cannot be a class method.  We get around this 
restriction by creating singleton classes through template insantiation 
which call upon static class methods for the ISRs.

This approach allows a clean interface to users and still retains its
performance!


####We are always looking to make this library better, so please fork us and open issues!
