/*
 * Project: Tinker_Json_Parser
 * File: TinkerValue.h
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#ifndef TINKER_JSON_PARSER_TINKER_VALUE_H
#define TINKER_JSON_PARSER_TINKER_VALUE_H

#include "TinkerConstant.h"

#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

namespace Tinker {
class Value {
 public:
  Value();
  Value(const char *json);
  ~Value();

  // Type wrapper
  Type GetType() const;
  const char* GetTypeString() const;
  // Boolean member wrapper
  bool GetBoolean() const;
  void SetBoolean(bool boolean);
  // Numeric member wrapper
  double GetNumber() const;
  void SetNumber(double number);
  // String member wrapper
  std::string& GetString() const;
  size_t GetLength() const;
  void SetString(std::string &str);
  void SetString(const char *str, size_t length);
  // Array member wrapper
  Value& GetElement(size_t index) const;
  size_t GetArraySize() const;
  void SetArray(std::vector<Value *> &vec);
  // Object member wrapper
  Value& GetValue(const std::string &key) const;
  bool HasKey(const std::string &key) const;
  size_t GetObjectSize() const;
  void SetObject(std::unordered_map<std::string, Value *> &obj);
  // Operator overloading
  Value& operator[] (size_t index) const;
  Value& operator[] (const std::string &key) const;

  // Parse json texts
  ReturnValue Parse(const char *json);

  // Stringify json values
  ReturnValue Stringify(std::string &text) const;

  // Prettify generated JSON string
  ReturnValue Prettify(std::string &text, int indent = 0) const;

 private:
  void Free();

  // JSON text parser
  void ParseWhitespace();
  ReturnValue ParseValue();
  ReturnValue ParseLiteral(const char *literal, Type type);
  ReturnValue ParseNumber();
  ReturnValue ParseRawString(std::string *str);
  ReturnValue ParseString();
  ReturnValue ParseArray();
  ReturnValue ParseObject();

  // JSON value stringifier
  ReturnValue StringifyLiteral(const char *literal, std::string &text) const;
  ReturnValue StringifyNumber(std::string &text) const;
  ReturnValue StringifyString(std::string &text) const;
  ReturnValue StringifyArray(std::string &text) const;
  ReturnValue StringifyObject(std::string &text) const;

  // JSON value stringifier
  ReturnValue PrettifyArray(std::string &text, int indent) const;
  ReturnValue PrettifyObject(std::string &text, int indent) const;

  // Data members
  Type _type;
  const char *_json;
  union {
    std::unordered_map<std::string, Value *> *_object;
    std::vector<Value *> *_array;
    std::string *_string;
    double _number;
  } _value;
};
}

#endif //TINKER_JSON_PARSER_TINKER_VALUE_H
