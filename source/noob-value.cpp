/*
 * Project: Noob_Json_Parser
 * File: noob-value.cpp
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#include "header/noob-constant.h"
#include "header/noob-value.h"

#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * Type & Status String Definitions
 */

static const char *NoobTypeString[] = {
  "Null",
  "False",
  "True",
  "Number",
  "String",
  "Array",
  "Object"
};

static const char *NoobStatusString[] = {
  "Ok",
  "ExpectValue",
  "InvalidValue",
  "NotSigular",
  "NumberTooBig",
  "MissQuotationMark",
  "InvalidStringEscape",
  "InvalidStringChar",
  "InvalidUnicodeHex",
  "InvalidUnicodeSurrogate",
  "MissCommaOrSquareBracket",
  "MissKey",
  "MissColon",
  "MissCommaOrCurlyBracket"
};

/**
 * Tool Functions
 */

/*
 * Since we do not suggest using exceptions in C++,
 * If the users try to get the wrong type of value,
 * The getter functions will call NoobCrash()
 * And exit the program directly.
 */
void NoobCrash(const char *error_msg) {
  fprintf(stderr, "> ERROR: %s\n", error_msg);
  exit(31);
}

/**
 * Constructors & Destructors
 */

NoobValue::NoobValue() {
  _type = kNoobNull;
  _json = nullptr;
  _value._string = nullptr;
}

NoobValue::NoobValue(const char *json) {
  _type = kNoobNull;
  _json = nullptr;
  _value._string = nullptr;
  Parse(json);
}

NoobValue::~NoobValue() {
  Free();
}

/**
 * Type Wrapper
 */

NoobType NoobValue::GetType() const {
  return _type;
}

const char* NoobValue::GetTypeString() const {
  return NoobTypeString[_type];
}

/**
 * Boolean member wrapper
 */

bool NoobValue::GetBoolean() const {
  if(_type == kNoobTrue) {
    return true;
  } else if(_type == kNoobFalse) {
    return false;
  } else {
    NoobCrash("Try to access the boolean value of a non-boolean object!");
  }
}

void NoobValue::SetBoolean(bool boolean) {
  Free();
  _type = (boolean ? kNoobTrue : kNoobFalse);
}

/**
 * Number member wrapper
 */

double NoobValue::GetNumber() const {
  if(_type == kNoobNumber) {
    return _value._number;
  } else {
    NoobCrash("Try to access the numeric value of a non-number object!");
  }
}

void NoobValue::SetNumber(double number) {
  Free();
  _value._number = number;
  _type = kNoobNumber;
}

/**
 * String member wrapper
 */

std::string& NoobValue::GetString() const {
  if(_type == kNoobString) {
    return *(_value._string);
  } else {
    NoobCrash("Try to access the string value of a non-string object!");
  }
}

size_t NoobValue::GetLength() const {
  if(_type == kNoobString) {
    return (_value._string)->length();
  } else {
    NoobCrash("Try to access the string length of a non-string object!");
  }
}

void NoobValue::SetString(std::string &str) {
  Free();
  _value._string = new std::string(str);
  _type = kNoobString;
}

void NoobValue::SetString(const char *str, size_t length) {
  Free();
  _value._string = new std::string(str, length);
  _type = kNoobString;
}

/**
 * Array member wrapper
 */

NoobValue & NoobValue::GetElement(size_t index) const {
  if(_type == kNoobArray) {
    return *((_value._array)->at(index));
  } else {
    NoobCrash("Try to access the element of a non-array object!");
  }
}

size_t NoobValue::GetArraySize() const {
  if(_type == kNoobArray) {
    return (_value._array)->size();
  } else {
    NoobCrash("Try to access the size of a non-object object!");
  }
}

void NoobValue::SetArray(std::vector<NoobValue *> &vec) {
  Free();
  _value._array = new std::vector<NoobValue *>(vec);
  _type = kNoobArray;
}

/**
 * Object member wrapper
 */

NoobValue& NoobValue::GetValue(const std::string &key) const {
  if(_type == kNoobObject) {
    return *((_value._object)->at(key));
  } else {
    NoobCrash("Try to access the element of a non-object object!");
  }
}

bool NoobValue::HasKey(const std::string &key) const {
  if(_type == kNoobObject) {
    return ((_value._object)->count(key) > 0);
  } else {
    NoobCrash("Try to access the key of a non-object object!");
  }
}

