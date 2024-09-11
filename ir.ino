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
      // irsend.sendRaw(raw_array, size, kFrequency);
      irrecv.resume();
    }
    yield();
  }
  return hasReceived;
}

void _irSendCommand(List<command> *command) {

  // for (int i = 0; i < newCommand.getSize(); ++i) {

  //   for (int j = 0; j < newCommand.get(i).size / 2; j++) {
  //     Serial.printf("%i - %i{0:%d}{1:%d} \n", i, j, newCommand.get(i).data[j], newCommand.get(i).data[j + 1]);
  //   }
  //   vTaskDelay(pdMS_TO_TICKS(50));
  // }

  for (int i = 0; i < command->getSize(); ++i) {

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

  int sizeCmd = newCommand.getSize();
  String address;

  if (type == ON) {
    address = "CmdOn";
  } else if (type == OFF) {
    address = "CmdOff";
  } else {
    address = "CmdVentilate";
  }

  nvs_set_u16(handle, String(address + "Number").c_str(), sizeCmd);
  Serial.printf("%s number %lu \n ", address, sizeCmd);

  for (int i = 0; i < newCommand.getSize(); i++) {
    sizeCmd = newCommand.get(i).size;

    char stringAddress[25];
    sprintf(stringAddress, "%sSize C%d",address, i);
    esp_err_t err = nvs_set_u16(handle, stringAddress, sizeCmd);

    if (err != ESP_OK) {
      Serial.println(esp_err_to_name(err));
      return false;
    }

    sprintf(stringAddress, "%s D%d",address, i);
    if (nvs_set_blob(handle, stringAddress, newCommand.get(i).data, sizeCmd * sizeof(uint16_t)) != ESP_OK) {
      return false;
    }
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

  String address;
  uint16_t numCmd;
  uint16_t sizeCmd = 0;
  command commandLoading;

  if (type == ON) {
    address = "CmdOn";
  } else if (type == OFF) {
    address = "CmdOff";
  } else {
    address = "CmdVentilate";
  }


  nvs_get_u16(handle, String(address + "Number").c_str(), &numCmd);
  Serial.printf("%s number %lu \n ", address, numCmd);

  for (int i = 0; i < numCmd; i++) {
    esp_err_t err = nvs_get_u16(handle, String(address + "Size C" + String(i)).c_str(), &sizeCmd);
    if (err != ESP_OK) {
      nvs_close(handle);
      Serial.println("ERRO de leitura");
      Serial.println(esp_err_to_name(err));

      return false;
    } else {
      commandLoading.size = sizeCmd;
      size_t sizeData = sizeCmd * sizeof(uint16_t);

      nvs_get_blob(handle, String(address + " D" + String(i)).c_str(), commandLoading.data, &sizeData);
      newCommand.add(commandLoading);
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
