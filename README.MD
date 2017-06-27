# Tinker JSON Parser - C++ Version of Lept JSON Parser

## Version

v 3.0.0

## Build status

| GCC             | Clang           |
| :-------------: | :-------------: |
| ![linux-status] | ![macos-status] |

[linux-status]: https://travis-ci.org/lilinsanity/Noob_Json.svg?branch=master "Travis build status"
[macos-status]: https://travis-ci.org/lilinsanity/Noob_Json.svg?branch=master "Travis build status"

## Introduction

The Lept JSON is a simple JSON parser & generator introduced by [@Milo Yip](https://github.com/miloyip) in his tutorial [从零开始的 JSON 库教程](https://zhuanlan.zhihu.com/p/22457315).

The original version is programmed in C, I rewrote the whole program using C++ while reading the tutorial. After I finished the first version, I realized that it was imperfect, mixed C and C++ style and full of bugs.

The **Tinker JSON Parser&Generator** is the refactor of the first version, aiming to make it more concise and robust. The name "Tinker" is a nickname I like :)

Finally I want to express my thanks to [@Milo Yip](https://github.com/miloyip), who writes such a good tutorial from which I learn so much.

## Speed Test

I use three json files which are also used in [NativeJson Benchmark](https://github.com/miloyip/nativejson-benchmark) to test the speed of parsing. The result is the average time of 10 tests.

**twitter.json**: 11.1200 ms

**canada.json**: 98.7580 ms

**citm_catalog.json**: 10.8850 ms

The test files can be found in the `test` folder.

The speed of serialization is not yet tested.

## Installation

To compile the Tinker JSON as dynamic library, you can make a `build` directory and enter it:

```
mkdir build
cd build
```

Then you can use **cmake** and **make** tools to build and install it. If you don't have these tools, you should probably install them on your computer first:

```
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..
make
sudo make install
```

Notice that you can use the parameter `-DCMAKE_INSTALL_PREFIX` to specify the destination of the installation. The default location is `/usr/local`.

## Usage

I provide a `test.cpp` to show compile and link the library to your project. To run the test, run the following commands:

```
g++ -std=c++11 test.cpp -o test-case -lTinkerJson
./test-case
```

Below is a simple example on how to parse a JSON string into a document, and get the value of the nodes. Finally it will stringify the document tree and return a string.

```c++
#include <tinker-json/TinkerValue.h>

#include <iostream>

using namespace Tinker;

int main() {
  const char* json = "{\"project\":\"NoobJson\",\"stars\":1}";
  Value v;
  v.Parse(json);

  const Value &p = v["project"];
  std::cout << "Type: " << p.GetTypeString() << std::endl;
  std::cout << "Value: " << p.GetString() << std::endl;

  const Value &s = v["stars"];
  std::cout << "Type: " << s.GetTypeString() << std::endl;
  std::cout << "Value: " << s.GetNumber() << std::endl;
  
  std::string str;
  v.Stringify(str);
  std::cout << "JSON Text: " << str << std::endl;
  
  std::string pre;
  v.Prettify(pre);
  std::cout << "Prettified JSON Text: " << std::endl;
  std::cout << pre << std::endl;

  return 0;
}
```

Result:

```
Type: String
Value: NoobJson
Type: Number
Value: 1
JSON Text: {"stars":1,"project":"NoobJson"}
Prettified JSON Text: 
{
  "stars": 1,
  "project": "NoobJson"
}
```

## Coding Environment

* **Language**: C++
* **System**: Mac OSX Sierra
* **IDE**: JetBrains CLion 2017.02
* **Compiler**: CMake 3.7.1 & Clang 800.0.42.1
