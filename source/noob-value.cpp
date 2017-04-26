/*
 * Project: Noob_Json_Parser
 * File: noob-value.cpp
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#include "../header/noob-constant.h"
#include "../header/noob-value.h"

#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>

/*
 * Constructors and destructors
 */

NoobValue::NoobValue() {
  _type = kNoobNull;
  _json = nullptr;
  _value._string = nullptr;
}

NoobValue::~NoobValue() {
  NoobFree();
}

NoobType NoobValue::NoobGetType() const { return _type; }

void NoobValue::NoobSetType(NoobType type) { _type = type; }

bool NoobValue::NoobGetBoolean() const { return boolean(); }

double NoobValue::NoobGetNumber() const { return number(); }

const std::string& NoobValue::NoobGetString() const { return string(); }

size_t NoobValue::NoobGetStringLength() const { return length(); }

const NoobValue& NoobValue::NoobGetArrayElement(size_t index) const { return at(index); }

size_t NoobValue::NoobGetArraySize() const { return array_size(); }

size_t NoobValue::NoobGetObjectSize() const { return object_size(); }

bool NoobValue::NoobHasKey(const std::string &key) const { return has_key(key); }

const NoobValue& NoobValue::NoobGetObjectValue(const std::string &key) const { return at(key); }

NoobReturnValue NoobValue::NoobParse(const char *json) {
  NoobFree();
  _json = json;
  _type = kNoobNull;
  NoobReturnValue result;
  NoobParseWhitespace();
  result = NoobParseValue();
  if(result == kNoobOk) {
    NoobParseWhitespace();
    if(*_json != '\0') {
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
  } else if(_type == kNoobArray) {
    size_t array_size = (_value._array)->size();
    for(size_t i = 0; i < array_size; ++i) {
      delete (_value._array)->at(i);
    }
    delete _value._array;
    _value._array = nullptr;
  } else if(_type == kNoobObject) {
    for(auto it = (_value._object)->begin();
      it != (_value._object)->end();
      ++it) {
      delete it->second;
    }
    delete _value._object;
    _value._object = nullptr;
  }
  _type = kNoobNull;
}

void NoobValue::NoobSetBoolean(bool boolean) {
  NoobFree();
  _type = (boolean ? kNoobTrue : kNoobFalse);
}

void NoobValue::NoobSetNumber(double number) {
  NoobFree();
  _value._number = number;
  _type = kNoobNumber;
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

void NoobValue::NoobParseWhitespace() {
  const char *pointer = _json;
  while(*pointer == ' ' || *pointer == '\t' ||
    *pointer == '\n' || *pointer == '\r') {
    ++pointer;
  }
  _json = pointer;
}

NoobReturnValue NoobValue::NoobParseValue() {
  switch(*_json) {
    case 'n': return NoobParseLiteral("null", kNoobNull);
    case 't': return NoobParseLiteral("true", kNoobTrue);
    case 'f': return NoobParseLiteral("false", kNoobFalse);
    case '"': return NoobParseString();
    case '[': return NoobParseArray();
    case '{': return NoobParseObject();
    case '\0': return kNoobExpectValue;
    default: return NoobParseNumber();
  }
}

NoobReturnValue NoobValue::NoobParseLiteral(const char* literal, NoobType type) {
  _json++;
  const char *pointer = _json;
  for (size_t i = 0; literal[i + 1]; ++i)
    if (*(pointer++) != literal[i + 1])
      return kNoobInvalidValue;
  _json = pointer;
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
  const char *pointer = _json;
  if(*pointer == '-') pointer++;
  if(*pointer == '0') {
    pointer++;
  } else {
    if(!IsDigit1To9(*pointer)) return kNoobInvalidValue;
    do pointer++; while(IsDigit(*pointer));
  }
  if(*pointer == '.') {
    pointer++;
    if(!IsDigit(*pointer)) return kNoobInvalidValue;
    do pointer++; while(IsDigit(*pointer));
  }
  if(*pointer == 'e' || *pointer == 'E') {
    pointer++;
    if(*pointer == '+' || *pointer == '-') pointer++;
    if(!IsDigit(*pointer)) return kNoobInvalidValue;
    do pointer++; while(IsDigit(*pointer));
  }

  errno = 0;
  _value._number = strtod(_json, nullptr);
  if(errno == ERANGE &&
    (_value._number == HUGE_VAL ||
      _value._number == -HUGE_VAL)) {
    return kNoobNumberTooBig;
  }
  _type = kNoobNumber;
  _json = pointer;
  return kNoobOk;
}

const char* NoobValue::NoobParseHex4(const char *pointer, unsigned *u) {
  *u = 0;
  for (int i = 0; i < 4; ++i) {
    char ch = *pointer++;
    *u <<= 4;
    if(ch >= '0' && ch <= '9') *u |= ch - '0';
    else if(ch >= 'A' && ch <= 'F') *u |= ch - ('A' - 10);
    else if(ch >= 'a' && ch <= 'f') *u |= ch - ('a' - 10);
    else return NULL;
  }
  return pointer;
}

void NoobValue::NoobEncodeUtf8(std::string *str, unsigned u) {
  if (u <= 0x7F) {
    str->push_back(u & 0xFF);
  } else if (u <= 0x7FF) {
    str->push_back(0xC0 | ((u >> 6) & 0xFF));
    str->push_back(0x80 | ( u       & 0x3F));
  } else if (u <= 0xFFFF) {
    str->push_back(0xE0 | ((u >> 12) & 0xFF));
    str->push_back(0x80 | ((u >>  6) & 0x3F));
    str->push_back(0x80 | ( u        & 0x3F));
  } else {
    str->push_back(0xF0 | ((u >> 18) & 0xFF));
    str->push_back(0x80 | ((u >> 12) & 0x3F));
    str->push_back(0x80 | ((u >>  6) & 0x3F));
    str->push_back(0x80 | ( u        & 0x3F));
  }
}

NoobReturnValue NoobValue::NoobParseStringRaw(std::string *str) {
  _json++;
  const char *pointer = _json;
  while(true) {
    char ch = *pointer++;
    switch(ch) {
      case '\"': {
        _json = pointer;
        return kNoobOk;
      }
      case '\0': {
        delete str;
        return kNoobMissQuotationMark;
      }
      case '\\': {
        switch(*pointer++) {
          case '\"': str->push_back('\"'); break;
          case '\\': str->push_back('\\'); break;
          case '/':  str->push_back('/' ); break;
          case 'b':  str->push_back('\b'); break;
          case 'f':  str->push_back('\f'); break;
          case 'n':  str->push_back('\n'); break;
          case 'r':  str->push_back('\r'); break;
          case 't':  str->push_back('\t'); break;
          case 'u': {
            unsigned u1, u2;
            pointer = NoobParseHex4(pointer, &u1);
            if(pointer == nullptr) {
              delete str;
              return kNoobInvalidUnicodeHex;
            }
            if(u1 >= 0xD800 && u1 <= 0xDBFF) {
              if(*pointer++ != '\\') {
                delete str;
                return kNoobInvalidUnicodeSurrogate;
              }
              if(*pointer++ != 'u') {
                delete str;
                return kNoobInvalidUnicodeSurrogate;
              }
              pointer = NoobParseHex4(pointer, &u2);
              if(pointer == nullptr) {
                delete str;
                return kNoobInvalidUnicodeHex;
              }
              if(u2 < 0xDC00 || u2 > 0xDFFF) {
                delete str;
                return kNoobInvalidUnicodeSurrogate;
              }
              u1 = (((u1 - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
            }
            NoobEncodeUtf8(str, u1);
            break;
          }
          default: {
            delete str;
            return kNoobInvalidStringEscape;
          }
        }
        break;
      }
      default: {
        if((unsigned char)ch < 0x20) {
          delete str;
          return kNoobInvalidStringChar;
        }
        str->push_back(ch);
      }
    }
  }
}

NoobReturnValue NoobValue::NoobParseString() {
  NoobFree();
  NoobReturnValue result;
  _value._string = new std::string();
  result = NoobParseStringRaw(_value._string);
  if(result == kNoobOk) {
    _type = kNoobString;
  }
  return result;
}

NoobReturnValue NoobValue::NoobParseArray() {
  _json++;
  NoobReturnValue result;
  NoobFree();
  _value._array = new std::vector<NoobValue *>();
  NoobParseWhitespace();
  if(*_json == ']') {
    _json++;
    _type = kNoobArray;
    return kNoobOk;
  }
  while(true) {
    NoobValue *element = new NoobValue();
    element->_json = _json;
    result = element->NoobParseValue();
    if(result != kNoobOk) {
      delete element;
      break;
    } else {
      (_value._array)->push_back(element);
      _json = element->_json;
      NoobParseWhitespace();
      if(*_json == ',') {
        _json++;
        NoobParseWhitespace();
      } else if(*_json == ']') {
        _json++;
        _type = kNoobArray;
        return kNoobOk;
      } else {
        result = kNoobMissCommaOrSquareBracket;
        break;
      }
    }
  }
  size_t array_size = (_value._array)->size();
  for(size_t i = 0; i < array_size; ++i) {
    delete (_value._array)->at(i);
  }
  delete _value._array;
  _value._array = nullptr;
  return result;
}

NoobReturnValue NoobValue::NoobParseObject() {
  _json++;
  NoobReturnValue result;
  NoobFree();
  _value._object = new std::unordered_map<std::string, NoobValue *>();
  NoobParseWhitespace();
  if(*_json == '}') {
    _json++;
    _type = kNoobObject;
    return kNoobOk;
  }
  while(true) {
    NoobValue *element = new NoobValue();
    std::string key = "";

    if(*_json != '"') {
      result = kNoobMissKey;
      delete element;
      break;
    }
    result = NoobParseStringRaw(&key);
    if(result != kNoobOk) {
      delete element;
      break;
    }
    NoobParseWhitespace();
    if(*_json != ':') {
      result = kNoobMissColon;
      delete element;
      break;
    }
    _json++;
    NoobParseWhitespace();
    element->_json = _json;
    result = element->NoobParseValue();
    if(result != kNoobOk) {
      delete element;
      break;
    }
    (_value._object)->insert({key, element});
    _json = element->_json;
    NoobParseWhitespace();
    if(*_json == ',') {
      _json++;
      NoobParseWhitespace();
    } else if(*_json == '}') {
      _json++;
      _type = kNoobObject;
      return kNoobOk;
    } else {
      result = kNoobMissCommaOrCurlyBracket;
      break;
    }
  }
  for(auto it = (_value._object)->begin();
    it != (_value._object)->end();
    ++it) {
    delete it->second;
  }
  delete _value._object;
  _value._object = nullptr;
  return result;
}
