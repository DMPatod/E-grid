#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>

#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

#define BAND 915E6

String LoRaData;

void setup()
{
  Serial.begin(115200);

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND))
  {
    Serial.println("Starting LoRa failed!");
    while (1)
    {
      delay(1000);
      Serial.println("Starting LoRa failed!");
    };
  }
}

void loop()
{
  int pkgSize = LoRa.parsePacket();
  if (pkgSize)
  {
    Serial.println("Received Packet:");

    while (LoRa.available())
    {
      LoRaData = LoRa.readString();
      Serial.print(LoRaData);
    }

    int rssi = LoRa.packetRssi();
    Serial.print(" with RSSI ");
    Serial.println(rssi);
  }
}