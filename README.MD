# Noob JSON Parser - C++ Version of Lept JSON Parser

## Version

v 1.0.0

## Build status

| GCC             | Clang           |
| :-------------: | :-------------: |
| ![linux-status] | ![macos-status] |

[linux-status]: https://travis-ci.org/lilinsanity/Noob-Json-Parser.svg?branch=master "Travis build status"
[macos-status]: https://travis-ci.org/lilinsanity/Noob-Json-Parser.svg?branch=master "Travis build status"

## Introduction

The Lept JSON is a simple JSON parser & generator introduced by [@Milo Yip](https://github.com/miloyip) in his tutorial [从零开始的 JSON 库教程](https://zhuanlan.zhihu.com/p/22457315).

The original version is programmed in C, I rewrote the whole program using C++ while reading the tutorial. After I finished the first version, I realized that it was imperfect, mixed C and C++ style and full of bugs.

The **Noob JSON Parser** is the refactor of the first version, aiming to make it more concise and robust. The name "Noob" means that the coder, which is myself, is totally a rookie in coding, not the parser.

Finally I want to express my thanks to [@Milo Yip](https://github.com/miloyip), who writes such a good tutorial from which I learn so much.

## Speed Test

I use three json files which are also used in [NativeJson Benchmark](https://github.com/miloyip/nativejson-benchmark) to test the speed of parsing. The result is the average time of 10 tests.

**twitter.json**: 11.1200 ms

**canada.json**: 98.7580 ms

**citm_catalog.json**: 10.8850 ms

The test files can be found in the `test` folder.

## Usage

Below is a simple example on how to parse a JSON string into a document, and get the value of the nodes.

```c++
#include "header/noob-value.h"

#include <iostream>

int main() {
  const char* json = "{\"project\":\"NoobJson\",\"stars\":1}";
  NoobValue v;
  v.NoobParse(json);

  const NoobValue &p = v["project"];
  std::cout << "Type: " << p.type() << std::endl;
  std::cout << "Value: " << p.string() << std::endl;

  const NoobValue &s = v["stars"];
  std::cout << "Type: " << s.type() << std::endl;
  std::cout << "Value: " << s.number() << std::endl;

  return 0;
}
```

Result:

```
Type: String
Value: NoobJson
Type: Number
Value: 1
```

## Coding Environment

* **Language**: C++
* **System**: Mac OSX Sierra
* **IDE**: JetBrains CLion 2017.02
* **Compiler**: CMake 3.7.1 & Clang 800.0.42.1
