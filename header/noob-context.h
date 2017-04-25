/*
 * Project: Noob_Json_Parser
 * File: noob-context.h
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#ifndef NOOB_JSON_PARSER_NOOB_CONTEXT_H
#define NOOB_JSON_PARSER_NOOB_CONTEXT_H

#include "std-header.h"
#include "noob-stack.h"

class NoobContext {
 public:
  NoobContext();
  NoobContext(const char *json);
  ~NoobContext();
  const char* NoobGetJson();
  void NoobSetJson(const char *json);
  void NoobMoveForward(int step);

 private:
  const char *_json;
};

#endif //NOOB_JSON_PARSER_NOOB_CONTEXT_H
