#include <Arduino.h>
#include <Arduino_CAN.h>

// CAN Address
const uint32_t AVIsendCANAddress = 0x2C0;

// Analog
#define NUM_ANALOG 4

const uint8_t analogPins[NUM_ANALOG] = { A0, A1, A2, A3 };

uint16_t analogValues[NUM_ANALOG];

// Setup
void setup() {

  Serial.begin(115200);
  while (!Serial);

  Serial.println("UNO R4 Analog CAN Sender");

  if (!CAN.begin(CanBitRate::BR_1000k)) {
    Serial.println("CAN Init Failed!");
    while (1);
  }

  Serial.println("CAN Ready");
}

// Read Analog
void readAnalogInputs() {

  for (int i = 0; i < NUM_ANALOG; i++) {
    analogValues[i] = analogRead(analogPins[i]);
  }
}

// Send CAN
void sendAnalogCAN() {

  uint8_t data[8];

  for (int i = 0; i < NUM_ANALOG; i++) {

    data[i * 2]     = lowByte(analogValues[i]);
    data[i * 2 + 1] = highByte(analogValues[i]);
  }

  // Construct CAN message (STANDARD ID)
  CanMsg msg(
    CanStandardId(AVIsendCANAddress),
    8,
    data
  );

  int rc = CAN.write(msg);

  if (rc < 0) {
    Serial.print("CAN send failed: ");
    Serial.println(rc);
  }
}

// Debug
void printAnalog() {

  Serial.print("Analog: ");

  for (int i = 0; i < NUM_ANALOG; i++) {

    Serial.print("A");
    Serial.print(i);
    Serial.print("=");

    Serial.print(analogValues[i]);

    if (i < NUM_ANALOG - 1)
      Serial.print(" | ");
  }

  Serial.println();
}

// Main Loop
void loop() {

  readAnalogInputs();

  sendAnalogCAN();

  printAnalog();

  delay(100);
}



