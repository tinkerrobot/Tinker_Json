#include "header/std-header.h"
#include "header/noob-enum.h"
#include "header/noob-value.h"

/*
 * Global Variables, store the results of the tests.
 */
std::string gCurrentTest = "";
int gResult = 0;
int gTestTotal = 0;
int gTestPass = 0;

/*
 * Test functions.
 */
#define TestEqualBase(equality, expect, actual)\
  do {\
    gTestTotal++;\
    if (equality)\
      gTestPass++;\
    else {\
      std::cout << __FILE__ << ":" << __LINE__ << " > Expect: " << expect << " Actual: " << actual << std::endl;\
      gResult++;\
    }\
  } while(0)


#define TestEqualInt(expect, actual)\
  do {\
    TestEqualBase((expect) == (actual), expect, actual);\
  } while(0)

#define TestEqualDouble(expect, actual)\
  do {\
    TestEqualBase((expect) == (actual), expect, actual);\
  } while(0)

#define TestEqualString(expect, actual, length) \
  do {\
    TestEqualBase(\
      sizeof(expect) - 1 == length && memcmp(expect, actual, length) == 0,\
      expect, actual);\
  } while(0)

#define TestNumber(expect, json)\
  do {\
    NoobValue v;\
    TestEqualInt(kNoobOk, v.NoobParse(json));\
    TestEqualInt(kNoobNumber, v.NoobGetType());\
    TestEqualDouble(expect, v.NoobGetNumber());\
  } while(0)

#define TestError(error, json)\
  do {\
    NoobValue v;\
    v.NoobSetType(kNoobFalse);\
    TestEqualInt(error, v.NoobParse(json));\
    TestEqualInt(kNoobNull, v.NoobGetType());\
  } while(0)

#define TestString(expect, json)\
  do {\
    NoobValue v;\
    TestEqualInt(kNoobOk, v.NoobParse(json));\
    TestEqualInt(kNoobString, v.NoobGetType());\
    TestEqualString(expect, v.NoobGetString()->c_str(), v.NoobGetStringLength());\
  } while(0)

#define TestTrue(actual) TestEqualBase((actual) != 0, "true", "false")
#define TestFalse(actual) TestEqualBase((actual) == 0, "false", "true")

/*
 * Test cases.
 */

static void TestParseLiteral() {
  NoobValue v;
  TestEqualInt(kNoobOk, v.NoobParse("null"));
  TestEqualInt(kNoobNull, v.NoobGetType());
  TestEqualInt(kNoobOk, v.NoobParse("true"));
  TestEqualInt(kNoobTrue, v.NoobGetType());
  TestEqualInt(kNoobOk, v.NoobParse("false"));
  TestEqualInt(kNoobFalse, v.NoobGetType());
}

static void TestParseNumber() {
  TestNumber(0.0, "0");
  TestNumber(0.0, "-0");
  TestNumber(0.0, "-0.0");
  TestNumber(1.0, "1");
  TestNumber(-1.0, "-1");
  TestNumber(1.5, "1.5");
  TestNumber(-1.5, "-1.5");
  TestNumber(3.1416, "3.1416");
  TestNumber(1E10, "1E10");
  TestNumber(1e10, "1e10");
  TestNumber(1E+10, "1E+10");
  TestNumber(1E-10, "1E-10");
  TestNumber(-1E10, "-1E10");
  TestNumber(-1e10, "-1e10");
  TestNumber(-1E+10, "-1E+10");
  TestNumber(-1E-10, "-1E-10");
  TestNumber(1.234E+10, "1.234E+10");
  TestNumber(1.234E-10, "1.234E-10");
  TestNumber(0.0, "1e-10000"); /* must underflow */

  TestNumber(1.0000000000000002, "1.0000000000000002"); /* the smallest number > 1 */
  TestNumber( 4.9406564584124654e-324, "4.9406564584124654e-324"); /* minimum denormal */
  TestNumber(-4.9406564584124654e-324, "-4.9406564584124654e-324");
  TestNumber( 2.2250738585072009e-308, "2.2250738585072009e-308");  /* Max subnormal double */
  TestNumber(-2.2250738585072009e-308, "-2.2250738585072009e-308");
  TestNumber( 2.2250738585072014e-308, "2.2250738585072014e-308");  /* Min normal positive double */
  TestNumber(-2.2250738585072014e-308, "-2.2250738585072014e-308");
  TestNumber( 1.7976931348623157e+308, "1.7976931348623157e+308");  /* Max double */
  TestNumber(-1.7976931348623157e+308, "-1.7976931348623157e+308");
};

