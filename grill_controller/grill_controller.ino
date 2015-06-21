#include <SPI.h>
#include <WiFi.h>

//WIFI SETUP
char ssid[] = "University";     //  your network SSID (name) 
char pass[] = "thugmansion";    // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
//char server[] = "www.google.com";
char server[] = "http://wifi-grill.herokuapp.com";    // name address for Google (using DNS)

WiFiClient client;

//PROBE SETUP
const int PROBE1PIN = A0;
const int PROBE2PIN = A1;
const int PROBE1LED = 8;
const int PROBE2LED = 9;

int Probe1AlarmTemp = 80;
int Probe2AlarmTemp = 95;

int loopint = 1;

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

  int loopint = 1;
}

void loop() {
  // put your main code here, to run repeatedly:

  delay(1000);

  float probe1reading = analogRead(PROBE1PIN); //read probe 1
  float probe2reading = analogRead(PROBE2PIN); //read probe 2

  float probe1temp = convertVoltToTemp(probe1reading); //convert to degrees
  float probe2temp = convertVoltToTemp(probe2reading);

  // check for probe 1 alarm temperature
  if (probe1temp > Probe1AlarmTemp)
  {
    digitalWrite(PROBE1LED,HIGH);
  }
  else
  {
    digitalWrite(PROBE1LED,LOW);
  }

  // check for alarm 2 temperatureÂ
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
  
  loopint = loopint +1;
  Serial.println(loopint);
  
  if (loopint == 30) {
    postTempReading(probe1temp,1);
    postTempReading(probe2temp,2);
  
    loopint = 0;
  }

}

double convertVoltToTemp(float volt){
  float tempK;
  float tempC;
  float tempF;

  // get the Kelvin temperature
  tempK = log(((10240000/volt) - 10000));
  tempK = 1 / (0.001129148 + (0.000234125 * tempK) + (0.0000000876741 *
    tempK * tempK * tempK));
    
  // convert to Celsius and round to 1 decimal place
  tempC = tempK - 273.15;
  //tempC = round(tempC*100)/100;

  // get the Fahrenheit temperature, rounded
  tempF = (tempC * 1.8) + 32;
  //tempF = round(tempF*100)/100;

  // return all three temperature scales
  return tempF;
}

void postTempReading(double temp,int probe_num) {

  data=String("temp%5Bdevice_id%5D=Device1");
  data.concat("&temp%5Bprobe_num%5D=");
  data.concat(probe_num);
  data.concat("&temp%5Btemperature%5D=");
  char tempstr[10];
  dtostrf(temp,2,1,tempstr);
  data.concat(tempstr);
  
  Serial.println(data);
  
  //String server2 ="/submit?";
  //server2.concat("device_id=Device1");
  //server2.concat("&probe_num=");
  //server2.concat(probe_num);
  //server2.concat("&temperature=");
  //char tempstr[10];
  //dtostrf(temp,2,1,tempstr);
  //server2.concat(tempstr);
  
  //Serial.println(server2);
  
  //int serverlength = 0;
  //serverlength = server2.length();
  
  //char server3[server2.length()+1];
  //server2.toCharArray(server3,server2.length()+1);
  //Serial.println(server3);
  
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    
    //client.println("GET / HTTP/1.1");
    //client.println("GET / HTTP/1.1");
    //client.println("Host: wifi-grill.herokuapp.com");
    //client.println();
    client.println("POST /submit HTTP/1.1");
    //client.print("Host: ");
    //client.println(server3);
    client.println("Host: wifi-grill.herokuapp.com");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.print(data);
    
    //client.println("GET /search?q=arduino HTTP/1.1");
    //client.println("Host: www.google.com");
    //client.println("Connection: close");
    //client.println();
  }
  else {
    Serial.println("No connection made"); 
  }
  
  int connectLoop = 0;
  int inChar;

  while(client.connected())
  {
    while(client.available())
    {
      inChar = client.read();
      Serial.write(inChar);
      connectLoop = 0;
    }

    delay(1);
    connectLoop++;
    if(connectLoop > 10000)
    {
      Serial.println();
      Serial.println(F("Timeout"));
      client.stop();
    }
  }

  Serial.println();
  Serial.println(F("disconnecting."));

  if(client.connected()) {
    client.stop();
  }

  //delay(10000); // wait one minute before posting again

}


