#pragma once
#include "GPM/Vector2.hpp"

namespace GPE
{
class Cursor
{
public:
    Cursor()  = default;
    ~Cursor() = default;

public:
    GPM::Vec2 m_position     = {0, 0};
    float     m_speed        = {1};
    GPM::Vec2 m_displacement = {0, 0};
};
} // namespace GPE