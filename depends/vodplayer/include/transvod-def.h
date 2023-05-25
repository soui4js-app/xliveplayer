#pragma once

#define transvod_ns transvod_win
#ifdef __cplusplus
#define VOD_NS_BEGIN namespace transvod_ns{
#define VOD_NS_END   }
#ifndef CONST
#define CONST const
#endif
#else
#define VOD_NS_BEGIN
#define VOD_NS_END
#ifndef CONST
#define CONST
#endif
#endif

#ifdef TRANSVOD_EXPORTS
#define TRANSVOD_API __declspec(dllexport)
#else
#define TRANSVOD_API __declspec(dllimport)
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <ObjBase.h>
#include <stdint.h>