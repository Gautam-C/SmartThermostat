#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <gfxfont.h>

String input = "";

const int heatPin = 5;
const int thermPin = 0;

int vIn;
float voltage;
float temperature;

bool fahren = true;
bool outFahren = true;

float tempFinal;
float storage;
float outTemp;
bool heaterOn = true;

int timeInput;
int timeOutStart;
int timeOutEnd;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define ONE_WIRE_BUS 8

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);

DeviceAddress tempSensorAddress;

void setup() {
    Serial.begin(9600);
    
    // Configure relay
    pinMode(heatPin, OUTPUT);
    digitalWrite(heatPin, HIGH);

    // Configure Dallas tempSensor
    Serial.println("tempSensor test...");
    Serial.println("Locating tempSensor...");
    tempSensor.begin();

    if(!tempSensor.getAddress(tempSensorAddress, 0)) {
        Serial.println("Unable to find tempSensor");
    } else {
        Serial.println("Found tempSensor");
    }

    tempSensor.setResolution(tempSensorAddress, 12);

    // Configure display
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
    }

    delay(2000);
    display.clearDisplay();


    display.setTextSize(1.9);
    display.setTextColor(WHITE);

    Serial.println("Display setup finished");

    display.display();
}

void loop() {

    bool out = checkTimeOut();
    readTemp(out);
    changeHeat(out);
    displayVals();
    serialPrintln();
    
}

void readTemp(bool tfMet) {
    tempSensor.requestTemperatures();

    if (tfMet) {
      if (outFahren) {
        temperature = tempSensor.getTempF(tempSensorAddress);
      } else {
        temperature = tempSensor.getTempC(tempSensorAddress);
      }
    } else {
      if (fahren) {
        temperature = tempSensor.getTempF(tempSensorAddress);
      } else {
        temperature = tempSensor.getTempC(tempSensorAddress);
      }
    }
}

void serialEvent() {
    input = Serial.readString();
    input.trim();
    input.toLowerCase();
    parseInput(input);

}

void parseInput(String inp) {
    Serial.print("input code : ");
    Serial.println(inp.charAt(0));

    switch (inp.charAt(0)) {
      case 'f':
        fahren = true;

        if (inp.length() < 2) {
          break;
        }
        tempFinal = inp.substring(1).toFloat();
        break;
      case 'c':
        fahren = false;

        if (inp.length() < 2) {
          break;
        }
        tempFinal = inp.substring(1).toFloat();
        break;
      case 'd':
        heaterOn = false;
        break;
      case 'e':
        heaterOn = true;
        break;
      case 'o':
        if (inp.charAt(1) == 'f')  {
          outFahren = true;
        } else if (inp.charAt(1) == 'c') {
          outFahren = false;
        } else {
          Serial.println("invalid outUnit input");
        }

        outTemp = inp.substring(2, 4).toFloat();
        timeOutStart = inp.substring(4, 8).toInt();
        timeOutEnd = inp.substring(8).toInt();
        break;
      default:
        Serial.println("invalid code input");
        Serial.println("code input : " + inp.charAt(0));
        break;
    }
}

void changeHeat(bool tfMet) {

    if (!heaterOn) {
      heatOff();
      Serial.println("Heater is off for now");
      return;
    }

    if (tfMet) {
      if (temperature >= outTemp) {
        heatOff();
      } else {
        heatOn();
      }
    } else {
      if (temperature >= tempFinal) {
        heatOff();
      } else {
        heatOn();
      }
    }
}

void heatOn() {
    digitalWrite(heatPin, LOW);
}

void heatOff() {
    digitalWrite(heatPin, HIGH);
}

bool checkTimeOut() {
  if (timeInput >= timeOutStart && timeInput <= timeOutEnd) {
    return true;
  } else {
    return false; 
  }
}

void sendCurrentTemp(bool tfMet) {
  if (tfMet) {
    if (outFahren) {
      String unit = "f";
      int tempSend = outTemp;
    } else {
      String unit = "c";
      int tempSend = outTemp;
    }
  } else {
    if (fahren) {
      String unit = "f";
      int tempSend = tempFinal;
    } else {
      String unit = "c";
      int tempSend = outTemp;
    }
  }
  // format : s'u''finalTemp'c'currentTemp'
  Serial.println("s" + unit + String(tempSend) + "r" + String(temperature, 0));
}

void displayVals() {
    
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Set : " + String(tempFinal, 0));
    display.println();
    display.print("Current : " + String(temperature, 0));
    display.display();
}

void serialPrintln() {
    Serial.println("Set : " + String(tempFinal, 0));
    Serial.println();
    Serial.println("Current : " + String(temperature, 0));
}
