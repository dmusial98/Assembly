#pragma once

#ifdef BITMAPCPP_EXPORTS
#define BITMAPCPP_API __declspec(dllexport)
#else
#define BITMAPCPP_API __declspec(dllimport)
#endif

int BITMAPCPP_API FunctionCpp();

