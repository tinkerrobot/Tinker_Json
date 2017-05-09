/*
 * Project: Noob_Json_Parser
 * File: noob-value.cpp
 * Author: Ling.Li
 * Date: 2017/4/25
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

