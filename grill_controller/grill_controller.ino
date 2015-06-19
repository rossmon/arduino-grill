#include <SPI.h>
#include <WiFi.h>

//WIFI SETUP
char ssid[] = "University";     //  your network SSID (name) 
char pass[] = "thugmansion";    // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
char server[] = "https://wifi-grill.herokuapp.com";    // name address for Google (using DNS)

WiFiClient client;

//PROBE SETUP
const int PROBE1PIN = A0;
const int PROBE2PIN = A1;
const int PROBE1LED = 8;
const int PROBE2LED = 9;

int Probe1AlarmTemp = 80;
int Probe2AlarmTemp = 95;

String data;

void setup() {
  // setup input and output pins
  Serial.begin(9600);
  pinMode(PROBE1PIN, INPUT);
  pinMode(PROBE2PIN, INPUT);
  pinMode(PROBE1LED, OUTPUT);
  pinMode(PROBE2LED, OUTPUT);

  //SETUP WIFI connection
  // attempt to connect using WPA2 encryption:
  Serial.println("Attempting to connect to WPA network...");
  status = WiFi.begin(ssid, pass);

  // if you're not connected, stop here:
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
    while(true);
  } 
  // if you are connected, print out info about the connection:
  else {
    Serial.println("Connected to network");
  }

//  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
//  if (client.connect(server, 80)) {
//    Serial.println("connected to server");
//  }
 // else {
   // Serial.println("No connection made"); 
  //}

  data = "";

}

void loop() {
  // put your main code here, to run repeatedly:

  delay(10000);

  double probe1reading = analogRead(PROBE1PIN); //read probe 1
  double probe2reading = analogRead(PROBE2PIN); //read probe 2

  double probe1temp = convertVoltToTemp(probe1reading); //convert to degrees
  double probe2temp = convertVoltToTemp(probe2reading);

  // check for probe 1 alarm temperature
  if (probe1temp > Probe1AlarmTemp)
  {
    digitalWrite(PROBE1LED,HIGH);
  }
  else
  {
    digitalWrite(PROBE1LED,LOW);
  }

  // check for alarm 2 temperature
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
  Serial.println(probe2temp);

  postTempReading(probe1temp,1);
  postTempReading(probe2temp,2);

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

void postTempReading(double temp,int probe_num) {

  data=String("temp[device_id]=Device1");
  data.concat("&temp[probe_num]=");
  data.concat(probe_num);
  data.concat("&temp[temperature]=");
  char tempstr[10];
  dtostrf(temp,2,1,tempstr);
  data.concat(tempstr);
  
  Serial.println(data);
  
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    
    //client.println("GET / HTTP/1.1");
    //client.println();
    client.println("POST / HTTP/1.1");
    client.println("Host: https://wifi-grill.herokuapp.com");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println(data.length());
    client.println();
    client.print(data);
  }
  else {
    Serial.println("No connection made"); 
  }

  if(client.connected()) {
    client.stop();
  }

  delay(60000); // wait one minute before posting again

}


