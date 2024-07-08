#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//Network 
const char* ssid     = "Your ssid";
const char* password = "Your pass";
//port
ESP8266WebServer server(80);

class LoadBalancing{
	private:
		//Servers
		const char* servers[4] = {"http://localhost:3000/","http://localhost:3001","http://localhost:3002","http://localhost:3003"};
		int count=0;

	public:
		LoadBalancing(){}
		const char* RoundRobinAlgo(){
  			int temp=count;
			count=(count+1)%4;
			return servers[temp];		
	   	}
};
void setup(){
	srand(time(NULL));
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
LoadBalancing lb;
void handleRedirection(){
	server.sendHeader("Location",lb.RoundRobinAlgo(),true);
  	server.send(302, "text/plain", "Redirecting...");
}
void loop(){
	server.handleClient();
}

