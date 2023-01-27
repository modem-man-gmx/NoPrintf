# Short term feat request:

- support all kinds of integers,
- support all kinds of float, #ifdef selectable
- support a struct fraction { unsigned long nomin, denomin; } as float replacement, #ifdef selectable
- support engineering values with printable size length like .val( power, "W", 5) => "12,34 W" or "5,789 kW"


# Log Term Feature Request:

- simple LATIN translatable unit by using 2D array of strings (sorry for the charset codepage problems then),
- then verify, if Utf8 or std::wstring is the better on ESP, switch to, to be able using other charsets too. (we do not many string fiddling, so Utf8 could do the job)
