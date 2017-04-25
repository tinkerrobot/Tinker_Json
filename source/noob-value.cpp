/*
 * Project: Noob_Json_Parser
 * File: noob-value.cpp
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#include "../header/std-header.h"
#include "../header/noob-enum.h"
#include "../header/noob-value.h"
/*
 * Tool functions
 */

NoobValue::NoobValue() {
  _type = kNoobNull;
  _context = nullptr;
  _value._string = nullptr;
}

NoobValue::~NoobValue() {
  NoobFree();
  delete _context;
  _context = nullptr;
}

NoobType NoobValue::NoobGetType() {
  return _type;
}

void NoobValue::NoobSetType(NoobType type) {
  _type = type;
}

bool NoobValue::NoobGetBoolean() {
  if(_type == kNoobTrue) {
    return true;
  } else if(_type == kNoobFalse) {
    return false;
  } else {
    printf("ERROR: Try to access the boolean value of a non-boolean object!\n");
    exit(-1);
  }
}

void NoobValue::NoobSetBoolean(bool boolean) {
  NoobFree();
  _type = (boolean ? kNoobTrue : kNoobFalse);
}

double NoobValue::NoobGetNumber() {
  if(_type == kNoobNumber) {
    return _value._number;
  } else {
    printf("ERROR: Try to access the numeric value of a non-number object!\n");
    exit(-1);
  }
}

void NoobValue::NoobSetNumber(double number) {
  NoobFree();
  _value._number = number;
  _type = kNoobNumber;
}

const std::string* NoobValue::NoobGetString() {
  if(_type == kNoobString) {
    return _value._string;
  } else {
    printf("ERROR: Try to access the string value of a non-string object!\n");
    exit(-1);
  }
}

size_t NoobValue::NoobGetStringLength() {
  if(_type == kNoobString) {
    return (_value._string)->length();
  } else {
    printf("ERROR: Try to access the string length of a non-string object!\n");
    exit(-1);
  }
}

void NoobValue::NoobSetString(std::string *str) {
  NoobFree();
  _value._string = new std::string(*str);
  _type = kNoobString;
}

void NoobValue::NoobSetString(std::string &str) {
  NoobFree();
  _value._string = new std::string(str);
  _type = kNoobString;
}

void NoobValue::NoobSetString(const char *str, size_t length) {
  NoobFree();
  _value._string = new std::string(str, length);
  _type = kNoobString;
}

NoobReturnValue NoobValue::NoobParse(const char *json) {
  delete _context;
  _context = nullptr;
  _context = new NoobContext(json);
  _type = kNoobNull;
  NoobReturnValue result = kNoobInvalidValue;
  NoobParseWhitespace();
  result = NoobParseValue();
  if(result == kNoobOk) {
    NoobParseWhitespace();
    if(*(_context->NoobGetJson()) != '\0') {
      _type = kNoobNull;
      return kNoobNotSigular;
    }
  }
  return result;
}

/*
 * Private functions
 */

void NoobValue::NoobFree() {
  if(_type == kNoobString) {
    delete _value._string;
    _value._string = nullptr;
  }
  _type = kNoobNull;
}

void NoobValue::NoobParseWhitespace() {
  const char *pointer = _context->NoobGetJson();
  while(*pointer == ' ' ||
    *pointer == '\t' ||
    *pointer == '\n' ||
    *pointer == '\r') {
    ++pointer;
  }
  _context->NoobSetJson(pointer);
}

NoobReturnValue NoobValue::NoobParseValue() {
  switch(*(_context->NoobGetJson())) {
    case 'n': {
      return NoobParseLiteral("null", kNoobNull);
    }
    case 't': {
      return NoobParseLiteral("true", kNoobTrue);
    }
    case 'f': {
      return NoobParseLiteral("false", kNoobFalse);
    }
    case '"': {
      return NoobParseString();
    }
    case '\0': {
      return kNoobExpectValue;
    }
    default: {
      return NoobParseNumber();
    }
  }
}

NoobReturnValue NoobValue::NoobParseLiteral(const char* literal, NoobType type) {
  _context->NoobMoveForward(1);
  const char *pointer = _context->NoobGetJson();
  for (size_t i = 0; literal[i + 1]; ++i)
    if (*(pointer++) != literal[i + 1])
      return kNoobInvalidValue;
  _context->NoobSetJson(pointer);
  _type = type;
  return kNoobOk;
}

inline bool IsDigit(char ch) {
  return (ch >= '0' && ch <= '9');
}

inline bool IsDigit1To9(char ch) {
  return (ch >= '1' && ch <= '9');
}

NoobReturnValue NoobValue::NoobParseNumber() {
  const char *pointer = _context->NoobGetJson();
  if(*pointer == '-') {
    pointer++;
  }
  if(*pointer == '0') {
    pointer++;
  } else {
    if(!IsDigit1To9(*pointer)) {
      return kNoobInvalidValue;
    }
    do {
      pointer++;
    } while(IsDigit(*pointer));
  }
  if(*pointer == '.') {
    pointer++;
    if(!IsDigit(*pointer)) {
      return kNoobInvalidValue;
    }
    do {
      pointer++;
    } while(IsDigit(*pointer));
  }
  if(*pointer == 'e' || *pointer == 'E') {
    pointer++;
    if(*pointer == '+' || *pointer == '-') {
      pointer++;
    }
    if(!IsDigit(*pointer)) {
      return kNoobInvalidValue;
    }
    do {
      pointer++;
    } while(IsDigit(*pointer));
  }

  errno = 0;
  _value._number = strtod(_context->NoobGetJson(), nullptr);
  if(errno == ERANGE &&
    (_value._number == HUGE_VAL ||
      _value._number == -HUGE_VAL)) {
    return kNoobNumberTooBig;
  }
  _type = kNoobNumber;
  _context->NoobSetJson(pointer);
  return kNoobOk;
}

NoobReturnValue NoobValue::NoobParseString() {
  _context->NoobMoveForward(1);
  std::string buffer = "";
  const char *pointer = _context->NoobGetJson();
  while(true) {
    char ch = *pointer++;
    switch(ch) {
      case '\"': {
        NoobSetString(buffer);
        _context->NoobSetJson(pointer);
        return kNoobOk;
      }
      case '\0': {
        return kNoobMissQuotationMark;
      }
      case '\\': {
        switch(*pointer++) {
          case '\"': buffer.push_back('\"'); break;
          case '\\': buffer.push_back('\\'); break;
          case '/':  buffer.push_back('/' ); break;
          case 'b':  buffer.push_back('\b'); break;
          case 'f':  buffer.push_back('\f'); break;
          case 'n':  buffer.push_back('\n'); break;
          case 'r':  buffer.push_back('\r'); break;
          case 't':  buffer.push_back('\t'); break;
          default: return kNoobInvalidStringEscape;
        }
        break;
      }
      default: {
        if((unsigned char)ch < 0x20) {
          return kNoobInvalidStringChar;
        }
        buffer.push_back(ch);
      }
    }
  }
}