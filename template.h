#ifndef TEMPLATE_H
#define TEMPLATE_H OCT_2014
#include <arduino.h>

/*
	trying to use templated parameters to create static void functions
	that I can call with a function pointer.
*/

template<int A>
class Template_test{
	static const int a_pin = A;
public:
	Template_test<A>() {}

	static void print() {
		Serial.print("a_pin is: ");
		Serial.println(a_pin);
	}

};

template<int A>
class Counter{
	static const int A_pin = A;
	
	static void inc() {
		++ct;
	}
	
public:
	static int ct;

	Counter<A>() {} 
	
	void begin(){
		//pinMode(A_pin, INPUT_PULLUP);
		attachInterrupt(A_pin, &Counter<A>::inc, CHANGE);
	}
	
	int count() {return ct;}
	
};

template<int A> int Counter<A>::ct = 0;


#endif