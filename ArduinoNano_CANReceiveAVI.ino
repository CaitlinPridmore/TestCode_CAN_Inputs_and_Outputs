#include <SPI.h>
#include <mcp_can.h>

// MCP2515 Pins
#define CAN_CS_PIN 10
#define CAN_INT_PIN 2

// CAN Settings
#define CAN_ID_ANALOG 0x2C0
#define CAN_SPEED CAN_1000KBPS
#define CAN_CLOCK MCP_8MHZ   // Change to MCP_16MHZ if your module uses 16MHz

MCP_CAN CAN(CAN_CS_PIN);

// Data Storage
uint16_t analogValues[4];

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Starting Nano CAN Receiver...");

  // Initialize MCP2515
  if (CAN.begin(MCP_ANY, CAN_SPEED, CAN_CLOCK) == CAN_OK) {
    Serial.println("CAN Init OK");
  } else {
    Serial.println("CAN Init FAILED");
    while (1);
  }

  CAN.setMode(MCP_NORMAL);   // Start CAN

  Serial.println("Waiting for messages...");
}

void loop() {

  // Check if message available
  if (CAN.checkReceive() == CAN_MSGAVAIL) {

    long unsigned int rxId;
    unsigned char len;
    unsigned char buf[8];

    CAN.readMsgBuf(&rxId, &len, buf);

    // Check ID
    if (rxId == CAN_ID_ANALOG && len == 8) {

      // Decode 4x 16-bit values
      for (int i = 0; i < 4; i++) {
        analogValues[i] =
          (uint16_t)buf[i * 2] |
          ((uint16_t)buf[i * 2 + 1] << 8);
      }

      // Print results
      Serial.println("Received Analog Values:");

      for (int i = 0; i < 4; i++) {
        Serial.print("A");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(analogValues[i]);
      }

      Serial.println("--------------------");
    }
  }
}

