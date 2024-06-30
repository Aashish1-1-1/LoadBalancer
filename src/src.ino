#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//Network 
const char* ssid     = "135966@ClassicTech";
const char* password = "12345678";
//Servers
const char* servers[] = {"https://www.facebook.com/","https://www.instagram.com","https://www.twitter.com","https://www.youtube.com"};
 int count=0;
//port
ESP8266WebServer server(80);
void setup(){
	Serial.begin(115200);
	Serial.print("Connecting to ");
  	Serial.println(ssid);
  	WiFi.begin(ssid, password);
  	while (WiFi.status() != WL_CONNECTED) {
  	  delay(500);
  	  Serial.print(".");
  	}
  	Serial.println("");
  	Serial.println("WiFi connected.");
  	Serial.println("IP address: ");
  	Serial.println(WiFi.localIP());
		server.on("/",handleRedirection);
  	server.begin();
}
const char* RoundRobinAlgo(){
  int temp=count;
	count=(count+1)%4;
	return servers[temp];		
}
void handleRedirection(){
	server.sendHeader("Location",RoundRobinAlgo(),true);  // true for 302 redirect
  	server.send(302, "text/plain", "Redirecting...");
}

void loop(){
	server.handleClient();
}
