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

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void setup()
{
  Serial.begin(115200);

  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(0x02, 0x3c, false, false))
  {
    Serial.println(F("OLED initiate failed!"));
  }

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

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("LORA ID 2");
  display.display();
}

void loop()
{
  Serial.print("Sending packet: ");

  LoRa.beginPacket();
  LoRa.print('#');
  LoRa.print(2);
  LoRa.print(',');
  LoRa.print(random(0.0, 2.5));
  LoRa.print(',');
  LoRa.print(random(0, 220));
  LoRa.print(',');
  LoRa.print(random(0.0, 2.5));
  LoRa.print(',');
  LoRa.print(random(0, 220));
  LoRa.print(',');
  LoRa.print(random(0.0, 2.5));
  LoRa.print(',');
  LoRa.print(random(0, 220));
  LoRa.print(';');

  LoRa.endPacket();

  Serial.println("LoRa packet sent.");

  delay(10000 + random(0, 2000));
}