## LoadBalancer
A minimal wannabe loadbalancer using Esp8236.

## Architecture
![archi](https://github.com/Aashish1-1-1/LoadBalancer/blob/main/images/archi.svg)

## Note
This is not how the actual loadbalancer works in actual loadbalancer are responsible for a tunnel between server and client,loadbalancer are responsible to accept request and wait for response,map them and send respective response to user. Here it is only redirecting to server and rest is handeled by server.

## To Run
Clone the repo in src file input wifi ssid and password and upload in ESP8266 and visit the ip printed in serial monitor
