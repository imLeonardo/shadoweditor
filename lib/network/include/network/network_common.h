#ifndef NETWORK_NETWORK_COMMON_H
#define NETWORK_NETWORK_COMMON_H

#include "common/platform.hpp"

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
#    if defined(NETWORK_SHARED_LIB)
#        if defined(NETWORK_EXPORTS)
#            define NETWORK_API __declspec(dllexport)
#        else// !NETWORK_EXPORTS
#            define NETWORK_API __declspec(dllimport)
#        endif// #if defined(NETWORK_EXPORTS)
#    else
#        define NETWORK_API
#    endif
#else// !defined(PLATFORM_OS_FAMILY_WINDOWS)
#    if defined(NETWORK_SHARED_LIB)
#        if defined(NETWORK_EXPORTS)
#            define NETWORK_API __attribute__((visibility("default")))
#        else// !NETWORK_EXPORTS
#            define NETWORK_API
#        endif// #if defined(NETWORK_EXPORTS)
#    else
#        define NETWORK_API
#    endif
#endif// #if defined(PLATFORM_OS_FAMILY_WINDOWS)

#endif// NETWORK_NETWORK_COMMON_H