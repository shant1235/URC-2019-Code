/*
 *Name: Prashant Mistry 
 *Date: 2019-02-07
 *Purpose: This program will control and print Temperture and humidity
 *PH,and sense for hydrogen gas
 *Libraries :https://github.com/practicalarduino/SHT1x
 * */
#include <SHT1x.h>
//Change datapin and clockpin for Temp and Humi Test
#define dataPin 10
#define clockPin 11
//Change gaspin for gas sensor (analog input)
#define gasPin 1
//Change for PH Sensor
#define SensorPin A0 //pH meter Analog output to Arduino Analog Input 0
#define Offset 0 //deviation compensate from orignal value, change appropriately 
#define LED 13
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth 40 //times of collection
int pHArray[ArrayLenth]; //Store the average value of the sensor feedback
int pHArrayIndex = 0;

SHT1x sht1x(dataPin, clockPin);
void setup() {
  // put your setup code here, to run once:
pinMode(LED, OUTPUT);
Serial.begin(9600); //Set serial baud rate to 9600 bps
Serial.begin(38400); //Set serial baud rate to 38400 bps
}

void loop() {
//XXXXXXXXXXXXXXXXXXXXXXXX Gas sensor test XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//Gas_test();

//XXXXXXXXXXXXXXXXXXXXXXXX Temperature and Humidity XXXXXXXXXXXXXXXXXXXXX

//Temp_Humidity_test(char var);

//XXXXXXXXXXXXXXXXXXXXXXXX PH Test XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

//PH_test(int samplingTime,int numLoop);
}

void Temp_Humidity_test(char var){
  float temp_c;
  float temp_f;
  float humidity;
  if(var == 'c'){
    //Returns temperature in celsius
      temp_c = sht1x.readTemperatureC();
  Serial.print("Temperature: ");
  Serial.print(temp_c, DEC);
  Serial.print("C");
  }else if (var == 'f'){
    //Returns temperature in fahrenheit 
      temp_f = sht1x.readTemperatureF();
  Serial.print("Temperature: ");
  Serial.print(temp_f, DEC);
  Serial.print("F");
  }else if (var == 'h'){
    //Returns humidity
      humidity = sht1x.readHumidity();
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  }else {
    //Returns all
  temp_c = sht1x.readTemperatureC();
  temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();
  Serial.print("Temperature: ");
  Serial.print(temp_c, DEC);
  Serial.print("C / ");
  Serial.print("F. Humidity: ");
  Serial.print(humidity);
  Serial.println("%");
  }
  
}
void Gas_test(){
  int x;
  x = analogRead(0);//Read Gas value from analog 0
  Serial.println("Gas value");
  Serial.println(x);//Print the value to serial port
}
void PH_test(int samplingTime,int numLoop){
  
  static float pHValue, voltage;
  for(int i = numLoop; i > 0;i--)
  {
    pHArray[pHArrayIndex++] = analogRead(SensorPin);
    if (pHArrayIndex == ArrayLenth)pHArrayIndex = 0;
    voltage = avergearray(pHArray, ArrayLenth) * 5.0 / 1024;
    pHValue = 3.5 * voltage + Offset;
    samplingTime = millis();
    Serial.print("Voltage:");
    Serial.print(voltage, 2);
    Serial.print(" pH value: ");
    Serial.println(pHValue, 2);
    digitalWrite(LED, digitalRead(LED) ^ 1);
    delay(samplingTime);
  }
  
}
//This method is used by PH_test()only
double avergearray(int* arr, int number) {
  int i;
  int max, min;
  double avg;
  long amount = 0;
  if (number <= 0) {
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if (number < 5) { //less than 5, calculated directly statistics
    for (i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  } else {
    if (arr[0] < arr[1]) {
      min = arr[0]; max = arr[1];
    }
    else {
      min = arr[1]; max = arr[0];
    }
    for (i = 2; i < number; i++) {
      if (arr[i] < min) {
        amount += min; //arr<min
        min = arr[i];
      } else {
        if (arr[i] > max) {
          amount += max; //arr>max
          max = arr[i];
        } else {
          amount += arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount / (number - 2);
  }//if
  return avg;
}
