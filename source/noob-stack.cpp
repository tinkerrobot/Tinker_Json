/*
 * Project: Noob_Json_Parser
 * File: noob-stack.cpp
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#include "../header/std-header.h"
#include "../header/noob-stack.h"

NoobStack::NoobStack() {
  _stack = nullptr;
  _size = _top = 0;
}

NoobStack::~NoobStack() {
  delete[] _stack;
}

void NoobStack::NoobPush(char *element, size_t size) {
  if(_top + size >= _size) {

  }
}