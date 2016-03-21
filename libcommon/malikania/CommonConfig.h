#ifndef COMMON_CONFIG_H
#define COMMON_CONFIG_H

#if defined(_WIN32)
#  if defined(MALIKANIA_COMMON_BUILD)
#    define MALIKANIA_COMMON_EXPORT __declspec(dllexport)
#  else
#    define MALIKANIA_COMMON_EXPORT __declspec(dllimport)
#  endif
#else
#  define MALIKANIA_COMMON_EXPORT
#endif

#endif // !COMMON_CONFIG_H