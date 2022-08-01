/*
 * This is a small example modified from examples for PicoW. 
 * Program revieves and confirms recived udp by send massage back. 
 * Great start for many projects that includes receiving and sending to pc from PicoW.
 * Comes with example python code for PC side
 * Both PC and PICOW have to be connected on same Wifi 
 * 
*/
#include <WiFi.h>
#include <WiFiUdp.h>
#ifndef STASSID
#define STASSID "mywifiname"
#define STAPSK "mywifipass"
#endif

#define LEDOUT 9 //use for remote controll
#define WIFISTATUSLED 13 //on if connected

bool LEDOUTSTATUS=0;
unsigned int localPort = 5005;  // local port to listen on
   
// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];  // buffer to hold incoming packet,
char ReplyBuffer[] = "AAAAAA\r\n";        // a string to send back

WiFiUDP Udp;

void LedToggler(int TargetPin,int togtime){
    digitalWrite(TargetPin, !digitalRead(TargetPin));
    delay(togtime); 
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(STASSID, STAPSK);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
  }
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDOUT, OUTPUT);
  pinMode(WIFISTATUSLED, OUTPUT);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d)\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort(), Udp.destinationIP().toString().c_str(), Udp.localPort());

    // read the packet into packetBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);



    //Process data recived
    //if i got A for pc i'll toggle led

    if (packetBuffer[0]=='A'){LEDOUTSTATUS=1;}
    else{LEDOUTSTATUS=0;}

    //define a replay:
    ReplyBuffer[0]=48+LEDOUTSTATUS;
    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(),localPort);
    Udp.write(ReplyBuffer);
    Udp.endPacket();

  }

  
//update wifi led status 
  if(WiFi.status() == WL_CONNECTED){LedToggler(WIFISTATUSLED,50);}
  else{digitalWrite(WIFISTATUSLED, LOW);}

//blink the status LED
  LedToggler(LED_BUILTIN,100);
    
// update LEDOUT status
    digitalWrite(LEDOUT, LEDOUTSTATUS);
    

}
