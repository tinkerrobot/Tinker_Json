/*
 * Project: Noob_Json_Parser
 * File: noob-value.h
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#ifndef NOOB_JSON_PARSER_NOOB_VALUE_H
#define NOOB_JSON_PARSER_NOOB_VALUE_H

#include "std-header.h"
#include "noob-enum.h"
#include "noob-context.h"

class NoobValue {
 public:
  NoobValue();
  ~NoobValue();

  NoobType NoobGetType() const;
  void NoobSetType(NoobType type);
  bool NoobGetBoolean() const;
  double NoobGetNumber() const;
  const std::string* NoobGetString() const;
  size_t NoobGetStringLength() const;
  NoobValue* NoobGetArrayElement(size_t index) const;
  NoobValue* operator[](size_t index) const;
  size_t NoobGetArraySize() const;

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
  NoobReturnValue NoobParseString();
  NoobReturnValue NoobParseArray();

  union {
    std::vector<NoobValue *> *_array;
    std::string *_string;
    double _number;
  } _value;
  NoobType _type;
  NoobContext *_context;
};

#endif //NOOB_JSON_PARSER_NOOB_VALUE_H
