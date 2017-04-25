/*
 * Project: Noob_Json_Parser
 * File: noob-context.h
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#ifndef NOOB_JSON_PARSER_NOOB_CONTEXT_H
#define NOOB_JSON_PARSER_NOOB_CONTEXT_H

#include <cstdio>

class NoobContext {
 public:
  NoobContext();
  NoobContext(const char *json);
  ~NoobContext();
  const char* NoobGetJson();
  void NoobSetJson(const char *json);
  //size_t NoobGetTop();
  //void set_top(size_t top);
  void NoobMoveForward(int step);
  //void* NoobPush(size_t size);
  //void* NoobPop(size_t size);


 private:
  const char *_json;
  char *_stack;
  size_t _size, _top;
};

#endif //NOOB_JSON_PARSER_NOOB_CONTEXT_H
