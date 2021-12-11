
#include "flags.hpp"

#ifdef OOP
#include "Seriallib.hpp"
#include "Arduino.h"

float data[100];

Parser receiver = Parser();

void setup()
{
  Serial.begin(115200);
}

initial_t x;

void loop()
{
  receiver.receive_serial_block(data, 10);
  x = receiver.getInitial();

  for (int i = 0; i < 5; i++)
  {
    Serial.println(data[i]);
  }

  Serial.print("Initial mode: ");
  Serial.println(x.mode);

  Serial.print("Initial full: ");
  Serial.println(x.str);
}
#endif // OOP

//@ADR#0.0#6969#1.5#2.25#3.0#&