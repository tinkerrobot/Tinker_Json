#include "header/std-header.h"
#include "header/noob-enum.h"
#include "header/noob-value.h"

/*
 * Global Variables, store the results of the tests.
 */
int gCurrentTest = 0;
int gResult = 0;
int gTestTotal = 0;
int gTestPass = 0;

/*
 * Test functions.
 */
template <typename T>
bool TestEqualBase(bool equality, T expect, T actual) {
  gCurrentTest++;
  gTestTotal++;
  if(equality) {
    gTestPass++;
  } else {
    std::cerr << "LINE: " << gCurrentTest
              << " >> Expect: " << expect
              << " Actual: " << actual
              << std::endl;
    gResult++;
  }
  return equality;
}

bool TestEqualInt(int expect, int actual) {
  return TestEqualBase<int>((expect == actual), expect, actual);
}

/*
 * Test cases.
 */

void TestLiteral() {
  NoobValue v;
  gCurrentTest = __LINE__;
  TestEqualInt(kNoobOk, v.NoobParse("null"));
  TestEqualInt(kNoobNull, v.NoobGetType());
  TestEqualInt(kNoobOk, v.NoobParse("true"));
  TestEqualInt(kNoobTrue, v.NoobGetType());
  TestEqualInt(kNoobOk, v.NoobParse("false"));
  TestEqualInt(kNoobFalse, v.NoobGetType());
}

void TestIllegal() {
  NoobValue v;

  /*
   * Expect Value
   */
  v.NoobSetType(kNoobFalse);
  gCurrentTest = __LINE__;
  TestEqualInt(kNoobExpectValue, v.NoobParse(""));
  TestEqualInt(kNoobNull, v.NoobGetType());
  v.NoobSetType(kNoobFalse);
  gCurrentTest = __LINE__;
  TestEqualInt(kNoobExpectValue, v.NoobParse(" "));
  TestEqualInt(kNoobNull, v.NoobGetType());

  /*
   * Invalid Value
   */
  v.NoobSetType(kNoobFalse);
  gCurrentTest = __LINE__;
  TestEqualInt(kNoobInvalidValue, v.NoobParse("nul"));
  TestEqualInt(kNoobNull, v.NoobGetType());
  v.NoobSetType(kNoobFalse);
  gCurrentTest = __LINE__;
  TestEqualInt(kNoobInvalidValue, v.NoobParse("?"));
  TestEqualInt(kNoobNull, v.NoobGetType());

  /*
   * Root not sigular
   */
  v.NoobSetType(kNoobFalse);
  gCurrentTest = __LINE__;
  TestEqualInt(kNoobNotSigular, v.NoobParse("null x"));
  TestEqualInt(kNoobNull, v.NoobGetType());
}

void MainTest() {
  TestLiteral();
  TestIllegal();
}

int main() {
  MainTest();
  printf("%d/%d (%3.2f%%) Passed\n", gTestPass, gTestTotal, gTestPass * 100.0 / gTestTotal);
  return gResult;
}