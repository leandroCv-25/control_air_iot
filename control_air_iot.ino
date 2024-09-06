
#include "ihm.h"
#include "ir.h"
#include "ir.h"

#define buttonEnter 34
#define buttonBack 35
#define buttonUp 36
#define buttonDown 33

IHM ihm(buttonEnter, buttonBack, buttonUp, buttonDown);

bool saveOnCmdOn() {
  return irsaveCommand(ON);
}

bool saveOnCmdOff() {
  return irsaveCommand(OFF);
}

bool saveOnCmdVentilate() {
  return irsaveCommand(VENTILATE);
}

void setup() {
  Serial.begin(115200);

  irBegin();

  ihm.begin(21, 22);
  ihm.setAddCommand(irCommandCopy);
  ihm.setTestCommand(irTestNewCommand);
  ihm.setSaveCmdOn(saveOnCmdOn);
  ihm.setSaveCmdOff(saveOnCmdOff);
  ihm.setSaveCmdVentilate(saveOnCmdVentilate);
  vTaskDelay(pdMS_TO_TICKS(2000));
}

void loop() {
  // put your main code here, to run repeatedly:
  ihm.controller();
  vTaskDelay(pdMS_TO_TICKS(200));
}
