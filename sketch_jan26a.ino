#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SPI.h>
#include <Ethernet.h>
 
#define DHTPIN 8     
#define DHTTYPE DHT22 
 
DHT dht(DHTPIN, DHTTYPE);



byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(10, 0, 0, 211);
EthernetServer server(80);
 
void setup() 
{
  Ethernet.begin(mac, ip);
  
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  
  dht.begin();
}
 
void loop() 
{
  delay(1000);
  float h = dht.readHumidity();     
  float t = dht.readTemperature(); 
 
  if (isnan(t) || isnan(h)) 
  {
    Serial.println("DHT22 not connected");
  } 
  else 
  {
   
  EthernetClient client = server.available();
  if (client) {
  
     boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  
          client.println("Refresh: 5");  
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.print("Luftfeuchte: "); 
          client.print(h);
          client.print(" %\t");
          client.print("Temperatur: "); 
          client.print(t);
          client.println(" C");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
  
    client.stop();
  
  }


 }



  
}
