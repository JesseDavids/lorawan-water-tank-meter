/*
Ultrasonic Water Level Monitoring System (LORAWAN)
Board(s): MKRWAN 1310 
Sensor used: JSN-SR04T (not the enclosed water-proof version)
Antenna: SMA connector to a more powerful antenna

created by Jesse Davids
*/

//<Arduino.h> is used in conjuction with platofrmio, it is mandatory. If using the Arduino IDE you can remove it.
#include <Arduino.h>
//Lib to put the MKRWAN board to sleep 
#include <RTCZero.h>
//Lib to use the LoraWAN features built for the board
#include <MKRWAN.h>
//Initialize library
RTCZero rtc;
//Set rtc. 
const byte seconds = 0;
const byte minutes = 00;
const byte hours = 00;
const byte day = 14;
const byte month = 1;
const byte year = 21;
//define pins of ultrasonic sensor
#define trig 6
#define echo 7

LoRaModem modem;
// Uncomment below line if using the Murata chip as a module
// LoRaModem modem(Serial1)

//Below information should be tailored to your own device. Each device should be unique.
//Your device should be on ABP mode when registered to TTN.
// Device Address, Network Session Key, App Session Key
String devaddr = "";
String nsk = "";
String appSk = "";

void setup() {
  //This delay is to ensure you have enough time to reprogram the mcu if something goes wrong.. you have 5 seconds to upload new code
  delay(5000);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT_PULLUP);
  Serial.begin(9600);
  // change this to your regional band (eg. US915, AS923, ...)
  if (!modem.begin(EU868)) {
    Serial.println("Failed to start module");
    while (1) {}
  };
  //int connected = modem.joinOTAA(appEui, appKey); THIS IS FOR OTAA 
  int connected = modem.joinABP(devaddr, nsk, appSk);
  if (!connected) {
    Serial.println("Something went wrong; are you indoor? Move near a window and retry");
    while (1) {}
  }
  // Set poll interval to 60 secs. I tried various values, 60 has been the best so far
  modem.minPollInterval(60);
}
void loop() {
  //We delay for another 5 seconds and get the sensor going.
  delay(5000);
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(trig, LOW);
  //store distance in variable
  int distance = pulseIn(echo, HIGH,26000);
  //calculation into mm
  distance = distance /58 *10;
  delay(100);
  //converting the int to a string using: String(variable)
  String msg = String(distance);
  //Convert string into HEX
  Serial.print("Sending: " + msg + " - ");
  for (unsigned int i = 0; i < msg.length(); i++) {
    Serial.print(msg[i] >> 4, HEX);
    Serial.print(msg[i] & 0xF, HEX);
    Serial.print(" ");
  }
  //Send data to TTN
  int err;
  modem.beginPacket();
  modem.print(msg);
  err = modem.endPacket(true);
  if (err > 0) {
    Serial.println("Message sent correctly!");
  } else {
    Serial.println("Error sending message :(");
  }
  //put board to sleep.
  rtc.begin();
  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);

  //set time in minutes for arduino to sleep. Mine will sleep for 5 minute intervals, then stay awake for about 11 seconds before going back to sleep again.
  rtc.setAlarmMinutes(5);
  rtc.enableAlarm(rtc.MATCH_MMSS);
  //put samd21 to sleep
  rtc.standbyMode();
}