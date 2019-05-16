
// Include Libraries
#include "Arduino.h"
#include "DHT.h"
#include "Adafruit_SI1145.h"
#include <Wire.h>



// Pin Definitions
#define DHT_PIN_DATA	2

/////////////////////
///Order of output///
//////////////////////////
//TIME|UV|VIS|IR|TMP|HUM//
//////////////////////////

//Do we need to be able to request on demand
//Potential for warnings on 
//DO WE NEED A TIMESTAMP

// Global variables and defines

// object initialization
HardwareSerial& bthc06(Serial1);
DHT dht(DHT_PIN_DATA);
Adafruit_SI1145 uv = Adafruit_SI1145();


// define vars for testing menu
const int timeout = 5000;       //define timeout of 10 sec

const int UV_WARN = 1;
const int VIS_WARN = 500;
const int IR_WARN = 1000;
const int TMP_WARN = 24; //C
const int HUM_WARN = 30; //%

String menuOption = "1";
long time0;
String outputString = " ";

// Setup the essentials for your circuit to work.
//It runs first every time your circuit is powered with electricity.
void setup() {
  // Setup Serial which is useful for debugging
  // Use the Serial Monitor to view printed messages
  Serial.begin(9600);
  while (!Serial) ; // wait for serial port to connect. Needed for native USB
  Serial.println("start");
  //This example uses HC-06 Bluetooth to communicate with an Android device.
  //Download bluetooth terminal from google play store,
  //https://play.google.com/store/apps/details?id=Qwerty.BluetoothTerminal&hl=en
  //Pair and connect to 'HC-06', the default password for connection is '1234'.
  //You should see this message from your arduino on your android device
  bthc06.begin(9600);
  Serial.println("HC-06 Bluetooth: On");
  bthc06.println("HC-06 Bluetooth: On");

  dht.begin();
  Serial.println("DHT22/11 Humidity and Temperature Sensor: On");

  if (uv.begin()) {
    Serial.println("SI1145 Digital UV Index / IR / Visible Light Sensor: On");
  } else {
    Serial.println("SI1145 Digital UV Index / IR / Visible Light Sensor: NA");
  }

}

// Main logic of your circuit.
//It defines the interaction between the components you selected.
//After setup, it runs over and over again, in an eternal loop.
void loop() {
  if (millis() > time0 + timeout) {
    time0 = millis();
    displaySensorVals();
  }
  else if (menuOption == "1") {
    displaySensorVals();
    menuOption = "0";
  }

  menuOption = menu();
  checkWarnings();

}

// Menu function for selecting the components to be tested
// Follow serial monitor for instrcutions
String menu() {

  //bthc06.println(F("\nTo request data, Enter '1' "));
  //bthc06.println(F("\n"));
  //while (!bthc06.available());

  // Read data from serial monitor if received
  //Serial.println(time0 + timeout);
  //Serial.println(millis());
  // HC - 06 Bluetooth Serial Module - Test Code
  String bthc06Str = "";
  //Receive String from bluetooth device
  while (bthc06.available())
  {
    Serial.println("Reading");
    //Read a complete line from bluetooth terminal
    bthc06Str = bthc06.readStringUntil('\n');
    // Print raw data to serial monitor
    Serial.print("BT Raw Data: ");
    Serial.println(bthc06Str);
  }
  //Send sensor data to Bluetooth device
  //bthc06.println("PUT YOUR SENSOR DATA HERE");

  //char c = bthc06.read();
  return bthc06Str;
}


void displaySensorVals() {
  float t = 0+millis();
  float UV = getUV();
  float VIS = getVIS();
  float IR = getIR();
  float TMP = getTMP();
  float HUM = getHUM();
  
  outputString = "";
  outputString += String(t, 0);
  outputString += F(":");
  outputString += String(UV, 2);
  outputString += F(":");
  outputString += String(VIS, 0);
  outputString += F(":");
  outputString += String(IR, 0);
  outputString += F(":");
  outputString += String(TMP, 1);
  outputString += F(":");
  outputString += String(HUM, 1);
  outputString += F("/");

  Serial.println(outputString);
  bthc06.println(outputString);
}

