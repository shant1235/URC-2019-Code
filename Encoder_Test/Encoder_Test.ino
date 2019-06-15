#include <Encoder.h>
#include <Servo.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(2, 3);
Servo myservo;
const int pwmPin = 5;
const int switchPin = 7;
//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");
  pinMode(switchPin, INPUT);
  pinMode(pwmPin, OUTPUT);
  pinMode(8, INPUT);
  myservo.attach(pwmPin);

}

long oldPosition  = -999;
long newPosition = myEnc.read();
void loop() {
  readPosition();
  newPosition = myEnc.read();
  if (digitalRead(switchPin) == HIGH) {
    myservo.writeMicroseconds(1400);
  } else {
    myservo.writeMicroseconds(1500);
  }

  if (digitalRead(8) == LOW) {
    while (true) {
      readPosition();
      if (newPosition > 0) {
        myservo.writeMicroseconds(1550);
        Serial.println((newPosition));
      } else if (newPosition < 0) {
        myservo.writeMicroseconds(1450);
        Serial.println((newPosition));
      } else if (newPosition == 0 ) {
        Serial.println((newPosition));
        myservo.writeMicroseconds(1500);
        break;
      }
    }
  }
}
void readPosition() {
  newPosition = myEnc.read();
  if (newPosition != oldPosition) {

    oldPosition = newPosition;
    Serial.println((newPosition));


  }
}
