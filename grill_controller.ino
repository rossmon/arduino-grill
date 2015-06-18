#include <SPI.h>
#include <WiFi.h>

const int PROBE1PIN = A0;
const int PROBE2PIN = A1;
const int PROBE1LED = 8;
const int PROBE2LED = 9;

int Probe1AlarmTemp = 95;
int Probe2AlarmTemp = 95;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PROBE1PIN, INPUT);
  pinMode(PROBE2PIN, INPUT);
  pinMode(PROBE1LED, OUTPUT);
  pinMode(PROBE2LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(10000);

  double probe1reading = (analogRead(PROBE1PIN)*(5.0/1023.0));
  double probe2reading = (analogRead(PROBE2PIN)*(5.0/1023.0));

  double probe1temp = convertVoltToTemp(probe1reading);
  double probe2temp = convertVoltToTemp(probe2reading);

  if (probe1temp > Probe1AlarmTemp)
  {
    digitalWrite(PROBE1LED,HIGH);
  }
  else
  {
    digitalWrite(PROBE1LED,LOW);
  }
  if (probe2temp > Probe2AlarmTemp)
  {
    digitalWrite(PROBE2LED,HIGH);
  }
  else
  {
    digitalWrite(PROBE2LED,LOW);
  }

  Serial.print("Probe 1 Temp: ");
  Serial.println(probe1temp);
  Serial.print("Probe 2 Temp: ");
  Serial.println(probe1temp);

}

double convertVoltToTemp(double volt){
  double tempK;
  double tempC;
  double tempF;

  // get the Kelvin temperature
  tempK = log(((10240000/volt) - 10000));
  tempK = 1 / (0.001129148 + (0.000234125 * tempK) + (0.0000000876741 *
      tempK * tempK * tempK));

  // convert to Celsius and round to 1 decimal place
  tempC = tempK - 273.15;
  tempC = round(tempC*10)/10;

  // get the Fahrenheit temperature, rounded
  tempF = (tempC * 1.8) + 32;
  tempF = round(tempF*10)/10;

  // return all three temperature scales
  return tempF;
 }
