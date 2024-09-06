#include <IRsend.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>

#include "nvs_flash.h"

#include "ir.h"

uint16_t *raw_array;

// NVS name space used for station mode credentials
const char app_nvs_ir_namespace[] = "ir_cmds";

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
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}

bool irsaveCommand(typeCommand type) {

  nvs_handle handle;

  if (nvs_open(app_nvs_ir_namespace, NVS_READWRITE, &handle) != ESP_OK) {
    Serial.println("Error while opening NVS handle!\n");
    return false;
  }

  int sizeCmd = 0;

  if (type == ON) {
    sizeCmd = sizeof(newCommand);
    nvs_set_blob(handle, "CmdOnSize", &sizeCmd, sizeof(sizeCmd));
    nvs_set_blob(handle, "CmdOn", &newCommand, sizeof(newCommand));
  } else if (type == OFF) {
    sizeCmd = sizeof(newCommand);
    nvs_set_blob(handle, "CmdOffSize", &sizeCmd, sizeof(sizeCmd));
    nvs_set_blob(handle, "CmdOff", &newCommand, sizeof(newCommand));
  } else {
    sizeCmd = sizeof(newCommand);
    nvs_set_blob(handle, "CmdVentilateSize", &sizeCmd, sizeof(sizeCmd));
    nvs_set_blob(handle, "CmdVentilate", &newCommand, sizeof(newCommand));
  }


  if (nvs_commit(handle) != ESP_OK) {
    Serial.println("Error comitting state to NVS!");
    return false;
  }

  nvs_close(handle);
  return true;
}

bool _irloadCommand(typeCommand type) {

  nvs_handle handle;

  if (nvs_open(app_nvs_ir_namespace, NVS_READWRITE, &handle) != ESP_OK) {
    Serial.println("Error while opening NVS handle!\n");
    return false;
  }

  size_t sizeCmd = 0;
  size_t size = sizeof(sizeCmd);
  if (type == ON) {
    nvs_get_blob(handle, "CmdOnSize", &sizeCmd, &size);
    if (nvs_get_blob(handle, "CmdOn", &newCommand, &sizeCmd) != ESP_OK) {
      Serial.println("No cmd found in NVS");
      return false;
    }
  } else if (type == OFF) {
    nvs_get_blob(handle, "CmdOFFSize", &sizeCmd, &size);
    if (nvs_get_blob(handle, "CmdOFF", &newCommand, &sizeCmd) != ESP_OK) {
      Serial.println("No cmd found in NVS");
      return false;
    }
  } else {
    nvs_get_blob(handle, "CmdVentilateSize", &sizeCmd, &size);
    if (nvs_get_blob(handle, "CmdVentilate", &newCommand, &sizeCmd) != ESP_OK) {
      Serial.println("No cmd found in NVS");
      return false;
    }
  }

  nvs_close(handle);
  return true;
}

void irSendCommand(typeCommand type) {
  if (_irloadCommand(type)) {
    _irSendCommand(&newCommand);
  }
}



void irTestNewCommand() {
  _irSendCommand(&newCommand);
}
