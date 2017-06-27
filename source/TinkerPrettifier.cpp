/*
 * Project: Tinker_Json_Parser
 * File: TinkerPrettifier.cpp
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

void Indent(std::string &text, int indent) {
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
ReturnValue Value::Prettify(
  std::string &text, int indent)  const {
  std::string json;
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
      return PrettifyArray(text, indent);
    }
    case kObject: {
      return PrettifyObject(text, indent);
    }
    default: {
      return kInvalidValue;
    }
  }
}

/**
 * The functions below are private.
 */

ReturnValue Value::PrettifyArray(std::string &text, int indent) const {
  text.push_back('[');
  if((_value._array)->size() > 0) {
    text.push_back('\n');
    size_t size = (_value._array)->size();
    for(size_t i = 0; i < size; ++i) {
      Indent(text, indent + 1);
      (_value._array)->at(i)->Prettify(text, indent + 1);
      text += ",\n";
    }
    text.pop_back();
    text.pop_back();
    text.push_back('\n');
    Indent(text, indent);
  }
  text.push_back(']');
  return kOk;
}

ReturnValue Value::PrettifyObject(std::string &text, int indent) const {
  text.push_back('{');
  if((_value._object)->size() > 0) {
    text.push_back('\n');
    for(auto it = (_value._object)->begin();
      it != (_value._object)->end();
      ++it) {
      Indent(text, indent + 1);
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
    Indent(text, indent);
  }
  text.push_back('}');
  return kOk;
}
}
