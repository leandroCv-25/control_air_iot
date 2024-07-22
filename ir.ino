#include <IRsend.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

#include "ir.h"

uint16_t *raw_array;

void irBegin() {
  irrecv.enableIRIn();  // Start up the IR receiver.
  irsend.begin();       // Start up the IR sender.
}

bool irCommandCopy() {
  // Check if an IR message has been received.

  uint32_t tempoInicial = millis();
  decode_results results;
  bool hasReceived = false;


  while (!newCommand.isEmpty()) {
    newCommand.clear();
  }

  while (millis() - tempoInicial < 15000 || tempoInicial - millis() < 15000) {
    if (irrecv.decode(&results)) {  // We have captured something.
      uint16_t size = results.bits;
      hasReceived = true;
      raw_array = resultToRawArray(&results);
      // Find out how many elements are in the array.
      size = getCorrectedRawLength(&results);

      command commandCopy = {
        .size = size,
      };

      for (int i = 0; i < size; i++) {
        commandCopy.data[i] = raw_array[i];
      }

      newCommand.add(commandCopy);
      // // if (size < 100) {
      // for (int i = 0; i < size / 2; i++) {
      //   Serial.printf("%i{0:%d}{1:%d} \n", i, raw_array[i], raw_array[i + 1]);
      // }
      // Send it out via the IR LED circuit.


      // irsend.sendRaw(raw_array, size, kFrequency);
      irrecv.resume();
    }
    yield();
  }
  return hasReceived;
}

void _irSendCommand(List<command> *command) {

  for (int i = 0; i < command->getSize(); ++i) {

    // if (size < 100) {
    // for (int j = 0; j < command->get(i).size / 2; j++) {
    //   Serial.printf("%i - %i{0:%d}{1:%d} \n", i,j, command->get(i).data[j], command->get(i).data[j + 1]);
    // }

    irsend.sendRaw(command->get(i).data, command->get(i).size, kFrequency);
    // irrecv.resume();
    delay(50);
  }
}

void irTestNewCommand() {
  _irSendCommand(&newCommand);
}