size_t NoobValue::GetObjectSize() const {
  if(_type == kNoobObject) {
    return (_value._object)->size();
  } else {
    NoobCrash("Try to access the element of a non-object object!");
  }
}

void NoobValue::SetObject(std::unordered_map<std::string, NoobValue *> &obj) {
  Free();
  _value._object = new std::unordered_map<std::string, NoobValue *>(obj);
  _type = kNoobObject;
}

/**
 * Operator Overloading
 */

/*
 * Overloads [] to access the elements of the array.
 * Unlike the [] of std::vector, it is bound checked.
 * If the index is greater than the container size,
 * It will throw an out_of_range exception,
 * Because it calls the at() function of std::vector.
 */
NoobValue& NoobValue::operator[] (size_t index) const {
  if(_type == kNoobArray) {
    return *((_value._array)->at(index));
  } else {
    NoobCrash("Try to access the element of a non-array object!");
  }
}

/*
 * Overloads [] to access the elements of the object.
 * Unlike the [] operator of std::unordered_map,
 * If the key doesn't match the key of any element in the container,
 * The function throws an out_of_range exception,
 * Because it calls the at() function of std::unordered_map.
 * Therefore, the user must first use function HasKey()
 * To check the existence of the key.
 */
NoobValue& NoobValue::operator[] (const std::string &key) const {
  if(_type == kNoobObject) {
    return *((_value._object)->at(key));
  } else {
    NoobCrash("Try to access the element of a non-object object!");
  }
}

/**
 * Parser function
 */

NoobReturnValue NoobValue::Parse(const char *json) {
  Free();
  _json = json;
  _type = kNoobNull;
  NoobReturnValue result;
  ParseWhitespace();
  result = ParseValue();
  if(result == kNoobOk) {
    ParseWhitespace();
    if(*_json != '\0') {
      _type = kNoobNull;
      return kNoobNotSigular;
    }
  }
  return result;
}

/**
 * Stringify function
 */

NoobReturnValue NoobValue::Stringify(std::string &text) const {
  std::string json;
  switch(_type) {
    case kNoobNull: {
      return StringifyLiteral("null", text);
    }
    case kNoobTrue: {
      return StringifyLiteral("true", text);
    }
    case kNoobFalse: {
      return StringifyLiteral("false", text);
    }
    case kNoobNumber: {
      return StringifyNumber(text);
    }
    case kNoobString: {
      return StringifyString(text);
    }
    case kNoobArray: {
      return StringifyArray(text);
    }
    case kNoobObject: {
      return StringifyObject(text);
    }
    default: {
      return kNoobInvalidValue;
    }
  }
}

/**
 * Private functions
 */

