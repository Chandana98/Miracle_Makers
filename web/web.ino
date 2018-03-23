#include <ESP8266WiFi.h>
#include <DHT.h>

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22 

const char* ssid     = "Your-ssid"; // Your ssid
const char* password = "Your-Password"; // Your Password
WiFiServer server(80);

DHT dht(DHTPIN, DHTTYPE);

void setup() {
Serial.begin(115200);
delay(10);
Serial.println();
WiFi.mode(WIFI_STA);
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
server.begin();
Serial.println("Server started");
Serial.println(WiFi.localIP());
}

void loop() {
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    return;
  }  else {
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  }
WiFiClient client = server.available();
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close"); 
client.println("Refresh: 60"); 
client.println();
client.println("<!DOCTYPE html>");
client.println("<html xmlns='http://www.w3.org/1999/xhtml'>");
client.println("<head>\n<meta charset='UTF-8'>");
client.println("<title>Temperature & Humidity</title>");
client.println("</head>\n<body bgcolor='#000318'>");
client.println("<div style='margin: auto; padding: 20px; text-align: center; font-family: Impact, Charcoal, sans-serif; color: #fff'>");
client.println("<H1>ESP8266 & DHT11/22 Sensor</H1>");
client.println("<div style='font-size: 8em;'>");
client.print("Temperature<br /><b>");
client.print((float)t, 2);
client.println("°C</b><br />");
client.print("Humidity<br /><b>");
client.print((float)h, 2);
client.println("%</b><br />");
client.println("</div>");
client.println("");
client.println("</div>");
client.print("</body>\n</html>");
}
