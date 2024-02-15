Checking C++ version. Use \_MSVC\_LANG

```cpp
#if _MSVC_LANG >= 202002L
	// Code here ...
#endif
```

"MSVC also doesn't set this macro correctly, to this very day. By default it's defined to `199711L` regardless of the language version, and you either need to add [`/Zc:__cplusplus`](https://learn.microsoft.com/en-us/cpp/build/reference/zc-cplusplus) to compiler flags, or check a MSVC-specific macro `_MSVC_LANG` instead, which always has the right value." - pmr | HolyBlackCat

Version info :
- C++ pre-C++98: `__cplusplus` is `1`.
- C++98: `__cplusplus` is `199711L`.
- C++98 + TR1: This reads as C++98 and there is no way to check that I know of.
- C++11: `__cplusplus` is `201103L`.
- C++14: `__cplusplus` is `201402L`.
- C++17: `__cplusplus` is `201703L`.
- C++20: `__cplusplus` is `202002L`.

Info from here :
https://stackoverflow.com/questions/2324658/how-to-determine-the-version-of-the-c-standard-used-by-the-compiler