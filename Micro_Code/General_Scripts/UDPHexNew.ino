/*
     UDPSendReceive.pde:
 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender

 A Processing sketch is included near the end of file that can be used to send
 and received messages for testing with a output pin.

 */
/*
motor driver library
-functions should attatch motors, write pins, decode the messages

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
IPAddress ip(192, 168, 1, 100);

unsigned int localPort = 8080;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back
String pwmList [6];
const int frontLeftMotor = 2;
const int frontRightMotor = 3;
const int middleLeftMotor = 4;
const int middleRightMotor = 5;
const int backLeftMotor = 6;
const int backRightMotor = 7;
Servo LeftMotors[3];
Servo RightMotors[3];
Servo myservo;

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {

  pinMode(frontLeftMotor,OUTPUT);
  pinMode(frontRightMotor,OUTPUT);
  pinMode(middleLeftMotor,OUTPUT);
  pinMode(middleRightMotor,OUTPUT);
  pinMode(backLeftMotor,OUTPUT);
  pinMode(backRightMotor,OUTPUT);
  LeftMotors[0].attach(frontLeftMotor);
  LeftMotors[1].attatch(middleLeftMotor);
  LeftMotors[2].attatch(backLeftMotor);
  RightMotors[0].attatch(frontRightMotor);
  RightMotors[1].attatch(middleRightMotor);
  RightMotors[2].attatch(backRightMotor);
  /*
  LeftMotors[i].attatch(pin list);
  RightMotors[i].attatch(pinlist)
  */
  // start the Ethernet and UDP:
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
  /*
   * Include in setup when using example underneath
  pinMode(5,OUTPUT);
  pinMode(4,OUTPUT);
  */

  Serial.begin(9600);
   //while (!Serial);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();


  if (packetSize)
  {
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
     //Isolates command from number value, ex:"AABBCCDDEEFF" => "FF"
    String messageTemp = message;
    if((message.length() == 19) && (message.charAt(0) =='s')){
    for(int i = 0; i <= 5;i++){
      pwmList [i] =message.substring(i*3+1,4+i*3);
      // values are from 0 - 255
    }

    LeftMotors[0].writeMicroseconds(pwmToMicro(pwmList[0]));
    LeftMotors[1].writeMicroseconds(pwmToMicro(pwmList[1]));
    LeftMotors[2].writeMicroseconds(pwmToMicro(pwmList[2]));
    RightMotors[0].writeMicroseconds(pwmToMicro(pwmList[3]));
    RightMotors[1].writeMicroseconds(pwmToMicro(pwmList[4]));
    RightMotors[2].writeMicroseconds(pwmToMicro(pwmList[5]));


    /*for (int j = 0;j <= 5; j++){
      Serial.println(pwmList[j]);
      digitalWrite(j+2,pwmList[j].toInt());
    }*/
         //myservo.attatch(2);
        // myservo.writeMicroseconds(1400);
   //myservo[0].attach(2);
 // myservo[0].writeMicroseconds(1400);
         /** range should be between 1000 - 2000 we will map 0 - 255 to values between 1000 - 2000
       Example above, we will need to scale the balues and then multiply
         **/
    }

     // send a reply, to the IP address and port that sent us the packet we received

  }
  delay(10);
}

int pwmToMicro(int i){
float k = (i/255.0)*1000+1000;
  return (int)k;
}
