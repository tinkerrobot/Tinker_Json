/*
 * Project: Noob_Json_Parser
 * File: noob-value.h
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#ifndef NOOB_JSON_PARSER_NOOB_VALUE_H
#define NOOB_JSON_PARSER_NOOB_VALUE_H

#include "header/noob-constant.h"

#include <cstdio>
#include <string>
#include <unordered_map>
#include <vector>

class NoobValue {
 public:
  NoobValue();
  NoobValue(const char *json);
  ~NoobValue();

    // Type wrapper
  NoobType GetType() const;
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
  NoobValue& GetElement(size_t index) const;
  size_t GetArraySize() const;
  void SetArray(std::vector<NoobValue *> &vec);
    // Object member wrapper
  NoobValue& GetValue(const std::string &key) const;
  bool HasKey(const std::string &key) const;
  size_t GetObjectSize() const;
  void SetObject(std::unordered_map<std::string, NoobValue *> &obj);
    // Operator overloading
  NoobValue& operator[] (size_t index) const;
  NoobValue& operator[] (const std::string &key) const;

    // Parse json texts
  NoobReturnValue Parse(const char *json);

    // Stringify json values
  NoobReturnValue Stringify(std::string &text) const;

 private:
  void Free();

  // JSON text parser
  void ParseWhitespace();
  NoobReturnValue ParseValue();
  NoobReturnValue ParseLiteral(const char *literal, NoobType type);
  NoobReturnValue ParseNumber();
  NoobReturnValue ParseStringRaw(std::string *str);
  NoobReturnValue ParseString();
  NoobReturnValue ParseArray();
  NoobReturnValue ParseObject();

  // JSON value stringifier
  NoobReturnValue StringifyLiteral(const char *literal, std::string &text) const;
  NoobReturnValue StringifyNumber(std::string &text) const;
  NoobReturnValue StringifyString(std::string &text) const;
  NoobReturnValue StringifyArray(std::string &text) const;
  NoobReturnValue StringifyObject(std::string &text) const;

  // Data members
  NoobType _type;
  const char *_json;
  union {
    std::unordered_map<std::string, NoobValue *> *_object;
    std::vector<NoobValue *> *_array;
    std::string *_string;
    double _number;
  } _value;
};

#endif //NOOB_JSON_PARSER_NOOB_VALUE_H
