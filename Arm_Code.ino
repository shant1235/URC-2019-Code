/*
     UDPSendReceive.pde:
  This sketch receives UDP message strings, prints them to the serial port
  and sends an "acknowledge" string back to the sender

  A Processing sketch is included near the end of file that can be used to send
  and received messages for testing with a output pin.

*/

#include <Servo.h>
#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet2.h>
#include <EthernetUdp2.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 111);

unsigned int localPort = 8080;      // local port to listen on
static unsigned long lastrec;
// DEBUG MODE
const int debug = 0;
//END OF DEBUG MODE

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back
String pwmListStr [6];
int pwmListInt [6];
const int frontLeftMotor = 2;
const int middleLeftMotor = 3;
const int backLeftMotor = 4;
const int frontRightMotor = 5;
const int middleRightMotor = 6;
const int backRightMotor = 7;
Servo LeftMotors[3];
Servo RightMotors[3];
// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {

  pinMode(frontLeftMotor, OUTPUT);
  pinMode(frontRightMotor, OUTPUT);
  pinMode(middleLeftMotor, OUTPUT);
  pinMode(middleRightMotor, OUTPUT);
  pinMode(backLeftMotor, OUTPUT);
  pinMode(backRightMotor, OUTPUT);
  LeftMotors[0].attach(frontLeftMotor);
  LeftMotors[1].attach(middleLeftMotor);
  LeftMotors[2].attach(backLeftMotor);
  RightMotors[0].attach(frontRightMotor);
  RightMotors[1].attach(middleRightMotor);
  RightMotors[2].attach(backRightMotor);
  // start the Ethernet and UDP:
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  /*
     Include in setup when using example underneath
    pinMode(5,OUTPUT);
    pinMode(4,OUTPUT);
  */

  Serial.begin(9600);
  //while (!Serial);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  checkPack(packetSize);
  delay(50);


}
void checkPack(int packetSize) {

  if (packetSize)
  {
    lastrec = millis();
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
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
    //Isolates command from number value, ex:"DA111222333444555666|23" => "111"
    String messageTemp = message;
    if ((message.length() == 23) && (message.charAt(21) == '2') && (message.charAt(22) == '3')) {
      for (int i = 0; i <= 5; i++) {
        pwmListStr [i] = message.substring(i * 3 + 2, 5 + i * 3);
      }
      for (int j = 0; j <= 5; j++) {
        pwmListInt[j] = pwmListStr[j].toInt();
        Serial.println(pwmListInt[j]);

      }
      LeftMotors[0].writeMicroseconds(pwmToMicro(pwmListInt[0]));
      LeftMotors[1].writeMicroseconds(pwmToMicro(pwmListInt[1]));
      LeftMotors[2].writeMicroseconds(pwmToMicro(pwmListInt[2]));
      RightMotors[0].writeMicroseconds(pwmToMicro(pwmListInt[3]));//111 foward
      RightMotors[1].writeMicroseconds(pwmToMicro(pwmListInt[4]));//Full forward
      RightMotors[2].writeMicroseconds(pwmToMicro(pwmListInt[5]));//stop

    }

    // send a reply, to the IP address and port that sent us the packet we received

  }
  if ((millis() - lastrec) > 5000) {
    delay(1000);
    SoftKill();
  }
}
int pwmToMicro(int i) {
  float k = (i / 255.0) * 1000 + 1000;
  return (int)k;
}
void SoftKill() {
  LeftMotors[0].writeMicroseconds(pwmToMicro(128));
  LeftMotors[1].writeMicroseconds(pwmToMicro(128));
  LeftMotors[2].writeMicroseconds(pwmToMicro(128));
  RightMotors[0].writeMicroseconds(pwmToMicro(128));//111 foward
  RightMotors[1].writeMicroseconds(pwmToMicro(128));//Full forward
  RightMotors[2].writeMicroseconds(pwmToMicro(128));//stop
}
