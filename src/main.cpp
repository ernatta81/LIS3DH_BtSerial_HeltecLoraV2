#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBt;
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

#define SerialPortBt SerialBt
#define SDA_PIN 21
#define SCL_PIN 22

const char *pin = "0000"; // Bluetooth il PIN
bool startReceived = false; // Variabile per "START"

void configureAccelerometerSettings(); // Dichiarazione della funzione altrimenti non compila....
void configureLISPerformance();
void configureLISRange();
void configureLISDatarate();
void printLIS3Configuration();

void setup(void) {
  SerialPortBt.begin("V2LIS3DH");
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!lis.begin(0x19)) {
      Serial.println("LIS3DH not start");
      SerialPortBt.println("LIS3DH not start");
      while (1) yield();
    }
    Serial.println("LIS3DH found!");
  
  while (!SerialBt) delay(10);
  SerialBt.println("LIS3DH test!");
  Serial.println("LIS3DH test!");

  while (!SerialPortBt.hasClient()) {
    Serial.println("Waiting bluetooth connection...");
    delay(500); 
  }
  Serial.println("Bluetooth connesso!");

  Serial.println("Vuoi configurare l'accelerometro? (S/N)");
  SerialPortBt.println("Vuoi configurare l'accelerometro? (S/N)");
  while (true) {
    if (SerialPortBt.available()) {
      char response = SerialPortBt.read();
      if (response == 'S' || response == 's') {
        Serial.println("Configurazione accelerometro...");
        SerialPortBt.println("Configurazione accelerometro...");
        configureAccelerometerSettings(); // Funzione per la configurazione
        printLIS3Configuration();
        break;
      } else if (response == 'N' || response == 'n') {
        Serial.println("Skipping configuration. Waiting for START command...");
        SerialPortBt.println("Skipping configuration. Waiting for START command...");
        printLIS3Configuration();
        
        break;
      }
    }
  }

  Serial.println("Waiting receive START command");
  SerialPortBt.println("Waiting receive START command");
}

void configureAccelerometerSettings() {
  configureLISPerformance();
  configureLISRange();
  configureLISDatarate();
}
  
char dato[200];

void loop() {
  if (SerialPortBt.available()) {
    String command = SerialPortBt.readStringUntil('\n');  // Leggi il comando ricevuto
    if (command == "START") {
      startReceived = true;  // "START"
      Serial.println("Received START command. Beginning sensor data output...");
      SerialPortBt.println("Received START command. Beginning sensor data output...");
    } 
    else if (command == "STOP") {
      startReceived = false;  // "STOP"
      Serial.println("Received STOP command. Stopping sensor data output...");
      SerialPortBt.println("Received STOP command. Stopping sensor data output...");
    }
  }

  if (startReceived) {
    if ( lis.haveNewData() ) {
      lis.read();  // Ottieni i dati X Y e Z
      sensors_event_t event;
      lis.getEvent(&event);
      snprintf(dato,200,"%,%d,%d,%d,%f,%f,%f\r\n", millis(), lis.x, lis.y, lis.z, event.acceleration.x ,event.acceleration.y , event.acceleration.z );
      //Serial.print(" m/s^2 ");
      SerialPortBt.print(dato);
    }
    


#if 0
    // Dati grezzi
    Serial.print("X:  "); Serial.print(lis.x);
    Serial.print("  \tY:  "); Serial.print(lis.y);
    Serial.print("  \tZ:  "); Serial.print(lis.z);

    // Stampa su SerialPortBt
    SerialPortBt.print("X:  "); SerialPortBt.print(lis.x);
    SerialPortBt.print("  \tY:  "); SerialPortBt.print(lis.y);
    SerialPortBt.print("  \tZ:  "); SerialPortBt.print(lis.z);

    /* Ottieni un nuovo evento dal sensore, normalizzato */
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
#endif
  }

}

