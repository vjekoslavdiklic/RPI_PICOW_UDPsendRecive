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
#define STASSID "mywifi"
#define STAPSK ",ypass"
#endif

unsigned int localPort = 5005;  // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE + 1];  // buffer to hold incoming packet,
char ReplyBuffer[] = "acknowledged\r\n";        // a string to send back

WiFiUDP Udp;

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

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(),localPort);
    Udp.write(ReplyBuffer);
    Udp.endPacket();
  }
   //blink the LED
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
    
}


*/
