#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <iostream>
#include <string>
#include <vector>

bool checkAlErrors(const std::string& filename, const std::uint_fast32_t line);

bool checkAlcErrors(const std::string& filename, const std::uint_fast32_t line, ALCdevice* device);

template <typename alFunction, typename... Params>
auto alCallImpl(const char* filename, const std::uint_fast32_t line, alFunction function, Params... params) ->
    typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, decltype(function(params...))>
{
    auto ret = function(std::forward<Params>(params)...);
    checkAlErrors(filename, line);
    return ret;
}

template <typename alFunction, typename... Params>
auto alCallImpl(const char* filename, const std::uint_fast32_t line, alFunction function, Params... params) ->
    typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
{
    function(std::forward<Params>(params)...);
    return checkAlErrors(filename, line);
}

template <typename alcFunction, typename... Params>
auto alcCallImpl(const char* filename, const std::uint_fast32_t line, alcFunction function, ALCdevice* device,
                 Params... params) ->
    typename std::enable_if_t<std::is_same_v<void, decltype(function(params...))>, bool>
{
    function(std::forward<Params>(params)...);
    return checkAlcErrors(filename, line, device);
}

template <typename alcFunction, typename ReturnType, typename... Params>
auto alcCallImpl(const char* filename, const std::uint_fast32_t line, alcFunction function, ReturnType& returnValue,
                 ALCdevice* device, Params... params) ->
    typename std::enable_if_t<!std::is_same_v<void, decltype(function(params...))>, bool>
{
    returnValue = function(std::forward<Params>(params)...);
    return checkAlcErrors(filename, line, device);
}
#define AL_CALL(function, ...) alCallImpl(__FILE__, __LINE__, function, __VA_ARGS__)
#define ALC_CALL(function, device, ...) alcCallImpl(__FILE__, __LINE__, function, device, __VA_ARGS__)
