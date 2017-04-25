/*
 * Project: Noob_Json_Parser
 * File: noob-context.cpp
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#include "../header/std-header.h"
#include "../header/noob-enum.h"
#include "../header/noob-value.h"
#include "../header/noob-stack.h"
#include "../header/noob-context.h"

NoobContext::NoobContext() {
  _json = nullptr;
}

NoobContext::NoobContext(const char *json) {
  _json = json;
}

NoobContext::~NoobContext() {

}

const char* NoobContext::NoobGetJson() {
  return _json;
}

void NoobContext::NoobSetJson(const char *json) {
  _json = json;
}

void NoobContext::NoobMoveForward(int step) {
  _json += step;
}