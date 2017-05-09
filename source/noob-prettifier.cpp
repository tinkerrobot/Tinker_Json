/*
 * Project: Noob_Json_Parser
 * File: noob-prettifier.cpp
 * Author: Ling.Li
 * Date: 2017/5/4
 */

#include "source/noob-constant.h"
#include "source/noob-value.h"

#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * Tool functions
 */

void NoobIndent(std::string &text, int indent) {
  for(int i = 0; i < indent; ++i) {
    text += "  ";
  }
}

/*
 * Note that this is the only API exposed to the users.
 * Any other stringifier functions are private
 * and invisible to the outside.
 * Generally, when calling this function,
 * the argument indent should be 0.
 */
NoobReturnValue NoobValue::Prettify(
  std::string &text, int indent)  const {
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
      return PrettifyArray(text, indent);
    }
    case kNoobObject: {
      return PrettifyObject(text, indent);
    }
    default: {
      return kNoobInvalidValue;
    }
  }
}

/**
 * The functions below are private.
 */

NoobReturnValue NoobValue::PrettifyArray(std::string &text, int indent) const {
  text.push_back('[');
  if((_value._array)->size() > 0) {
    text.push_back('\n');
    size_t size = (_value._array)->size();
    for(size_t i = 0; i < size; ++i) {
      NoobIndent(text, indent + 1);
      (_value._array)->at(i)->Prettify(text, indent + 1);
      text += ",\n";
    }
    text.pop_back();
    text.pop_back();
    text.push_back('\n');
    NoobIndent(text, indent);
  }
  text.push_back(']');
  return kNoobOk;
}

NoobReturnValue NoobValue::PrettifyObject(std::string &text, int indent) const {
  text.push_back('{');
  if((_value._object)->size() > 0) {
    text.push_back('\n');
    for(auto it = (_value._object)->begin();
      it != (_value._object)->end();
      ++it) {
      NoobIndent(text, indent + 1);
      text.push_back('"');
      text += it->first;
      text.push_back('"');
      text += ": ";
      it->second->Prettify(text, indent + 1);
      text += ",\n";
    }
    text.pop_back();
    text.pop_back();
    text.push_back('\n');
    NoobIndent(text, indent);
  }
  text.push_back('}');
  return kNoobOk;
}
