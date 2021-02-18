/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include "Engine/Core/TimeSystem/TimeSystem.hpp"

class ContextStartup
{
public:
    GPE::TimeSystem timeSystem;
    bool isRunning = true;

public:
    // Loads and initializes libraries
    ContextStartup()
    {
    }

    // Contains gameloop
    void run()
    {
        while (isRunning)
        {
            update();
        }
    }

    virtual void update() = 0;

    // Unload libraries
    ~ContextStartup()
    {
    }
};