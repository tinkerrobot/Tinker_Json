/*
 * Project: Noob_Json_Parser
 * File: noob-accessor.cpp
 * Author: Ling.Li
 * Date: 2017/4/26
 */

#include "../header/noob-constant.h"
#include "../header/noob-value.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include <unordered_map>
#include <vector>

/*
 * Public accessors
 */

void NoobCrash(const char *error_msg) {
  fprintf(stderr, "> ERROR: %s\n", error_msg);
  exit(31);
}

NoobType NoobValue::type() const {
  return _type;
}

bool NoobValue::boolean() const {
  if(_type == kNoobTrue) return true;
  else if(_type == kNoobFalse) return false;
  else NoobCrash("Try to access the boolean value of a non-boolean object!");
}

double NoobValue::number() const {
  if(_type == kNoobNumber) return _value._number;
  else NoobCrash("Try to access the numeric value of a non-number object!");
}

const std::string& NoobValue::string() const {
  if(_type == kNoobString) return *(_value._string);
  else NoobCrash("Try to access the string value of a non-string object!");
}

size_t NoobValue::length() const {
  if(_type == kNoobString) return (_value._string)->length();
  else NoobCrash("Try to access the string length of a non-string object!");
}

const NoobValue& NoobValue::at(size_t index) const {
  if(_type == kNoobArray) return *((_value._array)->at(index));
  else NoobCrash("Try to access the element of a non-array object!");
}

const NoobValue& NoobValue::operator[](size_t index) const {
  if(_type == kNoobArray) return *((_value._array)->at(index));
  else NoobCrash("Try to access the element of a non-array object!");
}

size_t NoobValue::array_size() const {
  if(_type == kNoobArray) return (_value._array)->size();
  else NoobCrash("Try to access the size of a non-object object!");
}

bool NoobValue::has_key(const std::string &key) const {
  if(_type == kNoobObject) return ((_value._object)->count(key) > 0);
  else NoobCrash("Try to access the key of a non-object object!");
}

const NoobValue& NoobValue::at(const std::string &key) const {
  if(_type == kNoobObject) return *((_value._object)->at(key));
  else NoobCrash("Try to access the element of a non-object object!");
}

const NoobValue& NoobValue::operator[](const std::string &key) const {
  if(_type == kNoobObject) return *((_value._object)->at(key));
  else NoobCrash("Try to access the element of a non-object object!");
}

size_t NoobValue::object_size() const {
  if(_type == kNoobObject) return (_value._object)->size();
  else NoobCrash("Try to access the element of a non-object object!");
}