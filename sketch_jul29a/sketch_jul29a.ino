#include "SoftwareSerial.h"
#define LedPin 53
#define BTTX 51
#define BTRX 49

SoftwareSerial SerialBT(BTTX, BTRX);

void toBinary(byte in, char resultArr[9]) {
  resultArr[8] = '\0';
  for(int i = 7; in != 0; i--){
      char bit = in%2==0 ?'1':'0'; 
      in /= 2;
      resultArr[i] = bit;
  }
}

byte decodeByte(byte in){
  in ^= 255;
  in >>= 1;
  return in;
}



char getDecodedChar(byte input)
{
    uint8_t digitOffset = '9' - '0' + 1;
    uint8_t smallCharOffset = 'z' - 'a' + digitOffset + 1;
    if (input >= 0 && input <= 9)
    {
        return (char)(input + '0');
    }
    if (input >= digitOffset && input <= smallCharOffset - 1)
    {
        return (char)(input + 'a' - digitOffset);
    }
    const int arrLen = 5;
    char remaingChars[arrLen] = { ' ', ':', ';', '?', '!' };
    int currentOffset = smallCharOffset;
    for (int i = 0; i < arrLen; i++)
    {
        if (input == currentOffset)
        {
            return remaingChars[i];
        }

        currentOffset++;
    }

    return 'E';
}

void getDecodedString(byte input[], char output[], int length)
{
    for (int i = 0; i < length; i++)
    {
        char ch = getDecodedChar(input[i]);
        output[i] = ch;
    }

    output[length] = '\0';
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LedPin, OUTPUT);
  SerialBT.begin(9600);
  //SerialBT.println("Bluetooth connection established");
}

//void writeBits(byte single)
void loop() {
  // put your main code here, to run repeatedly:
  //SerialBT.print("abcd");
  if (SerialBT.available()){
    byte arr[4];
    size_t readBytes = SerialBT.readBytes(arr, 4);
    Serial.print("len: ");
    Serial.print(readBytes);
    Serial.print(' ');
    byte resultBytes[4];
    char str[5];
    for(int i = 0; i < readBytes; i++){
      char message[9] = {'0','0','0','0','0','0','0','0', '\0'};
      toBinary(arr[i], message);
      Serial.print(message);
      Serial.print(' ');
      byte decoded = decodeByte(arr[i]);
      Serial.print(decoded);
      Serial.print(' ');
      resultBytes[i] = decoded;
    }
    getDecodedString(resultBytes, str, readBytes);

    Serial.print(" : ");

    Serial.println(str);
  }
  delay(200);

  delay(200);
}
