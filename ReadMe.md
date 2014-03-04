JsonCppTest
=====
 test source code : simple benchmarks of 3 json c++ libraries.

##tested environments.

- Windows 7 x64
- Intel Core i7 4770K Quad-core / 16GB Ram
- Visual Studio 2013 / compiled x64 binary with /O2

##tested libraries.

following json lib for c++ tested.

- libjson - http://sourceforge.net/projects/libjson/
- rapidjson - https://code.google.com/p/rapidjson/
- jsoncpp - http://sourceforge.net/projects/jsoncpp/

- most json files generated from http://www.json-generator.com/

##test methods:
- parsing from json string: 1
- writing to string: 1

##results graph:

﻿![](https://raw.github.com/icedac/JsonCppTest/master/result.png)
﻿

##conclusion

- winner: rapidjson
- rapidjson 2.5x to 4.0x faster than libjson (2.5x for 0.5k bytes json files, 4.0x for 200k bytes json files)
- rapidjson 3x to 4.5x faster than jsoncpp