void checkWarnings(){

  bool danger = false;

  outputString = "WARNINGS: ";
  
  float UV = getUV();
  float VIS = getVIS();
  float IR = getIR();
  float TMP = getTMP();
  float HUM = getHUM();

  if (UV > UV_WARN){
    danger = true;
    outputString += F("UV ");
  }
  if (VIS > VIS_WARN){
    danger = true;
    outputString += F("VIS ");
  }
  if (IR > IR_WARN){
    danger = true;
    outputString += F("IR ");
  }
  if (TMP > TMP_WARN){
    danger = true;
    outputString += F("TMP ");
  }
  if (HUM > HUM_WARN){
    danger = true;
    outputString += F("HUM ");

  }

  if (danger){
    Serial.println(outputString);
    bthc06.println(outputString);
    displaySensorVals();
    delay(1000);
  }
  
}

float getUV(){
  float UV = uv.readUV();
  // the index is multiplied by 100 so to get the
  // integer index, divide by 100!
  UV /= 100.0;
  return UV;
}

float getVIS(){
  return uv.readVisible();
}

float getIR(){
  return uv.readIR();
}

float getTMP(){
  // Read temperature in Celsius, for Fahrenheit use .readTempF()
  return dht.readTempC();
}

float getHUM(){
  // Reading humidity in %
  return dht.readHumidity();
}

/*******************************************************

     Circuito.io is an automatic generator of schematics and code for off
     the shelf hardware combinations.

     Copyright (C) 2016 Roboplan Technologies Ltd.

     This program is free software: you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation, either version 3 of the License, or
     (at your option) any later version.

     This program is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.

     In addition, and without limitation, to the disclaimers of warranties
     stated above and in the GNU General Public License version 3 (or any
     later version), Roboplan Technologies Ltd. ("Roboplan") offers this
     program subject to the following warranty disclaimers and by using
     this program you acknowledge and agree to the following:
     THIS PROGRAM IS PROVIDED ON AN "AS IS" AND "AS AVAILABLE" BASIS, AND
     WITHOUT WARRANTIES OF ANY KIND EITHER EXPRESS OR IMPLIED.  ROBOPLAN
     HEREBY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT
     NOT LIMITED TO IMPLIED WARRANTIES OF MERCHANTABILITY, TITLE, FITNESS
     FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, AND THOSE ARISING BY
     STATUTE OR FROM A COURSE OF DEALING OR USAGE OF TRADE.
     YOUR RELIANCE ON, OR USE OF THIS PROGRAM IS AT YOUR SOLE RISK.
     ROBOPLAN DOES NOT GUARANTEE THAT THE PROGRAM WILL BE FREE OF, OR NOT
     SUSCEPTIBLE TO, BUGS, SECURITY BREACHES, OR VIRUSES. ROBOPLAN DOES
     NOT WARRANT THAT YOUR USE OF THE PROGRAM, INCLUDING PURSUANT TO
     SCHEMATICS, INSTRUCTIONS OR RECOMMENDATIONS OF ROBOPLAN, WILL BE SAFE
     FOR PERSONAL USE OR FOR PRODUCTION OR COMMERCIAL USE, WILL NOT
     VIOLATE ANY THIRD PARTY RIGHTS, WILL PROVIDE THE INTENDED OR DESIRED
     RESULTS, OR OPERATE AS YOU INTENDED OR AS MAY BE INDICATED BY ROBOPLAN.
     YOU HEREBY WAIVE, AGREE NOT TO ASSERT AGAINST, AND RELEASE ROBOPLAN,
     ITS LICENSORS AND AFFILIATES FROM, ANY CLAIMS IN CONNECTION WITH ANY OF
     THE ABOVE.
********************************************************/
