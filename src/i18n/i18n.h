#ifndef LANGUAGE_H
#define LANGUAGE_H

// Define a macro to convert the language variable to a string literal
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

// Define the language variable
// const char* language = "fr";

// Try to include the language file based on the value of the language variable
#if __has_include(STR(language.h))
#include STR(language.h)
#else
// If the language file doesn't exist, include the English file (fallback)
#include "en.h"
#endif

#endif
