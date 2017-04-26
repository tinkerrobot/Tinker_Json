/*
 * Project: Noob_Json_Parser
 * File: noob-constant.h
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#ifndef NOOB_JSON_PARSER_NOOB_CONSTANT_H
#define NOOB_JSON_PARSER_NOOB_CONSTANT_H

enum NoobType {
  kNoobNull,
  kNoobFalse,
  kNoobTrue,
  kNoobNumber,
  kNoobString,
  kNoobArray,
  kNoobObject
};

enum NoobReturnValue {
  kNoobOk = 0,
  kNoobExpectValue,
  kNoobInvalidValue,
  kNoobNotSigular,
  kNoobNumberTooBig,
  kNoobMissQuotationMark,
  kNoobInvalidStringEscape,
  kNoobInvalidStringChar,
  kNoobInvalidUnicodeHex,
  kNoobInvalidUnicodeSurrogate,
  kNoobMissCommaOrSquareBracket,
  kNoobMissKey,
  kNoobMissColon,
  kNoobMissCommaOrCurlyBracket
};

#endif //NOOB_JSON_PARSER_NOOB_CONSTANT_H
