/*
 * Project: Tinker_Json_Parser
 * File: TinkerStringifier.cpp
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
 * Stringify function
 */

/*
 * Note that this is the only API exposed to the users.
 * Any other stringifier functions are private
 * and invisible to the outside.
 */
ReturnValue Value::Stringify(std::string &text) const {
  switch(_type) {
    case kNull: {
      return StringifyLiteral("null", text);
    }
    case kTrue: {
      return StringifyLiteral("true", text);
    }
    case kFalse: {
      return StringifyLiteral("false", text);
    }
    case kNumber: {
      return StringifyNumber(text);
    }
    case kString: {
      return StringifyString(text);
    }
    case kArray: {
      return StringifyArray(text);
    }
    case kObject: {
      return StringifyObject(text);
    }
    default: {
      return kInvalidValue;
    }
  }
}

/**
 * The functions below are private.
 */

ReturnValue Value::StringifyLiteral(
  const char *literal,
  std::string &text) const {
  text += literal;
  return kOk;
}

ReturnValue Value::StringifyNumber(std::string &text) const {
  char buffer[32];
  sprintf(buffer, "%.17g", _value._number);
  text += buffer;
  return kOk;
}

ReturnValue Value::StringifyString(std::string &text) const {
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
  return kOk;
}

ReturnValue Value::StringifyArray(std::string &text) const {
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
  return kOk;
}

ReturnValue Value::StringifyObject(std::string &text) const {
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
  return kOk;
}
}
