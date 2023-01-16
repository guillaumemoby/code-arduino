#include <IRremote.h>

#define IR_RECEIVE_PIN 8

void setup() {
  Serial.begin(9600);
    Serial.println("Initialize System");
  IrReceiver.begin(IR_RECEIVE_PIN);
}

void loop() {
  if (IrReceiver.decode()) {
    IrReceiver.resume();
    Serial.println(IrReceiver.decodedIRData.command);
  }
}
