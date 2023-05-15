#include <DHTesp.h>
//temp
#define WORK_PIN 13
//display
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//rgb
#include <Adafruit_NeoPixel.h>
#define PIN 15
#define NUM 3
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM, PIN, NEO_GRB + NEO_KHZ800);

//temp
DHTesp dht;

//display
/*
   SCL GPIO 22
  SDA GPIO 21
*/
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


float humidity ;
float temperature;
float coLevel;

void setup() {
  Serial.begin(115200);
  dht.setup(WORK_PIN, DHTesp::DHT22);

  //mq7
  //pinMode(34, INPUT);


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(255, 255, 0));
  pixels.setBrightness(1);
  pixels.show();
  pixels.clear();
}

void loop() {
  dhtSensor();
  testDisplay(temperature, humidity, coLevel);
  startuptest();
  readMQ7();
  // put your main code here, to run repeatedly:

}

void dhtSensor() {
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  serialLogging("HUM: " + String(humidity, 2));
  serialLogging("TMP: " + String(temperature, 2));
  delay(1000);
}

void testDisplay(float temp, float hum, float co) {

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("TMP :" + String(temp, 2));
  display.display();
  delay(1000);
  display.println("HUM :" + String(hum, 2));
  display.display();
  delay(1000);
  display.println("CO  :" + String(co, 2));
  display.display();
  display.clearDisplay();

}

void startuptest()  {
  /*


    red: (255, 0, 0)
    green: (0, 255, 0)
    blue: (0, 0, 255)
    cyan: (0, 255, 255)
    purple: (255, 0, 255)
    yellow: (255, 255, 0)
    white: (255, 255, 255)
    black (off): (0, 0, 0)

  */

  for (int i = 1; i < NUM; i++)
  {
    pixels.clear();
    pixels.setPixelColor(i, pixels.Color(0, 0, 255));
    pixels.show();
    delay(300);
    pixels.clear();
    pixels.setPixelColor(i, pixels.Color(0, 255, 0));
    pixels.show();
    delay(300);
    pixels.clear();
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    pixels.show();
    delay(300);
    pixels.clear();
  }


}

void readMQ7() {

  float coefficient_A = 19.32;
  float coefficient_B = -0.64;
  float v_in = 3.3; //sensor power volt
  float ppm = 0.0;

  //get ratio
  int value = analogRead(34);
  float v_out = value * (v_in / 1023.0);
  float ratio = (v_in - v_out) / v_out;

  ppm = coefficient_A * pow(ratio, coefficient_B);
  coLevel = ppm;

  serialLogging("CO :" + String(coLevel, 2));


}

void serialLogging(String msg) {

  //for handle logging

  if (true) {
    Serial.println(msg);
  }
}
