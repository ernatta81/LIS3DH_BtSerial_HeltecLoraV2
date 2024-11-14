#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBt;
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

#define SerialPortBt SerialBt
#define SDA_PIN 21
#define SCL_PIN 22

const char *pin = "0000";// Definisci il PIN
bool startReceived = false; // Variabile per  "START"

void setup(void) {
  SerialPortBt.begin("V2LIS3DH");
  //SerialPortBt.setPin(pin); // Abilita il PIN
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  while (!SerialBt) delay(10);
  SerialBt.println("LIS3DH test!");
  Serial.println("LIS3DH test!");

  while (!SerialPortBt.hasClient()) {
    Serial.println("Waiting bluetooth connection...");
    delay(500); 
  }
  Serial.println("Bluetooth connesso!");

  if (!lis.begin(0x19)) {
    Serial.println("LIS3DH not start");
    SerialPortBt.println("LIS3DH not start");
    while (1) yield();
  }
  Serial.println("LIS3DH found!");

  // lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 o 16 G!
  Serial.print("Range = "); Serial.print(2 << lis.getRange());
  Serial.println("G");

  // lis.setPerformanceMode(LIS3DH_MODE_LOW_POWER);
  Serial.print("Performance mode set to: ");
  switch (lis.getPerformanceMode()) {
    case LIS3DH_MODE_NORMAL: Serial.println("Normal 10bit"); break;
    case LIS3DH_MODE_LOW_POWER: Serial.println("Low Power 8bit"); break;
    case LIS3DH_MODE_HIGH_RESOLUTION: Serial.println("High Resolution 12bit"); break;
  }

  // lis.setDataRate(LIS3DH_DATARATE_50_HZ);
  Serial.print("Data rate set to: ");
  switch (lis.getDataRate()) {
    case LIS3DH_DATARATE_1_HZ: Serial.println("1 Hz"); break;
    case LIS3DH_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3DH_DATARATE_25_HZ: Serial.println("25 Hz"); break;
    case LIS3DH_DATARATE_50_HZ: Serial.println("50 Hz"); break;
    case LIS3DH_DATARATE_100_HZ: Serial.println("100 Hz"); break;
    case LIS3DH_DATARATE_200_HZ: Serial.println("200 Hz"); break;
    case LIS3DH_DATARATE_400_HZ: Serial.println("400 Hz"); break;
    case LIS3DH_DATARATE_POWERDOWN: Serial.println("Powered Down"); break;
    case LIS3DH_DATARATE_LOWPOWER_5KHZ: Serial.println("5 Khz Low Power"); break;
    case LIS3DH_DATARATE_LOWPOWER_1K6HZ: Serial.println("1.6 Khz Low Power"); break;
  }
}

void loop() {
  if (SerialPortBt.available()) {
    String command = SerialPortBt.readStringUntil('\n');  // Leggi il comando ricevuto
    if (command == "START") {
      startReceived = true; //  "START"
      Serial.println("Received START command. Beginning sensor data output...");
      SerialPortBt.println("Received START command. Beginning sensor data output...");
    }
  }

  if (startReceived) {
    lis.read();  // ottieni i dati X Y e Z
    // dati grezzi
    Serial.print("X:  "); Serial.print(lis.x);
    Serial.print("  \tY:  "); Serial.print(lis.y);
    Serial.print("  \tZ:  "); Serial.print(lis.z);

    // Stampa su SerialPortBt
    SerialPortBt.print("X:  "); SerialPortBt.print(lis.x);
    SerialPortBt.print("  \tY:  "); SerialPortBt.print(lis.y);
    SerialPortBt.print("  \tZ:  "); SerialPortBt.print(lis.z);

    /* ottieni un nuovo evento dal sensore, normalizzato */
    sensors_event_t event;
    lis.getEvent(&event);

    /* Mostra i risultati (l'accelerazione è misurata in m/s^2) */
    Serial.print("\t\tX: "); Serial.print(event.acceleration.x);
    Serial.print(" \tY: "); Serial.print(event.acceleration.y);
    Serial.print(" \tZ: "); Serial.print(event.acceleration.z);
    Serial.println(" m/s^2 ");

    SerialPortBt.print("\t\tX: "); SerialPortBt.print(event.acceleration.x);
    SerialPortBt.print(" \tY: "); SerialPortBt.print(event.acceleration.y);
    SerialPortBt.print(" \tZ: "); SerialPortBt.print(event.acceleration.z);
    SerialPortBt.println(" m/s^2 ");

    Serial.println();
  }

  delay(200);
}
