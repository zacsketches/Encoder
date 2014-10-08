#include <template.h>

Template_test<3> t1;

Counter<0> c1;

void setup() {
  Serial.begin(115200);
  Serial.println();
  t1.print();
  
  c1.begin();
  
}

void loop() {
  char buf[50];
  sprintf(buf, "c1 count is: %d", c1.count());
  Serial.println(buf);
  delay(100);
}