static void TestParseString() {
  TestString("", "\"\"");
  TestString("Hello", "\"Hello\"");
  TestString("Hello\nWorld", "\"Hello\\nWorld\"");
  TestString("\" \\ / \b \f \n \r \t", "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"");
}

static void TestParseArray() {
  NoobValue v;
  TestEqualInt(kNoobOk, v.NoobParse("[ ]"));
  TestEqualInt(kNoobArray, v.NoobGetType());
  TestEqualInt(0, v.NoobGetArraySize());

  TestEqualInt(kNoobOk, v.NoobParse("[ null , false , true , 123 , \"abc\" ]"));
  TestEqualInt(kNoobArray, v.NoobGetType());
  TestEqualInt(5, v.NoobGetArraySize());
  TestEqualInt(kNoobNull,   v[0]->NoobGetType());
  TestEqualInt(kNoobFalse,  v[1]->NoobGetType());
  TestEqualInt(kNoobTrue,   v[2]->NoobGetType());
  TestEqualInt(kNoobNumber, v[3]->NoobGetType());
  TestEqualInt(kNoobString, v[4]->NoobGetType());
  TestEqualDouble(123.0, v[3]->NoobGetNumber());
  TestEqualString("abc",
    v[4]->NoobGetString()->c_str(),
    v[4]->NoobGetStringLength());

  TestEqualInt(kNoobOk, v.NoobParse("[ [ ] , [ 0 ] , [ 0 , 1 ] , [ 0 , 1 , 2 ] ]"));
  TestEqualInt(kNoobArray, v.NoobGetType());
  TestEqualInt(4, v.NoobGetArraySize());
  for (int i = 0; i < 4; i++) {
    NoobValue* a = v[i];
    TestEqualInt(kNoobArray, a->NoobGetType());
    TestEqualInt(i, a->NoobGetArraySize());
    for (int j = 0; j < i; j++) {
      NoobValue *e = (*a)[j];
      TestEqualInt(kNoobNumber, e->NoobGetType());
      TestEqualDouble((double)j, e->NoobGetNumber());
    }
  }
}

static void TestParseIllegalLiteral() {
  TestError(kNoobExpectValue, "");
  TestError(kNoobExpectValue, " ");

  TestError(kNoobInvalidValue, "nul");
  TestError(kNoobInvalidValue, "?");

  TestError(kNoobNotSigular, "null x");
}

static void TestParseIllegalNumber() {
  TestError(kNoobInvalidValue, "+0");
  TestError(kNoobInvalidValue, "+1");
  TestError(kNoobInvalidValue, ".123"); /* at least one digit before '.' */
  TestError(kNoobInvalidValue, "1.");   /* at least one digit after '.' */
  TestError(kNoobInvalidValue, "INF");
  TestError(kNoobInvalidValue, "inf");
  TestError(kNoobInvalidValue, "NAN");
  TestError(kNoobInvalidValue, "nan");

  TestError(kNoobNumberTooBig, "1e309");
  TestError(kNoobNumberTooBig, "-1e309");
  TestError(kNoobNotSigular, "0123"); /* after zero should be '.' or nothing */
  TestError(kNoobNotSigular, "0x0");
  TestError(kNoobNotSigular, "0x123");
}

static void TestParseIllegalString() {
  TestError(kNoobMissQuotationMark, "\"");
  TestError(kNoobMissQuotationMark, "\"abc");
  TestError(kNoobInvalidStringEscape, "\"\\v\"");
  TestError(kNoobInvalidStringEscape, "\"\\'\"");
  TestError(kNoobInvalidStringEscape, "\"\\0\"");
  TestError(kNoobInvalidStringEscape, "\"\\x12\"");
  TestError(kNoobInvalidStringChar, "\"\x01\"");
  TestError(kNoobInvalidStringChar, "\"\x1F\"");
}

static void TestParseIllegalArray() {
  TestError(kNoobMissCommaOrSquareBracket, "[1");
  TestError(kNoobMissCommaOrSquareBracket, "[1}");
  TestError(kNoobMissCommaOrSquareBracket, "[1 2");
  TestError(kNoobMissCommaOrSquareBracket, "[[]");
}

void MainTest() {
  TestParseLiteral();
  TestParseNumber();
  TestParseString();
  TestParseArray();
  TestParseIllegalLiteral();
  TestParseIllegalNumber();
  TestParseIllegalString();
  TestParseIllegalArray();
}

int main() {
  MainTest();
  printf("%d/%d (%3.2f%%) Passed\n",
    gTestPass,
    gTestTotal,
    gTestPass * 100.0 / gTestTotal
  );
  return gResult;
}