#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ctime>
//Network 
 const char* ssid     = "Your Ssid";
 const char* password = "Your pass";
//port
ESP8266WebServer server(80);
int rand_num(int n){
  return (int)((float)rand()/(float) RAND_MAX*n);
}

class LoadBalancing{
	private:
		const char* servers[4] = {"http://localhost:3000/","http://localhost:3001","http://localhost:3002","http://localhost:3003"};
		int weight[4]={4,3,2,1};
		int CF[4];
		int total=0;
	public:
	       LoadBalancing(){
			totalcal();
			SetCumulativeFreq();
		}

		void totalcal(){
			for(int i=0;i<4;++i){
				total+=weight[i];
			}	
		}
		void SetCumulativeFreq(){
			 CF[0]=weight[0];
			for(int i=1;i<4;++i){
				CF[i]=weight[i]+CF[i-1];
			}	
		}
		const char* WeightedRoundRobinAlgo(){
				int rand=rand_num(total);
				int serverindex=0;
				for(int i=0;i<4;++i){
					if(rand<CF[i]){
						serverindex=i;
						break;
					}
				}
				return servers[serverindex];
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
	server.sendHeader("Location",lb.WeightedRoundRobinAlgo(),true);
  	server.send(302, "text/plain", "Redirecting...");
}
void loop(){
	server.handleClient();
}

