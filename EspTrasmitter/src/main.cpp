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

#define LORA_ID 1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

float i0 = 0;
float v0 = 0;
float i1 = 0;
float v1 = 0;
float i2 = 0;
float v2 = 0;

unsigned long oscClock = 2000;
unsigned long taskClock = millis();
unsigned long oledClock = millis();

void lora_Transmitter()
{
}

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
  display.print("LORA ID 1");
  display.display();
}

void loop()
{
  if ((millis() - taskClock) > oscClock)
  {
    Serial.print("Sending packet: ");

    i0 = random(0.0, 2.5);
    v0 = random(0, 220);
    i1 = random(0.0, 2.5);
    v1 = random(0, 220);
    i2 = random(0.0, 2.5);
    v2 = random(0, 220);

    unsigned long CHECKSUM = i0 || v0 || i1 || v1 || i2 || v2;

    LoRa.beginPacket();
    LoRa.print('#');
    LoRa.print(LORA_ID);
    LoRa.print(',');
    LoRa.print(i0);
    LoRa.print(',');
    LoRa.print(v0);
    LoRa.print(',');
    LoRa.print(i1);
    LoRa.print(',');
    LoRa.print(v1);
    LoRa.print(',');
    LoRa.print(i2);
    LoRa.print(',');
    LoRa.print(v2);
    LoRa.print(',');
    LoRa.print(CHECKSUM);
    LoRa.print(';');

    LoRa.endPacket();

    Serial.println("LoRa packet sent.");

    taskClock = millis();
    oscClock = random(3000,4000);
  }
  if ((millis() - oledClock) > 750)
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("LORA ID 1");
    display.setCursor(0, 13);
    display.print("I0=>");
    display.print(i0);
    display.print(";");
    display.print("v0=>");
    display.print(v0);
    display.setCursor(0, 23);
    display.print("I1=>");
    display.print(i1);
    display.print(";");
    display.print("v1=>");
    display.print(v1);
    display.setCursor(0, 33);
    display.print("I2=>");
    display.print(i2);
    display.print(";");
    display.print("v2=>");
    display.print(v2);
    display.setCursor(0, 43);
    unsigned long CHECKSUM = i0 || v0 || i1 || v1 || i2 || v2;
    display.print(CHECKSUM);
    display.display();

    oledClock = millis();
  }
}