void configureLISRange(){
  Serial.println("Set range to 2G, 4G, 8G or 16G?");
  SerialPortBt.println("Set range to 2G, 4G, 8G or 16G?");
   while (true) {
    if (SerialPortBt.available()) {
      String range = SerialPortBt.readStringUntil('\n');
      if (range == "2G") {
        lis.setRange(LIS3DH_RANGE_2_G);
        Serial.println("Range set to 2G");
        SerialPortBt.println("Range set to 2G");
        break;
      } else if (range == "4G") {
        lis.setRange(LIS3DH_RANGE_4_G);
        Serial.println("Range set to 4G");
        SerialPortBt.println("Range set to 4G");
        break;
      } else if (range == "8G") {
        lis.setRange(LIS3DH_RANGE_8_G);
        Serial.println("Range set to 8G");
        SerialPortBt.println("Range set to 8G");
        break;
      } else if (range == "16G") {
        lis.setRange(LIS3DH_RANGE_16_G);
        Serial.println("Range set to 16G");
        SerialPortBt.println("Range set to 16G");
        break;
      }
    }
  }



}


void configureLISDatarate(){
   Serial.println("Set datarate to 1HZ, 10HZ, 25HZ, 50HZ, 100Hz, 200HZ, 400HZ, POWERDOWN, 5KHZ, 1K6HZ");
  SerialPortBt.println("Set datarate to 1HZ, 10HZ, 25HZ, 50HZ, 100Hz, 200HZ, 400HZ, POWERDOWN, 5KHZ, 1K6HZ");
   while (true) {
    if (SerialPortBt.available()) {
      String range = SerialPortBt.readStringUntil('\n');
      if (range == "1HZ") {
        lis.setDataRate(LIS3DH_DATARATE_1_HZ);
        Serial.println("Datarate set to 1HZ");
        SerialPortBt.println("Datarate set to 1HZ");
        break;
      } else if (range == "10HZ") {
        lis.setDataRate(LIS3DH_DATARATE_10_HZ);
        Serial.println("Datarate set to 10HZ");
        SerialPortBt.println("Datarate set to 10HZ");
        break;
      } else if (range == "25HZ") {
        lis.setDataRate(LIS3DH_DATARATE_25_HZ);
        Serial.println("Datarate set to 25HZ");
        SerialPortBt.println("Datarate set to 25HZ");
        break;
      } else if (range == "50HZ") {
        lis.setDataRate(LIS3DH_DATARATE_50_HZ);
        Serial.println("Datarate set to 50HZ");
        SerialPortBt.println("Datarate set to 50HZ");
        break;
      } else if (range == "100HZ") {
        lis.setDataRate(LIS3DH_DATARATE_100_HZ);
        Serial.println("Datarate set to 100HZ");
        SerialPortBt.println("Datarate set to 100HZ");
        break;
      } else if (range == "200HZ") {
        lis.setDataRate(LIS3DH_DATARATE_200_HZ);
        Serial.println("Datarate set to 200HZ");
        SerialPortBt.println("Datarate set to 200HZ");
        break;
      } else if (range == "400HZ") {
        lis.setDataRate(LIS3DH_DATARATE_400_HZ);
        Serial.println("Datarate set to 400HZ");
        SerialPortBt.println("Datarate set to 400HZ");
        break;
      } else if (range == "POWERDOWN") {
        lis.setDataRate(LIS3DH_DATARATE_POWERDOWN);
        Serial.println("Datarate set to POWERDOWN");
        SerialPortBt.println("Datarate set to POWERDOWN");
        break;
      } else if (range == "5KHZ") {
        lis.setDataRate(LIS3DH_DATARATE_LOWPOWER_5KHZ);
        Serial.println("Datarate set to 5KHZ");
        SerialPortBt.println("Datarate set to 5KHZ");
        break;
      } else if (range == "10K6Z") {
        lis.setDataRate(LIS3DH_DATARATE_LOWPOWER_1K6HZ);
        Serial.println("Datarate set to 10K6Z");
        SerialPortBt.println("Datarate set to 10K6Z");
        break;
      }
     }  
    }


}

