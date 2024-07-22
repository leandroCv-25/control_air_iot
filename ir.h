#ifndef ir_H_
#define ir_H_

#include <IRsend.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRutils.h>
#include <List.hpp>

#define kCaptureBufferSize 1024
#define kTimeout 50
#define kFrequency 38000
#define recvPin 19
#define senderPin 18

typedef struct no{
  uint16_t  data[kCaptureBufferSize];
  uint16_t size;
} command;

List<command> newCommand;

// The IR transmitter.
IRsend irsend(senderPin);
// The IR receiver.
IRrecv irrecv(recvPin, kCaptureBufferSize, kTimeout, false);

void irBegin();
bool irCommandCopy();
void irTestNewCommand();

#endif