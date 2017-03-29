
#include <DHT.h>
#include <ESP8266WiFi.h>

#define DHTTYPE DHT11
#define DHTPIN  13

const char* ssid     = "shmoo3";
const char* password = "s1lkw0rm";

// replace with your channel's thingspeak API key
String apiKey = "KIH7BGQ47EKFDK1T";
const char* server = "api.thingspeak.com";

WiFiClient client;



DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266
float humidity, temp_f;  // Values read from sensor
int ledPin = 4;

void setup()
{ // initialize digital pin LED_BUILTIN as an output.
  dht.begin();           //initialize the Serial communication
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  delay(10);



  WiFi.begin(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  delay(10);

  Serial.println("Temperature and Humidity test!");//print on Serial monitor
  Serial.println("T(C) \tH(%)");                   //print on Serial monitor

}

void loop() {
  digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);                       // wait for a second
  digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
  delay(200);                       // wait for a second

  float h = dht.readHumidity();    // reading Humidity
  float tempC = dht.readTemperature(); // read Temperature as Celsius (the default)
  float temp = (32 + (9 * tempC) / 5); //approximately 2 degrees error correction for conversion and sensor inaccuracy
  Serial.println(temp, h);
  delay(1000);

  if (client.connect(server, 80)) { // "184.106.153.149" or api.thingspeak.com
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(temp);
    postStr += "&field2=";
    postStr += String(h);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" F");
    Serial.println("% send to Thingspeak");
  }
  client.stop();

  Serial.println("Waiting...");
  // thingspeak needs minimum 15 sec delay between updates
  //  currently set to 60 seconds
  delay(60000);
}

