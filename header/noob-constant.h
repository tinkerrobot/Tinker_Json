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

static const char *NoobTypeString[] = {
  "Null",
  "False",
  "True",
  "Number",
  "String",
  "Array",
  "Object"
};

static const char *NoobStatusString[] = {
  "Ok",
  "ExpectValue",
  "InvalidValue",
  "NotSigular",
  "NumberTooBig",
  "MissQuotationMark",
  "InvalidStringEscape",
  "InvalidStringChar",
  "InvalidUnicodeHex",
  "InvalidUnicodeSurrogate",
  "MissCommaOrSquareBracket",
  "MissKey",
  "MissColon",
  "MissCommaOrCurlyBracket"
};

#endif //NOOB_JSON_PARSER_NOOB_CONSTANT_H
