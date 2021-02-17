/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <Kodgen/Misc/DefaultLogger.h>
#include <RefurekuGenerator/CodeGen/FileGenerationUnit.h>
#include <RefurekuGenerator/CodeGen/FileGenerator.h>
#include <RefurekuGenerator/Parsing/FileParser.h>
#include <RefurekuGenerator/Parsing/FileParserFactory.h>

class AbstractGame
{
public:
    AbstractGame() = default;
    virtual void update(double unscaledDeltaTime, double deltaTime) = 0;
    virtual void fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime) = 0;
    virtual void render() = 0;
    virtual ~AbstractGame() = default;
};