/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

class AbstractGame
{
public:
    AbstractGame();
    virtual void update(double unscaledDeltaTime, double deltaTime) = 0;
    virtual void fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime);
    virtual void render()   = 0;
    virtual ~AbstractGame() = default;
};
