/*
 * Project: Tinker_Json_Parser
 * File: TinkerAccessor.cpp
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

/*
 * Since we do not suggest using exceptions in C++,
 * if the users try to get the wrong type of value,
 * the getter functions will call Error()
 * and exit the program directly.
 * The reason I don't exit the program in Error()
 * is that it will generate many warnings.
 */
void Error(const char *error_msg) {
  fprintf(stderr, "> ERROR: %s\n", error_msg);
}

/**
 * Type wrapper
 */

Type Value::GetType() const {
  return _type;
}

const char* Value::GetTypeString() const {
  return TypeString[_type];
}

/**
 * Boolean member wrapper
 */

bool Value::GetBoolean() const {
  if(_type == kTrue) {
    return true;
  } else if(_type == kFalse) {
    return false;
  } else {
    Error("Try to access the boolean value of a non-boolean object!");
    exit(31);
  }
}

void Value::SetBoolean(bool boolean) {
  Free();
  _type = (boolean ? kTrue : kFalse);
}

/**
 * Number member wrapper
 */

double Value::GetNumber() const {
  if(_type == kNumber) {
    return _value._number;
  } else {
    Error("Try to access the numeric value of a non-number object!");
    exit(31);
  }
}

void Value::SetNumber(double number) {
  Free();
  _value._number = number;
  _type = kNumber;
}

/**
 * String member wrapper
 */

std::string& Value::GetString() const {
  if(_type == kString) {
    return *(_value._string);
  } else {
    Error("Try to access the string value of a non-string object!");
    exit(31);
  }
}

size_t Value::GetLength() const {
  if(_type == kString) {
    return (_value._string)->length();
  } else {
    Error("Try to access the string length of a non-string object!");
    exit(31);
  }
}

void Value::SetString(std::string &str) {
  Free();
  _value._string = new std::string(str);
  _type = kString;
}

void Value::SetString(const char *str, size_t length) {
  Free();
  _value._string = new std::string(str, length);
  _type = kString;
}

/**
 * Array member wrapper
 */

Value & Value::GetElement(size_t index) const {
  if(_type == kArray) {
    return *((_value._array)->at(index));
  } else {
    Error("Try to access the element of a non-array object!");
    exit(31);
  }
}

size_t Value::GetArraySize() const {
  if(_type == kArray) {
    return (_value._array)->size();
  } else {
    Error("Try to access the size of a non-object object!");
    exit(31);
  }
}

void Value::SetArray(std::vector<Value *> &vec) {
  Free();
  _value._array = new std::vector<Value *>(vec);
  _type = kArray;
}

/**
 * Object member wrapper
 */

Value& Value::GetValue(const std::string &key) const {
  if(_type == kObject) {
    return *((_value._object)->at(key));
  } else {
    Error("Try to access the element of a non-object object!");
    exit(31);
  }
}

bool Value::HasKey(const std::string &key) const {
  if(_type == kObject) {
    return ((_value._object)->count(key) > 0);
  } else {
    Error("Try to access the key of a non-object object!");
    exit(31);
  }
}

size_t Value::GetObjectSize() const {
  if(_type == kObject) {
    return (_value._object)->size();
  } else {
    Error("Try to access the element of a non-object object!");
    exit(31);
  }
}

void Value::SetObject(std::unordered_map<std::string, Value *> &obj) {
  Free();
  _value._object = new std::unordered_map<std::string, Value *>(obj);
  _type = kObject;
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
Value& Value::operator[] (size_t index) const {
  if(_type == kArray) {
    return *((_value._array)->at(index));
  } else {
    Error("Try to access the element of a non-array object!");
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
Value& Value::operator[] (const std::string &key) const {
  if(_type == kObject) {
    return *((_value._object)->at(key));
  } else {
    Error("Try to access the element of a non-object object!");
    exit(31);
  }
}
}
