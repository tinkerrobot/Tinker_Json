/*
 * Project: Noob_Json_Parser
 * File: noob-value.h
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#ifndef NOOB_JSON_PARSER_NOOB_VALUE_H
#define NOOB_JSON_PARSER_NOOB_VALUE_H

#include "noob-enum.h"
#include "noob-context.h"

class NoobValue {
 public:
  NoobValue();
  ~NoobValue();
  NoobType NoobGetType();
  void NoobSetType(NoobType type);
  NoobReturnValue NoobParse(const char *json);

 private:
  void NoobParseWhitespace();
  NoobReturnValue NoobParseValue();
  NoobReturnValue NoobParseLiteral(const char* literal, NoobType type);

  NoobType _type;
  NoobContext *_context;
};

#endif //NOOB_JSON_PARSER_NOOB_VALUE_H
