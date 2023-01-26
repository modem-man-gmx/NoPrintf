# NoPrintf
- NoPrintf - try to avoid printf without loosing formatted strings + without fiddling with &lt;&lt; operators.
- Have engineers formatted units like mV, kWh auto selected from field size.
- Select language from a table.
- Robustness by test driven development and unit test integration in local runs and github actions.

## Join me?
Just fork this git repository, pick the "dev" branch and send pull requests of tested code.

## Why?
Inspiration came from 30 years bad experience with printf, good things seen from Qt and Boost.
And last but not least from an i18n discussion on [Ahoy DTU Discord](https://discord.gg/WzhxEY62mB),
where we ended up in simple i18n allows "no printf", because of grammar and argument order.

Aside of this, I always wanted to have an "fixed format engineers formatting" for numbers, like
```
1.345 mW * 100
134.5 mW * 100
13.45 W * 100
1.345 kW * 100
134.5 kW * 100
13.45 MW
```
and so on ...

## Goals:
- working for strings and const char
- working for relevant integers
- conditionally compile with float
- benchmark for low copy mem footprint & speed
- metric prefixes "fpnµm kMGTP" auto select the way, always having 1 to 3 digits in front of the dot, on grow or shrink: take next 10^3 prefix.
- ESP8266 and ESP32 optimized
- translation table for latin-1 codepage languages 1st.
- verification of UTF8, mbcs or wstring while keeping gnu++11 compatibility (PlatformIO default)

## ToDo and Feature Requests
[ToDo list](todo/ToDo.md)
[Feature Requests](todo/feature_requests.md)

## read more?
[Whole Story](doc/README_STORY.md)
