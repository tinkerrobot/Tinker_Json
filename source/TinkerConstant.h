/*
 * Project: Tinker_Json_Parser
 * File: TinkerConstant.h
 * Author: Ling.Li
 * Date: 2017/4/25
 */

#ifndef TINKER_JSON_PARSER_TINKER_CONSTANT_H
#define TINKER_JSON_PARSER_TINKER_CONSTANT_H

namespace Tinker {
enum Type {
  kNull,
  kFalse,
  kTrue,
  kNumber,
  kString,
  kArray,
  kObject
};

enum ReturnValue {
  kOk = 0,
  kExpectValue,
  kInvalidValue,
  kNotSingular,
  kNumberTooBig,
  kMissQuotationMark,
  kInvalidStringEscape,
  kInvalidStringChar,
  kInvalidUnicodeHex,
  kInvalidUnicodeSurrogate,
  kMissCommaOrSquareBracket,
  kMissKey,
  kMissColon,
  kMissCommaOrCurlyBracket
};

static const char *TypeString[] = {
  "Null",
  "False",
  "True",
  "Number",
  "String",
  "Array",
  "Object"
};

static const char *StatusString[] = {
  "Ok",
  "ExpectValue",
  "InvalidValue",
  "NotSingular",
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
}

#endif //TINKER_JSON_PARSER_TINKER_CONSTANT_H
