/*
 * Project: Noob_Json_Parser
 * File: noob-accessor.cpp
 * Author: Ling.Li
 * Date: 2017/4/26
 */

#include "../header/std-header.h"
#include "../header/noob-enum.h"
#include "../header/noob-value.h"

/*
 * Public accessors
 */

NoobType NoobValue::type() const {
  return _type;
}

bool NoobValue::boolean() const {
  if(_type == kNoobTrue) {
    return true;
  } else if(_type == kNoobFalse) {
    return false;
  } else {
    printf("ERROR: Try to access the boolean value of a non-boolean object!\n");
    exit(-1);
  }
}

double NoobValue::number() const {
  if(_type == kNoobNumber) {
    return _value._number;
  } else {
    printf("ERROR: Try to access the numeric value of a non-number object!\n");
    exit(-1);
  }
}

const std::string& NoobValue::string() const {
  if(_type == kNoobString) {
    return *(_value._string);
  } else {
    printf("ERROR: Try to access the string value of a non-string object!\n");
    exit(-1);
  }
}

size_t NoobValue::length() const {
  if(_type == kNoobString) {
    return (_value._string)->length();
  } else {
    printf("ERROR: Try to access the string length of a non-string object!\n");
    exit(-1);
  }
}

const NoobValue& NoobValue::at(size_t index) const {
  if(_type == kNoobArray) {
    return *((_value._array)->at(index));
  } else {
    printf("ERROR: Try to access the element of a non-array object!\n");
    exit(-1);
  }
}


const NoobValue& NoobValue::operator[](size_t index) const {
  if(_type == kNoobArray) {
    return *((_value._array)->at(index));
  } else {
    printf("ERROR: Try to access the element of a non-array object!\n");
    exit(-1);
  }
}

size_t NoobValue::array_size() const {
  if(_type == kNoobObject) {
    return (_value._object)->size();
  } else {
    printf("ERROR: Try to access the size of a non-object object!\n");
    exit(-1);
  }
}

bool NoobValue::has_key(const std::string &key) const {
  if(_type == kNoobObject) {
    return ((_value._object)->count(key) > 0);
  } else {
    printf("ERROR: Try to access the key of a non-object object!\n");
    exit(-1);
  }
}

const NoobValue& NoobValue::at(const std::string &key) const {
  if(_type == kNoobObject) {
    return *((_value._object)->at(key));
  } else {
    printf("ERROR: Try to access the element of a non-object object!\n");
    exit(-1);
  }
}

const NoobValue& NoobValue::operator[](const std::string &key) const {
  if(_type == kNoobObject) {
    return *((_value._object)->at(key));
  } else {
    printf("ERROR: Try to access the element of a non-object object!\n");
    exit(-1);
  }
}