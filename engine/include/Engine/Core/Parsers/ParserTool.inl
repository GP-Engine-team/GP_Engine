#include "Engine/Core/Parsers/ParserTool.hpp"

inline bool stringStartsWith(const std::string& line, std::string&& title) noexcept
{
    return line.substr(0, title.size()) == title;
}

template <typename T>
T parse(const std::string& src, size_t pos, size_t len) noexcept
{
    T i;
    std::istringstream(src.substr(pos, len)) >> i;
    return i;
}

inline void removeUntilFirstSpace(const std::string& src, std::string& dest) noexcept
{
    std::size_t cursorStart = src.find_first_of(" ", 0) + 1;
    dest = src.substr(cursorStart, src.size() - cursorStart); // Search first word between two space
    if (!isalpha(dest.back()))
    {
        dest.erase(dest.end() - 1);
    }
}

inline std::string removeUntilFirstSpaceInPath(const char* path) noexcept
{
    std::string src(path);

    std::size_t cursStart = src.find_last_of('/');
    if (cursStart == std::string::npos)
        cursStart = 0;
    cursStart += 1;
    std::size_t cursEnd = src.find_last_of('.');

    return src.substr(cursStart, cursEnd - cursStart);
}