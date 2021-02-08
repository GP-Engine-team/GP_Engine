/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

class ContextStartup
{
  public:
    // Loads and initializes libraries
    ContextStartup()
    {
    }

    // Contains gameloop
    void run()
    {
        for (;;)
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