/*
                                   )
                                  (.)
                                  .|.
                                  | |
                              _.--| |--._
                           .-';  ;`-'& ; `&.
                          \   &  ;    &   &_/
                           |"""---...---"""|
                           \ | | | | | | | /
                            `---.|.|.|.---'

 * This file is generated by bake.lang.c for your convenience. Headers of
 * dependencies will automatically show up in this file. Include bake_config.h
 * in your main project file. Do not edit! */

#ifndef OPTION_AND_RESULT_BAKE_CONFIG_H
#define OPTION_AND_RESULT_BAKE_CONFIG_H

/* Headers of public dependencies */
/* No dependencies */

/* Convenience macro for exporting symbols */
#ifndef option_and_result_STATIC
#if defined(option_and_result_EXPORTS) && (defined(_MSC_VER) || defined(__MINGW32__))
  #define OPTION_AND_RESULT_API __declspec(dllexport)
#elif defined(option_and_result_EXPORTS)
  #define OPTION_AND_RESULT_API __attribute__((__visibility__("default")))
#elif defined(_MSC_VER)
  #define OPTION_AND_RESULT_API __declspec(dllimport)
#else
  #define OPTION_AND_RESULT_API
#endif
#else
  #define OPTION_AND_RESULT_API
#endif

#endif

