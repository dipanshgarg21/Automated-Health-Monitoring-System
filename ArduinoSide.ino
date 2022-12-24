// First we include the libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Firebase_Arduino_WiFiNINA.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

MAX30100 sensor;

#define ONE_WIRE_BUS 2
#define REPORTING_PERIOD_MS     1000

OneWire oneWire(ONE_WIRE_BUS);
PulseOximeter pox;

uint32_t tsLastReport = 0;

DallasTemperature sensors(&oneWire);

#define FIREBASE_HOST "health-monitoring-system-f0abd-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "IVhLXXD3b6gbkFDWNKO77YeXozDOFxJQDUmASbXJ"
#define WIFI_SSID "SlowWiFi"
#define WIFI_PASSWORD "connectnow21"

FirebaseData firebaseData1, firebaseData2, firebaseData3;

String path1 = "/Temperature";
String jsonStr1;

String path2 = "/Pulse";
String jsonStr2;

String path3 = "/SpO2";
String jsonStr3;

void onBeatDetected()
{
  Serial.println("Beat!");
}

void setup()
{
  Serial.begin(9600);
  delay(100);
  Serial.println();

  //  sensors.requestTemperatures(); // Send the command to get temperature readings
  //  /********************************************************************/
  //  Serial.print("Temperature is: ");
  //  Serial.print(((sensors.getTempCByIndex(0) * 1.8) + 32)); // Why "byIndex"?
  //  Serial.println(" °F");

  Serial.print("Connecting to WiFi…");
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED) {
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print(".");
    delay(100);
  }
  Serial.print(" IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);

  int i = 0;
  while (i < 30)
  {
    temperature();
    i++;
    delay(1000);
  }

  int j = 0;
  while (j < 20)
  {
    Pulse();
    j++;
    delay(1000);
  }

  int k = 0;
  while (k < 20)
  {
    SpO2();
    k++;
    delay(1000);
  }

}

void temperature()
{
  sensors.requestTemperatures();
  float t;
  Serial.print("Temperature is: ");
  t = ((sensors.getTempCByIndex(0) * 1.8) + 32);
  Serial.print(t);
  Serial.println(" °F");
  if (Firebase.setFloat(firebaseData1, path1 + "/FinalTemp/T", t))
  {
    Serial.println(firebaseData1.dataPath() + " = " + t);
  }

  jsonStr1 = "{\"T\":" + String(t, 2) + "}";

  if (Firebase.pushJSON(firebaseData1, path1 + "/RealTime", jsonStr1)) {
    Serial.println(firebaseData1.dataPath() + " = " + firebaseData1.pushName());
  }
  else {
    Serial.println("Error: " + firebaseData1.errorReason());
  }

  Serial.println();
}

void Pulse()
{
  pox.update();
  int p;
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate:");
    Serial.print(pox.getHeartRate());
    Serial.println("bpm")
    tsLastReport = millis();
  }
  Serial.println("Pulse: " + p);
  if (Firebase.setInt(firebaseData2, path2 + "/FinalPulse/P", p))
  {
    Serial.println(firebaseData2.dataPath() + " = " + p);
  }
  jsonStr2 = "{\"P\":" + String(p, 2) + "}";

  if (Firebase.pushJSON(firebaseData2, path2 + "/RealTime2", jsonStr2)) {
    Serial.println(firebaseData2.dataPath() + " = " + firebaseData2.pushName());
  }
  else {
    Serial.println("Error: " + firebaseData2.errorReason());
  }

  Serial.println();
}

void SpO2()
{
  pox.update();
  int s;
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    Serial.print("SpO2: ");
    s = pox.getSpO2();
    Serial.print(t);
    Serial.println("%");
    tsLastReport = millis();
  }
  Serial.println("SpO2: " + s);
  if (Firebase.setInt(firebaseData3, path3 + "/FinalPulse/S", s))
  {
    Serial.println(firebaseData3.dataPath() + " = " + s);
  }
  jsonStr3 = "{\"S\":" + String(s, 2) + "}";

  if (Firebase.pushJSON(firebaseData3, path3 + "/RealTime3", jsonStr3)) {
    Serial.println(firebaseData3.dataPath() + " = " + firebaseData3.pushName());
  }
  else {
    Serial.println("Error: " + firebaseData3.errorReason());
  }

  Serial.println();
}

void loop()
{}
