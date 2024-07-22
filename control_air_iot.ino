
#include "ihm.h"
#include "ir.h"

#define buttonEnter 34
#define buttonBack 35
#define buttonUp 32
#define buttonDown 33

IHM ihm(buttonEnter, buttonBack, buttonUp, buttonDown);


void setup() {
  Serial.begin(115200);

  irBegin();

  ihm.begin(21, 22);
  ihm.setAddCommand(irCommandCopy);
  ihm.setTestCommand(irTestNewCommand);
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  ihm.controller();
  delay(200);
}
