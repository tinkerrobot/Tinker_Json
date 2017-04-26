/*
 * Project: Noob_Json_Parser
 * File: noob-value.h
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#ifndef NOOB_JSON_PARSER_NOOB_VALUE_H
#define NOOB_JSON_PARSER_NOOB_VALUE_H

#include "noob-constant.h"

#include <cstdio>
#include <unordered_map>
#include <vector>

class NoobValue {
 public:
  NoobValue();
  ~NoobValue();

  const char* type() const;
  NoobType type_val() const;
  bool boolean() const;
  double number() const;
  const std::string& string() const;
  size_t length() const;
  const NoobValue& at(size_t index) const;
  const NoobValue& operator[](size_t index) const;
  size_t array_size() const;
  bool has_key(const std::string &key) const;
  const NoobValue& at(const std::string &key) const;
  const NoobValue& operator[](const std::string &key) const;
  size_t object_size() const;

  NoobType NoobGetType() const;
  void NoobSetType(NoobType type);
  bool NoobGetBoolean() const;
  double NoobGetNumber() const;
  const std::string& NoobGetString() const;
  size_t NoobGetStringLength() const;
  const NoobValue& NoobGetArrayElement(size_t index) const;
  size_t NoobGetArraySize() const;
  size_t NoobGetObjectSize() const;
  bool NoobHasKey(const std::string &key) const;
  const NoobValue& NoobGetObjectValue(const std::string &key) const;

  NoobReturnValue NoobParse(const char *json);

 private:
  void NoobFree();

  void NoobSetBoolean(bool boolean);
  void NoobSetNumber(double number);
  void NoobSetString(std::string *str);
  void NoobSetString(std::string &str);
  void NoobSetString(const char *str, size_t length);
  // void NoobSetArray(std::vector<NoobValue *> *vec);
  // void NoobSetArray(std::vector<NoobValue *> &vec);

  void NoobParseWhitespace();
  NoobReturnValue NoobParseValue();
  NoobReturnValue NoobParseLiteral(const char* literal, NoobType type);
  NoobReturnValue NoobParseNumber();
  const char* NoobParseHex4(const char *pointer, unsigned *u);
  void NoobEncodeUtf8(std::string *str, unsigned u);
  NoobReturnValue NoobParseStringRaw(std::string *str);
  NoobReturnValue NoobParseString();
  NoobReturnValue NoobParseArray();
  NoobReturnValue NoobParseObject();

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
