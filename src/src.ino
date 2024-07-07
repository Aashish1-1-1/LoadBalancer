#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <map>
#include <cstdlib>
#include <ctime>

//Network 
const char* ssid     = "Your Wifi Ssid";
const char* password = "Your Wifi pass";
//port
ESP8266WebServer server(80);
int rand_num(int n){
  return (int)((float)rand()/(float) RAND_MAX*n);
}
class LoadBalancing{
	private:
		//Servers
		const char* servers[4]; 
		int count=0;
		std::map<const char*,int> serversmap;
		int CF[4];
		int total=0;

	public:
		LoadBalancing(int algo){
     			 switch(algo){
     			   case 1:
     			     void RoundRobin_setter();
     			     break;
     			   case 2:
     			     void RoundRobin_setter();
     			     void WeightedRoundRobin_setter();
     			     void totalcal();
     			     void setCumulativeFreq();
     			     break;
     			   case 3:
     			     void RoundRobin_setter();
     			     void LeastConnection_setter();
     			     break;
     			 }
		}
		void RoundRobin_setter(){
		  servers[0] = "https://www.facebook.com/";
      		  servers[1] = "https://www.instagram.com";
      		  servers[2] = "https://www.twitter.com";
      		  servers[3] = "https://www.youtube.com";
    		}
		void WeightedRoundRobin_setter(){
			serversmap["https://www.facebook.com/"]=4;
			serversmap["https://instagram.com/"]=3;
			serversmap["https://www.twitter.com/"]=2;
			serversmap["https://www.youtube.com/"]=1;
		}
		void totalcal(){
			for(int i=0;i<4;++i){
				total+=serversmap[servers[i]];
			}	
		}
		void SetCumulativeFreq(){
			 CF[0]=serversmap[servers[0]];
			for(int i=1;i<4;++i){
				CF[i]=serversmap[servers[i]]+CF[i-1];
			}	
		}
		void LeastConnection_setter(){
			serversmap["https://www.facebook.com/"]=0;
			serversmap["https://instagram.com/"]=0;
			serversmap["https://www.twitter.com/"]=0;
			serversmap["https://www.youtube.com/"]=0;
		}
	  	const char* RoundRobinAlgo(){
  			int temp=count;
			count=(count+1)%4;
			return servers[temp];		
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
		const char* LeastConnectionAlgo(){
			int leastconnindex=0;
			for(int i=1;i<4;++i){
				if(serversmap[servers[i]]<serversmap[servers[leastconnindex]]){
					leastconnindex=i;
				}
			}
			serversmap[servers[leastconnindex]]=serversmap[servers[leastconnindex]]+1;
			return servers[leastconnindex];
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
void handleRedirection(){
	LoadBalancing lb(1);
	server.sendHeader("Location",lb.RoundRobinAlgo(),true);
  	server.send(302, "text/plain", "Redirecting...");
}
void loop(){
	server.handleClient();
}

