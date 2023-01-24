# NoPrintf
NoPrintf - try to avoid printf without loosing formatted strings + without fiddling with &lt;&lt; operators

Story:
Some 90's days me and my team were forced by our boss to stop using `printf()` and its sisters, because of type-unsafety and of course because of rare but annoying crashes in long running integration test. Today I understand this, but in the past we all also hated him for this. How could we get well-readable logging and all the cool stuff?
C++ didn't reached `C++03` status, C not even `C99` and even our preprocessor did not learned some nasty tricks, as we didn't.
So we created functions like
```
void logwrite_and_Long(const char*, long);
void logwrite_and_LongString(const char*, long, const char*);
void logwrite_and_LongStringLong(const char*, long, const char*, long);
```
and the like.

Years later, we learned some Qt and Boost ways to handle such things better. But Boost is always a too fast moving target, sometimes breaking compatibility (yes, indeed, it did it several times). Qt, since Qt6 ... different story.

And on an ESP8266 or alike device it is never a good idea to start with big fat ugly libraries, right?

Then I had an interesting discussion with @lumapu about a simple and more or less efficient i18n implementation. We ended up with "no printf", because of grammar and argument order.
Asinde of this, I always wanted to have an "engineers formatting" for numbers, like
```
P = 1.345 kWh
/ 10^6
= 1.345 mWh
* 100
= "134.5 Wh
```
So I remembered the "no printf" times, the Qt .arg() idea and just started to warm up my compiler.

The compiler is almost all times g++, either on Debian, Raspbian or MSYS2/Win64, soon on Platform.IO for ESP8266.

The IDE I am using is most times the impressive flexible Codelite (by Eran). It strongly remembers me to glory days with similar CodeWright IDE. But I'm also okay with any other IDE which wasn't made for Java and then seduced to laggy support C++. (Even geany is often a better choice).

Long read! 
Up to now, this is more or less a warm-up for me. Perhaps some people come along and like to use or improve this.
Mid term goal is to have a robust, small, portable version which performs well on ESP and then to extend it by a simple 2D-array for translations.
