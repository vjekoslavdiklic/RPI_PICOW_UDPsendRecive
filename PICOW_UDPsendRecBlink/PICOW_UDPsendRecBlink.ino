/*
 * This is a small example modified from examples for PicoW. 
 * Program revieves and confirms recived UDP by send massage back. 
 * UDP Massage contains analog value form A2 pin and Onboard temp sensor value.
 * Great start for many projects that includes receiving and sending to pc from PicoW.
 * Comes with example python code for PC side
 * Both PC and PICOW have to be connected on same Wifi 
 * 
*/
#include <stdio.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#ifndef STASSID
#define STASSID "WifiName" //Wifi Network Name
#define STAPSK "WifiPass" //Wifi Network Password
#endif

#define LEDOUT 9 //Use For Output Pin
#define WIFISTATUSLED 13 //On If Connected On Wifi Network
#define AnalogPin A2 //Pin ID For Analog
bool LEDOUTSTATUS=0; //OUTPUT Status Variable
unsigned int localPort = 5005;  // Local Port To Listen On
int AnalogValue=0; //Variable For Storing Analog Readout
float Tempvalue=0; //Varibale For Storing Temp Readout
   
// Buffers For Receiving And Sending Data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];  // buffer to hold incoming packet,
char ReplyBuffer[] = "E_T=TempC_A=Aval\r\n";        // a string to send back

WiFiUDP Udp; //Activete Udp

//Simple LED Toggler Function
void LedToggler(int TargetPin,int togtime)
{
  digitalWrite(TargetPin, !digitalRead(TargetPin));
  delay(togtime); 
}

//This Function Forms UDP Data For Sending
void UpdateReplayBuffer(char *rBuffer,float temp,int anaval,bool ledstat)
{
  rBuffer[0]=48+ledstat; //write ledstatus on first char
  sprintf( &ReplyBuffer[12], "%d", anaval); //write analog value
  sprintf( &ReplyBuffer[4], "%2.1fC", temp); //write temp value
  rBuffer[9]='_';//fix /remove \n written by sprintf at position 9
  rBuffer[10]='A';
  rBuffer[11]='=';
  }

void setup() 
{
  Serial.begin(115200); //start serial communication
  //Try To Connect to Wifi
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
  pinMode(AnalogPin,INPUT);
}

void loop() 
{
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) 
  {
    Serial.printf("Received packet of size %d from %s:%d\n    (to %s:%d)\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort(), Udp.destinationIP().toString().c_str(), Udp.localPort());

    // Read The Packet Into PacketBufffer
    int n = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    packetBuffer[n] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    //Process Data Recived   
    if (packetBuffer[0]=='A'){LEDOUTSTATUS=1;} //if i got A from pc LEDOUTSTATUS=1
    else{LEDOUTSTATUS=0;} //otherwise LEDOUTSTATUS=0
    
    //Update UDP Buffer
    UpdateReplayBuffer(&ReplyBuffer[0], Tempvalue, AnalogValue,LEDOUTSTATUS);

    // Send a Reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(),localPort);
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
  
  //Update Wifi Led Status 
  if(WiFi.status() == WL_CONNECTED){LedToggler(WIFISTATUSLED,50);}
  else{digitalWrite(WIFISTATUSLED, LOW);}

  //Blink the status LED
  LedToggler(LED_BUILTIN,100);

  //Read All Analog Inputs:
  AnalogValue=analogRead(AnalogPin);// read analog input value
  Tempvalue=analogReadTemp(); // read onborad temp analog value
    
  // Update LEDOUT Output Status
  digitalWrite(LEDOUT, LEDOUTSTATUS);// SET output pin
}
