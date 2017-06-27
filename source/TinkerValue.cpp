/*
 * Project: Tinker_Json_Parser
 * File: TinkerValue.cpp
 * Author: Ling.Li
 * Date: 2017/4/25
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
 * Constructors & Destructors
 */

Value::Value() {
  _type = kNull;
  _json = nullptr;
  _value._string = nullptr;
}

Value::Value(const char *json) {
  _type = kNull;
  _json = nullptr;
  _value._string = nullptr;
  Parse(json);
}

Value::~Value() {
  Free();
}

/**
 * Private functions
 */

void Value::Free() {
  if(_type == kString) {
    delete _value._string;
    _value._string = nullptr;
  } else if(_type == kArray) {
    size_t array_size = (_value._array)->size();
    for(size_t i = 0; i < array_size; ++i) {
      delete (_value._array)->at(i);
    }
    delete _value._array;
    _value._array = nullptr;
  } else if(_type == kObject) {
    for(auto it = (_value._object)->begin();
      it != (_value._object)->end();
      ++it) {
      delete it->second;
    }
    delete _value._object;
    _value._object = nullptr;
  }
  _type = kNull;
}
}