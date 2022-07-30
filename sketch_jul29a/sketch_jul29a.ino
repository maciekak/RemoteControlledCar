#include "SoftwareSerial.h"

enum State {
    NotStarted = 0,
    Begin,
    Velocity,
    VelocityColon,
    VelocityValue1,
    VelocityValue2,
    Turn,
    TurnColon,
    TurnValue1,
    TurnValue2
};

struct Command {
    uint8_t velocity;
    uint8_t turn;
};

class BTReader{
    private:
    int _velocityValue = 50;
    int _turnValue = 50; 
    State _state = State::NotStarted;

    char getDecodedChar(uint8_t input);
    void getDecodedString(uint8_t input[], char output[], int length);
    void resetState();
    byte decodeByte(byte in);

    public:
    int tryRead(uint8_t input);
    Command getLastCommand();
};

/////////////////////////////

char BTReader::getDecodedChar(byte input){
    uint8_t digitOffset = '9' - '0' + 2;
    uint8_t smallCharOffset = 'z' - 'a' + digitOffset + 1;
    if (input >= 1 && input <= 10){
        return (char)(input + '0' -1);
    }
    if (input >= digitOffset && input <= smallCharOffset - 1){
        return (char)(input + 'a' - digitOffset);
    }
    const int arrLen = 5;
    char remaingChars[arrLen] = { ' ', ':', ';', '?', '!' };
    int currentOffset = smallCharOffset;
    for (int i = 0; i < arrLen; i++){
        if (input == currentOffset){
            return remaingChars[i];
        }

        currentOffset++;
    }

    return 'E';
}
char getDecodedChar(byte input){
    uint8_t digitOffset = '9' - '0' + 2;
    uint8_t smallCharOffset = 'z' - 'a' + digitOffset + 1;
    if (input >= 1 && input <= 10){
        return (char)(input + '0' -1);
    }
    if (input >= digitOffset && input <= smallCharOffset - 1){
        return (char)(input + 'a' - digitOffset);
    }
    const int arrLen = 5;
    char remaingChars[arrLen] = { ' ', ':', ';', '?', '!' };
    int currentOffset = smallCharOffset;
    for (int i = 0; i < arrLen; i++){
        if (input == currentOffset){
            return remaingChars[i];
        }

        currentOffset++;
    }

    return 'E';
}

void getDecodedString(byte input[], char output[], int length){
    for (int i = 0; i < length; i++){
        char ch = getDecodedChar(input[i]);
        output[i] = ch;
    }

    output[length] = '\0';
}

void BTReader::resetState(){
    _state = State::NotStarted;
    _velocityValue = 50;
    _turnValue = 50; 
}

int BTReader::tryRead(byte input){
    input = decodeByte(input);
    char decoded = getDecodedChar(input);
    if(decoded == 'E'){
        return 0;
    }
    switch(_state){
        case State::NotStarted:
            if(decoded == '?') {
                resetState();
                _state = State::Begin;
            }
            break;
        case State::Begin:
            if(decoded == 'v'){
                _state = State::Velocity;
            } else {
                _state = State::NotStarted;
            }
            break;
        case State::Velocity:
            if(decoded = ':'){
                _state = State::VelocityColon;
            } else {
                _state = State::NotStarted;
            }
            break;
        case State::VelocityColon:
            if(decoded >= '0' && decoded <= '9'){
                _state = State::VelocityValue1;
                int value = (int) (decoded - '0') * 10;
                _velocityValue = value;
            } else {
                _state = State::NotStarted;
            }
            break;
        case State::VelocityValue1:
            if(decoded >= '0' && decoded <= '9'){
                _state = State::VelocityValue2;
                int value = (int) (decoded - '0');
                _velocityValue += value;
            } else {
                _state = State::NotStarted;
            }
            break;
        case State::VelocityValue2:
            if(decoded = 't'){
                _state = State::Turn;
            } else {
                _state = State::NotStarted;
            }
            break;
        case State::Turn:
            if(decoded = ':'){
                _state = State::TurnColon;
            } else {
                _state = State::NotStarted;
            }
            break;
        case State::TurnColon:
            if(decoded >= '0' && decoded <= '9'){
                _state = State::TurnValue1;
                int value = (int) (decoded - '0') * 10;
                _turnValue = value;
            } else {
                _state = State::NotStarted;
            }
            break;
        case State::TurnValue1:
            if(decoded >= '0' && decoded <= '9'){
                _state = State::TurnValue2;
                int value = (int) (decoded - '0');
                _turnValue += value;
            } else {
                _state = State::NotStarted;
            }
            break;
        case State::TurnValue2:
            if(decoded == '!'){
                _state = State::NotStarted;
                return 1;
            }
            break;
    }

    return 0;
}

Command BTReader::getLastCommand(){
    Command result = Command();
    result.turn = _turnValue;
    result.velocity = _velocityValue;
    return result;
}


///////////////////////////////////////////////////////////////////////////////////////////

#define LedPin 53
#define BTTX 51
#define BTRX 49
#define LedLeft 47
#define LedRight 45

SoftwareSerial SerialBT(BTTX, BTRX);
BTReader reader = BTReader();

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

byte BTReader::decodeByte(byte in){
  in ^= 255;
  in >>= 1;
  return in;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LedPin, OUTPUT);
  pinMode(LedLeft, OUTPUT);
  pinMode(LedRight, OUTPUT);
  SerialBT.begin(9600);
  //SerialBT.println("Bluetooth connection established");
} 

//void writeBits(byte single)
void loop() {
  // put your main code here, to run repeatedly:
  if (SerialBT.available()){
    byte arr[4];
    size_t readBytes = SerialBT.readBytes(arr, 4);
    //Serial.print("len: ");
    //Serial.print(readBytes);
    //Serial.print(' ');
    byte resultBytes[4];
    char str[5];
    for(int i = 0; i < readBytes; i++){
      char message[9] = {'0','0','0','0','0','0','0','0', '\0'};

      //Serial.print(' ');
      //Serial.print((char) (arr[i] ^ 255));
      if(reader.tryRead(arr[i])){
        Command command = reader.getLastCommand();
        Serial.print("Turn: ");
        Serial.println(command.turn);
        if(command.turn < 75) {
          digitalWrite(LedLeft, HIGH);
        }
        if(command.turn >= 75) {
          digitalWrite(LedLeft, LOW);
        }
        if(command.turn > 25){
          digitalWrite(LedRight, HIGH);
        }
        if(command.turn <= 25){
          digitalWrite(LedRight, LOW);
        }
      }
      //toBinary(arr[i], message);
      //Serial.print(message);
      //Serial.print(' ');
      //byte decoded = decodeByte(arr[i]);
      //Serial.print(decoded);
      //Serial.print(' ');
      //resultBytes[i] = decoded;
    }
    Serial.println();
    //Serial.println();
    //getDecodedString(resultBytes, str, readBytes);
    //Serial.print(" : ");
  //
    //Serial.println(str);
  }
  delay(200);
  digitalWrite(LedPin, HIGH);
  delay(100);
  digitalWrite(LedPin, LOW);
}
