/*
     UDPSendReceive.pde:
  This sketch receives UDP message strings, sends messages back and replies back

*/


#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet2.h>
#include <EthernetUdp2.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 100);

unsigned int localPort = 8080;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  MessageBuffer[] = "Hello world";       // a string to send back
char  ReplyBuffer[] = "acknowledged";       // a string to send back
IPAddress remote;
// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  Serial.begin(9600);
}

void loop() {
    // Check/wait for data packet
    //receiveDataPacket();
    //Send a message(Hello world) toward a chosen ip
    //sendDataPacket((192,288,2,111),8888,MessageBuffer);
    //Reply to the the sender with "acknowledged"
    //replyDataPacket(ReplyBuffer);
  }
  

void receiveDataPacket() {
  // Will wait until a packet has been received
  while(true){
     // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
   if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    remote = Udp.remoteIP();
    for (int i = 0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    // Turns char datapacket into string
    String message(packetBuffer);
    delay(10);
  }

}}
void sendDataPacket(IPAddress remoteIP,unsigned int remotePort,char data[]) {
   // send a message to the IP address and port specified 
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(data);
    Udp.endPacket();

}
void replyDataPacket(char data[]) {
 // send a message, to the IP address and port that sent us the packet we received
 
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(data);
    Udp.endPacket();

}

