/*
 * Project: Noob_Json_Parser
 * File: noob-value.cpp
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#include "../header/noob-enum.h"
#include "../header/noob-value.h"
/*
 * Tool functions
 */

NoobValue::NoobValue() {
  _type = kNoobNull;
  _context = nullptr;
}

NoobValue::~NoobValue() {
  delete _context;
  _context = nullptr;
}

NoobType NoobValue::NoobGetType() {
  return _type;
}

void NoobValue::NoobSetType(NoobType type) {
  _type = type;
}

NoobReturnValue NoobValue::NoobParse(const char *json) {
  delete _context;
  _context = nullptr;
  _context = new NoobContext(json);
  _type = kNoobNull;
  NoobReturnValue result = kNoobInvalidValue;
  NoobParseWhitespace();
  result = NoobParseValue();
  if(result == kNoobOk) {
    NoobParseWhitespace();
    if(*(_context->NoobGetJson()) != '\0') {
      return kNoobNotSigular;
    }
  }
  return result;
}

/*
 * Private functions
 */

void NoobValue::NoobParseWhitespace() {
  const char *pointer = _context->NoobGetJson();
  while(*pointer == ' ' ||
    *pointer == '\t' ||
    *pointer == '\n' ||
    *pointer == '\r') {
    ++pointer;
  }
  _context->NoobSetJson(pointer);
}

NoobReturnValue NoobValue::NoobParseValue() {
  switch(*(_context->NoobGetJson())) {
    case 'n': {
      return NoobParseLiteral("null", kNoobNull);
    }
    case 't': {
      return NoobParseLiteral("true", kNoobTrue);
    }
    case 'f': {
      return NoobParseLiteral("false", kNoobFalse);
    }
    case '\0': {
      return kNoobExpectValue;
    }
    default: {
      return kNoobInvalidValue;
    }
  }
}

NoobReturnValue NoobValue::NoobParseLiteral(const char* literal, NoobType type) {
  _context->NoobMoveForward(1);
  const char *pointer = _context->NoobGetJson();
  for (size_t i = 0; literal[i + 1]; ++i)
    if (*(pointer++) != literal[i + 1])
      return kNoobInvalidValue;
  _context->NoobSetJson(pointer);
  _type = type;
  return kNoobOk;
}