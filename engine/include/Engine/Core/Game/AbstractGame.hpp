/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 * found in the top-level directory of this distribution.
 */

#pragma once

namespace GPE
{

class AbstractGame
{
public:
    virtual ~AbstractGame() = default;

    virtual void update(double unscaledDeltaTime, double deltaTime)                = 0;
    virtual void fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime) = 0;
    virtual void render()                                                          = 0;

    virtual void setViewport(float x, float y, float w, float h) = 0;
};

} // namespace GPE