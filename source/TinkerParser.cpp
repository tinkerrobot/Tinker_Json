/*
 * Project: Tinker_Json_Parser
 * File: TinkerParser.cpp
 * Author: Ling.Li
 * Date: 2017/5/4
 */

#include "source/TinkerConstant.h"
#include "source/TinkerValue.h"

#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>


namespace Tinker {
/**
 * Tool functions
 */

inline bool IsDigit(char ch) {
  return (ch >= '0' && ch <= '9');
}

inline bool IsDigit1To9(char ch) {
  return (ch >= '1' && ch <= '9');
}

ReturnValue DeleteString(
  std::string *str,
  ReturnValue result) {
  delete str;
  return result;
}

inline const char* ParseHex4(const char *pointer, unsigned *u) {
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

inline void EncodeUtf8(std::string *str, unsigned u) {
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

/**
 * Parser functions
 */

/*
 * Note that this is the only API exposed to the users.
 * Any other parser functions are private and invisible to the outside.
 */
ReturnValue Value::Parse(const char *json) {
  Free();
  _json = json;
  _type = kNull;
  ReturnValue result;
  ParseWhitespace();
  result = ParseValue();
  if(result == kOk) {
    ParseWhitespace();
    if(*_json != '\0') {
      _type = kNull;
      return kNotSingular;
    }
  }
  return result;
}

/**
 * The functions below are private.
 */

void Value::ParseWhitespace() {
  const char *pointer = _json;
  while(*pointer == ' ' || *pointer == '\t' ||
    *pointer == '\n' || *pointer == '\r') {
    ++pointer;
  }
  _json = pointer;
}

ReturnValue Value::ParseValue() {
  switch(*_json) {
    case 'n': return ParseLiteral("null", kNull);
    case 't': return ParseLiteral("true", kTrue);
    case 'f': return ParseLiteral("false", kFalse);
    case '"': return ParseString();
    case '[': return ParseArray();
    case '{': return ParseObject();
    case '\0': return kExpectValue;
    default: return ParseNumber();
  }
}

ReturnValue Value::ParseLiteral(const char *literal, Type type) {
  _json++;
  const char *pointer = _json;
  for (size_t i = 0; literal[i + 1]; ++i)
    if (*(pointer++) != literal[i + 1])
      return kInvalidValue;
  _json = pointer;
  _type = type;
  return kOk;
}

ReturnValue Value::ParseNumber() {
  const char *pointer = _json;
  if(*pointer == '-') pointer++;
  if(*pointer == '0') {
    pointer++;
  } else {
    if(!IsDigit1To9(*pointer)) return kInvalidValue;
    do pointer++; while(IsDigit(*pointer));
  }
  if(*pointer == '.') {
    pointer++;
    if(!IsDigit(*pointer)) return kInvalidValue;
    do pointer++; while(IsDigit(*pointer));
  }
  if(*pointer == 'e' || *pointer == 'E') {
    pointer++;
    if(*pointer == '+' || *pointer == '-') pointer++;
    if(!IsDigit(*pointer)) return kInvalidValue;
    do pointer++; while(IsDigit(*pointer));
  }

  errno = 0;
  _value._number = strtod(_json, nullptr);
  if(errno == ERANGE &&
    (_value._number == HUGE_VAL ||
      _value._number == -HUGE_VAL)) {
    return kNumberTooBig;
  }
  _type = kNumber;
  _json = pointer;
  return kOk;
}

ReturnValue Value::ParseRawString(std::string *str) {
  _json++;
  const char *pointer = _json;
  while(true) {
    char ch = *pointer++;
    switch(ch) {
      case '\"': {
        _json = pointer;
        return kOk;
      }
      case '\0': {
        return DeleteString(str, kMissQuotationMark);
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
            pointer = ParseHex4(pointer, &u1);
            if(pointer == nullptr)
              return DeleteString(str, kInvalidUnicodeHex);
            if(u1 >= 0xD800 && u1 <= 0xDBFF) {
              if(*pointer++ != '\\')
                return DeleteString(str, kInvalidUnicodeSurrogate);
              if(*pointer++ != 'u')
                return DeleteString(str, kInvalidUnicodeSurrogate);
              pointer = ParseHex4(pointer, &u2);
              if(pointer == nullptr)
                return DeleteString(str, kInvalidUnicodeHex);
              if(u2 < 0xDC00 || u2 > 0xDFFF)
                return DeleteString(str, kInvalidUnicodeSurrogate);
              u1 = (((u1 - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
            }
            EncodeUtf8(str, u1);
            break;
          }
          default: {
            return DeleteString(str, kInvalidStringEscape);
          }
        }
        break;
      }
      default: {
        if((unsigned char)ch < 0x20)
          return DeleteString(str, kInvalidStringChar);
        str->push_back(ch);
      }
    }
  }
}

ReturnValue Value::ParseString() {
  Free();
  ReturnValue result;
  _value._string = new std::string();
  result = ParseRawString(_value._string);
  if(result == kOk) {
    _type = kString;
  }
  return result;
}

ReturnValue Value::ParseArray() {
  _json++;
  ReturnValue result;
  Free();
  _value._array = new std::vector<Value *>();
  ParseWhitespace();
  if(*_json == ']') {
    _json++;
    _type = kArray;
    return kOk;
  }
  while(true) {
    Value *element = new Value();
    element->_json = _json;
    result = element->ParseValue();
    if(result != kOk) {
      delete element;
      break;
    } else {
      (_value._array)->push_back(element);
      _json = element->_json;
      ParseWhitespace();
      if(*_json == ',') {
        _json++;
        ParseWhitespace();
      } else if(*_json == ']') {
        _json++;
        _type = kArray;
        return kOk;
      } else {
        result = kMissCommaOrSquareBracket;
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

ReturnValue Value::ParseObject() {
  _json++;
  ReturnValue result;
  Free();
  _value._object = new std::unordered_map<std::string, Value *>();
  ParseWhitespace();
  if(*_json == '}') {
    _json++;
    _type = kObject;
    return kOk;
  }
  while(true) {
    Value *element = new Value();
    std::string key = "";

    if(*_json != '"') {
      result = kMissKey;
      delete element;
      break;
    }
    result = ParseRawString(&key);
    if(result != kOk) {
      delete element;
      break;
    }
    ParseWhitespace();
    if(*_json != ':') {
      result = kMissColon;
      delete element;
      break;
    }
    _json++;
    ParseWhitespace();
    element->_json = _json;
    result = element->ParseValue();
    if(result != kOk) {
      delete element;
      break;
    }
    (_value._object)->insert({key, element});
    _json = element->_json;
    ParseWhitespace();
    if(*_json == ',') {
      _json++;
      ParseWhitespace();
    } else if(*_json == '}') {
      _json++;
      _type = kObject;
      return kOk;
    } else {
      result = kMissCommaOrCurlyBracket;
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
}
