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
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


float humidity ;
float temperature;

void setup() {
  Serial.begin(115200);
  dht.setup(WORK_PIN, DHTesp::DHT22);


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  pixels.begin();

}

void loop() {
  dhtSensor();
  testDisplay(temperature, humidity);
  startuptest();
  // put your main code here, to run repeatedly:

}

void dhtSensor() {
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  Serial.println(humidity);
  Serial.println(temperature);
  delay(1000);
}

void testDisplay(float temp, float hum) {

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("TMP :"+ String(temp,2));
  display.display();
  delay(1000);
  display.println("HUM :" + String(hum,2));
  display.display();


}

void startuptest()  {

    for (int i = 0; i < NUM; i++)
    {
      pixels.setPixelColor(i, pixels.Color(0, 0, 255));
      pixels.show();
      delay(500);
      pixels.setPixelColor(i, pixels.Color(0, 255, 0));
      pixels.show();
      delay(500);
      pixels.setPixelColor(i, pixels.Color(255, 0, 0));
      pixels.show();
      delay(500);
    }
  

}
