#include "SoftwareSerial.h"
#define LedPin 53
#define BTTX 51
#define BTRX 49

//maciek to kurwa jebana
//trrsfdfsaaaaaaaaaa


SoftwareSerial SerialBT(BTTX, BTRX);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LedPin, OUTPUT);
  //SerialBT.begin(9600);
  //SerialBT.println("Bluetooth connection established");
}

//void writeBits(byte single)
void loop() {
  if(Serial.available()){
    String msg = Serial.readString();
    Serial.println(msg);
    if(msg[0] == '0'){
      digitalWrite(LedPin, LOW);
    } else if(msg[0] == '1'){
      digitalWrite(LedPin, HIGH);
    }

  }
  // put your main code here, to run repeatedly:
  //SerialBT.print("abcd");
  //if (SerialBT.available()){
  //  byte arr[4];
  //  size_t msg = SerialBT.readBytes(arr, 4);
  //  Serial.print("byte: ");
  //  Serial.write(msg);
  //  Serial.println();
  //  char r;
  //  char message[9] = {'0','0','0','0','0','0','0','0', '\0'};
  //  //for(int i = 7; msg != 0; i--){
  //  //  r=(msg%2==0 ?'0':'1'); 
  //  //  msg /= 2;
  //  //  message[i] = r;
  //  //}
  //  for(int i = 7; msg != 0; i--){
  //    r=(msg%2==0 ?'1':'0'); 
  //    msg /= 2;
  //    message[i] = r;
  //  }
  //  Serial.write(message);
  //}
  delay(100);
  //delay(200);
}
