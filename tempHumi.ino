
// Temperature display using DHT22 sensor, Waveshare 2.9" e-ink display and Lolin D1 mini

#include <GxEPD.h>
#include <DHT.h>

// 2.9inch black/white/red display from Waveshare
#include <GxGDEW029Z10/GxGDEW029Z10.cpp>

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBold24pt7b.h>

#include <GxIO/GxIO_SPI/GxIO_SPI.cpp>
#include <GxIO/GxIO.cpp>

#define DHTPIN D1
#define DHTTYPE DHT22

#define SLEEPSECONDS 60

#define TEMPERATUREHEADERXPOS 3
#define TEMPERATUREHEADERYPOS 20
#define TEMPERATUREVALUEXPOS 5
#define TEMPERATUREVALUEYPOS 90
#define TEMPERATURETEXT "TEMPERATURE"

#define DIVIDERXPOS 0
#define DIVIDERYPOS 147
#define DIVIDERXSIZE 148
#define DIVIDERYSIZE 2

#define HUMIDITYHEADERXPOS 8
#define HUMIDITYHEADERYPOS 170
#define HUMIDITYVALUEXPOS 5
#define HUMIDITYVALUEYPOS 240
#define HUMIDITYTEXT "HUMIDITY %"

GxIO_Class io(SPI, SS, 0, 2);
GxEPD_Class display(io);
DHT dht(DHTPIN, DHTTYPE);

const GFXfont* f_small = &FreeMonoBold9pt7b;
const GFXfont* f_large = &FreeMonoBold24pt7b;

void setup() {
  display.init(115200);
  display.setRotation(0);
  display.setTextColor(GxEPD_BLACK);
  dht.begin();
}

void loop() {
  // Reads the temperature as celcius. Set the parameter to true for Fahrenheit.
  float temperature = dht.readTemperature(false);
  float humidity = dht.readHumidity();
  fullScreenUpdate(temperature, humidity);

  /* 
   * This requires pins 16 (D0) and rst to be connected to each other.
   * If you want you could change the time to sleep to a small value,
   * and add a button between the pins to only update when button is pressed.
   */
  ESP.deepSleep(SLEEPSECONDS * 1000 * 1000, WAKE_RF_DISABLED);
}

// fullScreenUpdate takes the values as parameters and prints them on the screen
void fullScreenUpdate(float temperature, float humidity) {
  // Clear the screen
  display.fillScreen(GxEPD_WHITE);

  // Print the temperature header
  display.setCursor(TEMPERATUREHEADERXPOS, TEMPERATUREHEADERYPOS);
  display.setFont(f_small);
  display.println(TEMPERATURETEXT);

  // Print the temperature value
  display.setFont(f_large);
  display.setCursor(TEMPERATUREVALUEXPOS, TEMPERATUREVALUEYPOS);
  // Print temperature value with 1 decimal
  display.println(temperature, 1);

  // Print the divider
  display.fillRect(DIVIDERXPOS, DIVIDERYPOS, DIVIDERXSIZE, DIVIDERYSIZE, GxEPD_BLACK);

  // Print the humidity header
  display.setCursor(HUMIDITYHEADERXPOS, HUMIDITYHEADERYPOS);
  display.setFont(f_small);
  display.println(HUMIDITYTEXT);

  // Print the humidity value
  display.setFont(f_large);
  display.setCursor(HUMIDITYVALUEXPOS, HUMIDITYVALUEYPOS);
  // Print humidity value with 1 decimal
  display.println(humidity, 1);

  // Update the display
  display.update();
}

