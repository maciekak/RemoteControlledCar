#include "ProtocolReader.h"

//char BTReader::getDecodedChar(byte input){
//    uint8_t digitOffset = '9' - '0' + 2;
//    uint8_t smallCharOffset = 'z' - 'a' + digitOffset + 1;
//    if (input >= 1 && input <= 10){
//        return (char)(input + '0' -1);
//    }
//    if (input >= digitOffset && input <= smallCharOffset - 1){
//        return (char)(input + 'a' - digitOffset);
//    }
//    const int arrLen = 5;
//    char remaingChars[arrLen] = { ' ', ':', ';', '?', '!' };
//    int currentOffset = smallCharOffset;
//    for (int i = 0; i < arrLen; i++){
//        if (input == currentOffset){
//            return remaingChars[i];
//        }
//
//        currentOffset++;
//    }
//
//    return 'E';
//}
//
//void BTReader::getDecodedString(byte input[], char output[], int length){
//    for (int i = 0; i < length; i++){
//        char ch = getDecodedChar(input[i]);
//        output[i] = ch;
//    }
//
//    output[length] = '\0';
//}
//
//void BTReader::resetState(){
//    _state = State::NotStarted;
//    _velocityValue = 50;
//    _turnValue = 50; 
//}
//
//int BTReader::tryRead(byte input){
//    char decoded = getDecodedChar(input);
//    if(decoded == 'E'){
//        return 0;
//    }
//    switch(_state){
//        case State::NotStarted:
//            if(decoded == '?') {
//                _state = State::Begin;
//                resetState();
//            }
//            break;
//        case State::Begin:
//            if(decoded == 'v'){
//                _state = State::Velocity;
//            } else {
//                _state = State::NotStarted;
//            }
//            break;
//        case State::Velocity:
//            if(decoded = ':'){
//                _state = State::VelocityColon;
//            } else {
//                _state = State::NotStarted;
//            }
//            break;
//        case State::VelocityColon:
//            if(decoded >= '0' && decoded <= '9'){
//                _state = State::VelocityValue1;
//                int value = (int) (decoded - '0') * 10;
//                _velocityValue = value;
//            } else {
//                _state = State::NotStarted;
//            }
//            break;
//        case State::VelocityValue1:
//            if(decoded >= '0' && decoded <= '9'){
//                _state = State::VelocityValue2;
//                int value = (int) (decoded - '0');
//                _velocityValue += value;
//            } else {
//                _state = State::NotStarted;
//            }
//            break;
//        case State::VelocityValue2:
//            if(decoded = 't'){
//                _state = State::Turn;
//            } else {
//                _state = State::NotStarted;
//            }
//            break;
//        case State::Turn:
//            if(decoded = ':'){
//                _state = State::TurnColon;
//            } else {
//                _state = State::NotStarted;
//            }
//            break;
//        case State::TurnColon:
//            if(decoded >= '0' && decoded <= '9'){
//                _state = State::TurnValue1;
//                int value = (int) (decoded - '0') * 10;
//                _turnValue = value;
//            } else {
//                _state = State::NotStarted;
//            }
//            break;
//        case State::TurnValue1:
//            if(decoded >= '0' && decoded <= '9'){
//                _state = State::TurnValue2;
//                int value = (int) (decoded - '0');
//                _turnValue += value;
//            } else {
//                _state = State::NotStarted;
//            }
//            break;
//        case State::TurnValue2:
//            if(decoded == '!'){
//                _state = State::NotStarted;
//                return 1;
//            }
//            break;
//    }
//
//    return 0;
//}
//
//Command BTReader::getLastCommand(){
//    Command result = Command();
//    result.turn = _turnValue;
//    result.velocity = _velocityValue;
//}