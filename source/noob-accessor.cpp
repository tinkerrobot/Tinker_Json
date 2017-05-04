/*
 * Project: Noob_Json_Parser
 * File: noob-accessor.cpp
 * Author: Ling.Li
 * Date: 2017/5/4
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
 * Tool functions
 */

/*
 * Since we do not suggest using exceptions in C++,
 * if the users try to get the wrong type of value,
 * the getter functions will call NoobError()
 * and exit the program directly.
 * The reason I don't exit the program in NoobError()
 * is that it will generate many warnings.
 */
void NoobError(const char *error_msg) {
  fprintf(stderr, "> ERROR: %s\n", error_msg);
}

/**
 * Type wrapper
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
    NoobError("Try to access the boolean value of a non-boolean object!");
    exit(31);
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
    NoobError("Try to access the numeric value of a non-number object!");
    exit(31);
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
    NoobError("Try to access the string value of a non-string object!");
    exit(31);
  }
}

size_t NoobValue::GetLength() const {
  if(_type == kNoobString) {
    return (_value._string)->length();
  } else {
    NoobError("Try to access the string length of a non-string object!");
    exit(31);
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
    NoobError("Try to access the element of a non-array object!");
    exit(31);
  }
}

size_t NoobValue::GetArraySize() const {
  if(_type == kNoobArray) {
    return (_value._array)->size();
  } else {
    NoobError("Try to access the size of a non-object object!");
    exit(31);
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
    NoobError("Try to access the element of a non-object object!");
    exit(31);
  }
}

bool NoobValue::HasKey(const std::string &key) const {
  if(_type == kNoobObject) {
    return ((_value._object)->count(key) > 0);
  } else {
    NoobError("Try to access the key of a non-object object!");
    exit(31);
  }
}

size_t NoobValue::GetObjectSize() const {
  if(_type == kNoobObject) {
    return (_value._object)->size();
  } else {
    NoobError("Try to access the element of a non-object object!");
    exit(31);
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
    NoobError("Try to access the element of a non-array object!");
    exit(31);
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
    NoobError("Try to access the element of a non-object object!");
    exit(31);
  }
}