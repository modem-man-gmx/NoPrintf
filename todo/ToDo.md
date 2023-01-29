# Plans

## Structure
- split src, inc, demo_src?

## Code Improvements
### On the `put()` method
- give an functor as argument, with a kind of `puts()` functor preset as default?

### On the `std::string& collect_int( unsigned int, std::string&, bool )`
- a hopefully smaller and faster implementation compared with [non-standard-C `ltoa()`](https://cplusplus.com/reference/cstdlib/itoa/) and [C++ `to_string()`](https://cplusplus.com/reference/string/to_string/)
- do a switchable by #ifdef USE_xxxx implementation via ltoa() and to_string()
- do benchmarks on size and speed, basically and on ESP8266

### On the `NoPrintf& arg( unsigned long int )`
- need to introduce template and template specification soon

### On the `std::string& collect_int( unsigned int, std::string&, bool )`
- the old C++03 trick of giving a buffer reference is probably not needed with gnu++11
- it is likely not needed with C++14 and absolutely not needed with c++17, but both are not to berequired for PlatformIO


## Structure
Either integrate
- [GoogleTest](http://google.github.io/googletest/platforms.html) or
- [UnitTest++](https://github.com/unittest-cpp/unittest-cpp)
instead of handcrafted main()-Demo code. Some crowds in github seem to prefer
- [unity](http://www.throwtheswitch.org/unity)
Hmmm ... Google Test is perhaps out, because rev13.xx is requiring C++14 in the meantime, need to check, if rev 12.xx is still maintained!

## Implement Methods
- NoPrintf& trim()
- NoPrintf& cut(siz_t max) for most needed string manipulation.

---

# Denied
- integrate either GoogleTest ... needs C++14 since rev 13.xx, but most PlatformIO project still stuck at gnu++11

# Done
- SEEN, that C++11 does support Delegating Constructors, made it
- integrate Unit Test -> I started with [Lightest](https://github.com/zhangzheheng12345/Lightest)
- verify std::string NoPrintf::get() vs. - std::string NoPrintf::get_ref(), do we need the distinction between ref and copy? in C++14 we have RVO, in 17 it is mandatory ... depends on what we have at ESP/PlatformIO ... most PlatformIO project still stuck at gnu++11 (c++11 with gnu extension, which are required for some Espresif parts) -> the good news: gnu++11 does RVO.