void configureLISPerformance(){
    Serial.println("Performance mode set to: NORMAL 10bit / LOW Power 8bit / HIGH Resolution 12bit");
  SerialPortBt.println("Performance mode set to: NORMAL 10bit / LOW Power 8bit / HIGH Resolution 12bit");
  while (true) {
    if (SerialPortBt.available()) {
      String range = SerialPortBt.readStringUntil('\n');
      if (range == "NORMAL") {
        lis.setPerformanceMode(LIS3DH_MODE_NORMAL);
        Serial.println("Set to NORMAL");
        SerialPortBt.println("Set to NORMAL");
        break;
      } else if (range == "LOW") {
        lis.setPerformanceMode(LIS3DH_MODE_LOW_POWER);
        Serial.println("Set to LOW");
        SerialPortBt.println("Set to LOW");
        break;
      } else if (range == "HIGH") {
        lis.setPerformanceMode(LIS3DH_MODE_HIGH_RESOLUTION);
        Serial.println("Set to HIGH");
        SerialPortBt.println("Set to HIGH");
        break;
      }
    }
  }
}

void printLIS3Configuration(){
      delay(500);
      Serial.println("- - - LIS3DH parameter - - -");
      SerialPortBt.print("- - - LIS3DH parameter - - -");  
      // STAMPA delle impostazioni su seriale 

      // lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 o 16 G!
      Serial.print("Range = "); 
      Serial.print(2 << lis.getRange());
      Serial.println("G");
      SerialPortBt.print("Range = ");   
      SerialPortBt.print(2 << lis.getRange());
      SerialPortBt.println("G");

      // lis.setPerformanceMode(LIS3DH_MODE_LOW_POWER);
      Serial.print("Performance mode set to: ");
      SerialPortBt.print("Performance mode set to: ");
      switch (lis.getPerformanceMode()) {
        case LIS3DH_MODE_NORMAL: 
            Serial.println("Normal 10bit"); 
            SerialPortBt.println("Normal 10bit");        
            break;
        case LIS3DH_MODE_LOW_POWER: 
            Serial.println("Low Power 8bit"); 
            SerialPortBt.println("Low Power 8bit"); 
            break;
        case LIS3DH_MODE_HIGH_RESOLUTION: 
            Serial.println("High Resolution 12bit"); 
            SerialPortBt.println("High Resolution 12bit"); 
            break;
      }
      // lis.setDataRate(LIS3DH_DATARATE_50_HZ);
      Serial.print("Data rate set to: ");
      SerialPortBt.print("Data rate set to: ");
      switch (lis.getDataRate()) {
        case LIS3DH_DATARATE_1_HZ: 
            Serial.println("1 Hz"); 
            SerialPortBt.println("1 Hz"); 
            break;
        case LIS3DH_DATARATE_10_HZ: 
            Serial.println("10 Hz"); 
            SerialPortBt.println("10 Hz"); 
            break;
        case LIS3DH_DATARATE_25_HZ: 
            Serial.println("25 Hz"); 
            SerialPortBt.println("25 Hz"); 
            break;
        case LIS3DH_DATARATE_50_HZ: 
            Serial.println("50 Hz"); 
            SerialPortBt.println("50 Hz"); 
            break;
        case LIS3DH_DATARATE_100_HZ: 
            Serial.println("100 Hz"); 
            SerialPortBt.println("100 Hz"); 
            break;
        case LIS3DH_DATARATE_200_HZ: 
            Serial.println("200 Hz"); 
            SerialPortBt.println("200 Hz"); 
            break;
        case LIS3DH_DATARATE_400_HZ: 
            Serial.println("400 Hz"); 
            SerialPortBt.println("400 Hz"); 
            break;
        case LIS3DH_DATARATE_POWERDOWN: 
            Serial.println("Powered Down"); 
            SerialPortBt.println("Powered Down"); 
            break;
        case LIS3DH_DATARATE_LOWPOWER_5KHZ: 
            Serial.println("5 Khz Low Power"); 
            SerialPortBt.println("5 Khz Low Power"); 
            break;
        case LIS3DH_DATARATE_LOWPOWER_1K6HZ: 
            Serial.println("1.6 Khz Low Power"); 
            SerialPortBt.println("1.6 Khz Low Power"); 
            break;
       }
}