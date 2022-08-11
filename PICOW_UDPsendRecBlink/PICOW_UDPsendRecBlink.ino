/*
 * This is a small example modified from examples for PicoW. 
 * Program revieves and confirms recived UDP by send massage back. 
 * UDP Massage contains analog value form A2 pin and Onboard temp sensor value.
 * Great start for many projects that includes receiving and sending to pc from PicoW.
 * Comes with example python code for PC side
 * Both PC and PICOW have to be connected on same Wifi 
 * 
*/
//Wifistable
#include <stdio.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#ifndef STASSID
#endif
#define SYSID "ID001" //Must have this number of chars (5) IDXXX
#define LEDOUT 9 //Use For Output Pin
#define WIFISTATUSLED 13 //On If Connected On Wifi Network
#define AnalogPin A2 //Pin ID For Analog

bool LEDOUTSTATUS=0; //OUTPUT Status Variable
unsigned int localPort = 5005;  // Local Port To Listen On
int AnalogValue=0; //Variable For Storing Analog Readout
float Tempvalue=0; //Varibale For Storing Temp Readout
int LT=100; //Low triger OFF fun
int HT=900; //High Trigger On fun
int Mode=4;//0 for auto,1 for remote control,3 hibrid, 4 for error   
// Buffers For Receiving And Sending Data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];  // buffer to hold incoming packet,
char ReplyBuffer[] = "E_T=XX.XC_A=XXXX_IDXXX_LT=XXXX_HT=XXXX_M=\r\n";        // a string to send back
int lastpack;
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
  sprintf( &ReplyBuffer[12], "%4.0d", anaval); //write analog value
  sprintf( &ReplyBuffer[4], "%2.1fC", temp); //write temp value
  sprintf( &ReplyBuffer[17], "%s", SYSID); //write SYS ID
  sprintf( &ReplyBuffer[26], "%4.0d", LT); //write LT value
  sprintf( &ReplyBuffer[34], "%4.0d", HT); //write HT value
  rBuffer[9]='_';//fix /remove \n written by sprintf at position 9
  rBuffer[10]='A';
  rBuffer[11]='=';
  rBuffer[16]='_';
  rBuffer[22]='_';
  rBuffer[23]='L';
  rBuffer[24]='T';
  rBuffer[25]='=';
  rBuffer[30]='_';
  rBuffer[31]='H';
  rBuffer[32]='T';
  rBuffer[33]='=';
  rBuffer[38]='_';
  rBuffer[39]='M';
  rBuffer[40]='=';
  rBuffer[41]=48+Mode;
  rBuffer[42]='\n';
  }

void setup() 
{
  Serial.begin(115200); //start serial communication
  WiFi.begin(STASSID, STAPSK); //Init and Try To Connect to Wifi
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
  Serial.printf("UDP server on port %d\n", localPort);
  Udp.begin(localPort);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDOUT, OUTPUT);
  pinMode(WIFISTATUSLED, OUTPUT);
  pinMode(AnalogPin,INPUT);
  Mode=0;
  void UpdateReplayBuffer(char *rBuffer,float temp,int anaval,bool ledstat);
}

void loop() 
{
  if (WiFi.status() == WL_CONNECTED)
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
        Mode=(packetBuffer[2]-48);
        if (Mode==1)
        {
          LEDOUTSTATUS=(packetBuffer[0]-48); 
        }
        //if (packetBuffer[4]==1){
        //  }
        //if (packetBuffer[0]=='A'){LEDOUTSTATUS=1;} //if i got A from pc LEDOUTSTATUS=1
        //if (Mode==1)
        //{//LEDOUTSTATUS=packetBuffer[1];
         //}
        //else{} //otherwise LEDOUTSTATUS=0
    
        //Update UDP Buffer
        UpdateReplayBuffer(&ReplyBuffer[0], Tempvalue, AnalogValue,LEDOUTSTATUS);

        // Send a Reply, to the IP address and port that sent us the packet we received
        Udp.beginPacket(Udp.remoteIP(),localPort);
        Udp.write(ReplyBuffer);
        Udp.endPacket();
      }
    
    }
  
    //Update Wifi Led Status 
    if(WiFi.status() == WL_CONNECTED){LedToggler(WIFISTATUSLED,50);}
    else{digitalWrite(WIFISTATUSLED, LOW);}

    //Blink the status LED
    LedToggler(LED_BUILTIN,100);

    //Read All Analog Inputs:
    AnalogValue=analogRead(AnalogPin);// read analog input value
    Tempvalue=analogReadTemp(); // read onborad temp analog value

    //Do the Logic if in auto Mode
    if (Mode==0)
      {
      if ((AnalogValue<LT)&(LEDOUTSTATUS)){LEDOUTSTATUS=0;}
      if ((AnalogValue>HT)&(!LEDOUTSTATUS)){LEDOUTSTATUS=1;}
      }
    // Update LEDOUT Output Status
    digitalWrite(LEDOUT, LEDOUTSTATUS);// SET output pin
 }
 
