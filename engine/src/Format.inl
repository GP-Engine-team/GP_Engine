#include "Engine/Core/Tools/Format.hpp"

template <typename... Args>
std::string Engine::Core::Tools::stringFormat(const std::string& format, Args... args) noexcept
{
    int size = snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'

    assert(size > 0 || "Error during formatting.");

    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args...);

    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}