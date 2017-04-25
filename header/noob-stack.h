/*
 * Project: Noob_Json_Parser
 * File: noob-stack.h
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#ifndef NOOB_JSON_PARSER_NOOB_STACK_H
#define NOOB_JSON_PARSER_NOOB_STACK_H

#include "std-header.h"

class NoobStack {
 public:
  NoobStack();
  ~NoobStack();
  void NoobPush(char *element, size_t size);
  char* NoobPop(size_t size);


 private:
  char *_stack;
  size_t _size;
  size_t _top;
};

#endif //NOOB_JSON_PARSER_NOOB_STACK_H
