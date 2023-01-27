# Plans

## Structure
- split src, inc, demo_src?

## Code Improvements
- On: put()
- give an functor as argument, with kind of puts() as default?

- On: static std::string& collect_int( unsigned int uVal, std::string& buffer, bool Minus = false )
- a hopefully smaller and faster implementation compared with ltoa() and to_string()
- #ifdef switchable implementation via ltoa() and to_string()
- do benchmarks on size and speed basically and on ESP

- On: NoPrintf& NoPrintf::arg( unsigned long int uVal )
- need to introduce template and template specification soon


## Structure
- integrate Either [GoogleTest](http://google.github.io/googletest/platforms.html) or [UnitTest++](https://github.com/unittest-cpp/unittest-cpp) instead of handcrafted main()-Demo code. Some crowds in github seem to prefer [unity](http://www.throwtheswitch.org/unity), Hmmm .. Google Test is perhaps out, because of requiring C++14 in the meantime?

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