void NoobValue::Free() {
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

void NoobValue::ParseWhitespace() {
  const char *pointer = _json;
  while(*pointer == ' ' || *pointer == '\t' ||
    *pointer == '\n' || *pointer == '\r') {
    ++pointer;
  }
  _json = pointer;
}

NoobReturnValue NoobValue::ParseValue() {
  switch(*_json) {
    case 'n': return ParseLiteral("null", kNoobNull);
    case 't': return ParseLiteral("true", kNoobTrue);
    case 'f': return ParseLiteral("false", kNoobFalse);
    case '"': return ParseString();
    case '[': return ParseArray();
    case '{': return ParseObject();
    case '\0': return kNoobExpectValue;
    default: return ParseNumber();
  }
}

NoobReturnValue NoobValue::ParseLiteral(const char *literal, NoobType type) {
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

NoobReturnValue NoobValue::ParseNumber() {
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

const char* NoobValue::ParseHex4(const char *pointer, unsigned *u) {
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

void NoobValue::EncodeUtf8(std::string *str, unsigned u) {
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

NoobReturnValue NoobDeleteString(std::string *str, NoobReturnValue result) {
  delete str;
  return result;
}

NoobReturnValue NoobValue::ParseStringRaw(std::string *str) {
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
        return NoobDeleteString(str, kNoobMissQuotationMark);
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
              return NoobDeleteString(str, kNoobInvalidUnicodeHex);
            if(u1 >= 0xD800 && u1 <= 0xDBFF) {
              if(*pointer++ != '\\')
                return NoobDeleteString(str, kNoobInvalidUnicodeSurrogate);
              if(*pointer++ != 'u')
                return NoobDeleteString(str, kNoobInvalidUnicodeSurrogate);
              pointer = ParseHex4(pointer, &u2);
              if(pointer == nullptr)
                return NoobDeleteString(str, kNoobInvalidUnicodeHex);
              if(u2 < 0xDC00 || u2 > 0xDFFF)
                return NoobDeleteString(str, kNoobInvalidUnicodeSurrogate);
              u1 = (((u1 - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
            }
            EncodeUtf8(str, u1);
            break;
          }
          default: {
            return NoobDeleteString(str, kNoobInvalidStringEscape);
          }
        }
        break;
      }
      default: {
        if((unsigned char)ch < 0x20)
          return NoobDeleteString(str, kNoobInvalidStringChar);
        str->push_back(ch);
      }
    }
  }
}

NoobReturnValue NoobValue::ParseString() {
  Free();
  NoobReturnValue result;
  _value._string = new std::string();
  result = ParseStringRaw(_value._string);
  if(result == kNoobOk) {
    _type = kNoobString;
  }
  return result;
}

NoobReturnValue NoobValue::ParseArray() {
  _json++;
  NoobReturnValue result;
  Free();
  _value._array = new std::vector<NoobValue *>();
  ParseWhitespace();
  if(*_json == ']') {
    _json++;
    _type = kNoobArray;
    return kNoobOk;
  }
  while(true) {
    NoobValue *element = new NoobValue();
    element->_json = _json;
    result = element->ParseValue();
    if(result != kNoobOk) {
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

NoobReturnValue NoobValue::ParseObject() {
  _json++;
  NoobReturnValue result;
  Free();
  _value._object = new std::unordered_map<std::string, NoobValue *>();
  ParseWhitespace();
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
    result = ParseStringRaw(&key);
    if(result != kNoobOk) {
      delete element;
      break;
    }
    ParseWhitespace();
    if(*_json != ':') {
      result = kNoobMissColon;
      delete element;
      break;
    }
    _json++;
    ParseWhitespace();
    element->_json = _json;
    result = element->ParseValue();
    if(result != kNoobOk) {
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

NoobReturnValue NoobValue::StringifyLiteral(
  const char *literal,
  std::string &text) const {
  text += literal;
  return kNoobOk;
}

NoobReturnValue NoobValue::StringifyNumber(std::string &text) const {
  char buffer[32];
  sprintf(buffer, "%.17g", _value._number);
  text += buffer;
  return kNoobOk;
}

NoobReturnValue NoobValue::StringifyString(std::string &text) const {
  static const char hex_digits[] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'A', 'B',
    'C', 'D', 'E', 'F'
  };

  text.push_back('\"');
  size_t length = (_value._string)->length();
  for(size_t i = 0; i < length; ++i) {
    char ch = (_value._string)->at(i);
    switch(ch) {
      case '\"': text += "\\\""; break;
      case '\\': text += "\\\\"; break;
      case '\b': text += "\\b"; break;
      case '\f': text += "\\f"; break;
      case '\n': text += "\\n"; break;
      case '\r': text += "\\r"; break;
      case '\t': text += "\\t"; break;
      default: {
        if(ch < 0x20) {
          text += "\\u00";
          text.push_back(hex_digits[ch >> 4]);
          text.push_back(hex_digits[ch & 15]);
        } else {
          text.push_back(ch);
        }
      }
    }
  }
  text.push_back('\"');
  return kNoobOk;
}

NoobReturnValue NoobValue::StringifyArray(std::string &text) const {
  text.push_back('[');
  size_t size = (_value._array)->size();
  for(size_t i = 0; i < size; ++i) {
    (_value._array)->at(i)->Stringify(text);
    text += ",";
  }
  if(text.back() == ',') {
    text.pop_back();
  }
  text.push_back(']');
  return kNoobOk;
}

NoobReturnValue NoobValue::StringifyObject(std::string &text) const {
  text.push_back('{');
  for(auto it = (_value._object)->begin();
    it != (_value._object)->end();
    ++it) {
    text.push_back('"');
    text += it->first;
    text.push_back('"');
    text.push_back(':');
    it->second->Stringify(text);
    text.push_back(',');
  }
  if(text.back() == ',') {
    text.pop_back();
  }
  text.push_back('}');
  return kNoobOk;
}

