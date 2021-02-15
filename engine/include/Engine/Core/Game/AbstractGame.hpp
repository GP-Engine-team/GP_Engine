/*
 * Copyright (C) 2021 Amara Sami, Dallard Thomas, Nardone William, Six Jonathan
 * This file is subject to the LGNU license terms in the LICENSE file
 *	found in the top-level directory of this distribution.
 */

#pragma once

#include <Kodgen/Misc/DefaultLogger.h>
#include <RefurekuGenerator/Parsing/FileParser.h>
#include <RefurekuGenerator/Parsing/FileParserFactory.h>
#include <RefurekuGenerator/CodeGen/FileGenerator.h>
#include <RefurekuGenerator/CodeGen/FileGenerationUnit.h>

#include "Engine/Foo.hpp"

class AbstractGame
{
public:
    AbstractGame()
    {
        rfk::FileParserFactory<rfk::FileParser> fileParserFactory;
        rfk::FileGenerator fileGenerator;
        rfk::FileGenerationUnit fileGenerationUnit;

        // Set logger
        kodgen::DefaultLogger logger;

        fileParserFactory.logger = &logger;
        fileGenerator.logger = &logger;

        fileParserFactory.loadSettings("RefurekuSettings.toml");
        fileGenerator.loadSettings("RefurekuSettings.toml");

        fileGenerator.generateFiles(fileParserFactory, fileGenerationUnit);

        std::cout << Foo::staticGetArchetype().getField("f")->ownerStruct->name << '\n';
    }
    virtual void update(double unscaledDeltaTime, double deltaTime) = 0;
    virtual void fixedUpdate(double fixedUnscaledDeltaTime, double fixedDeltaTime) = 0;
    virtual void render() = 0;
    virtual ~AbstractGame() = default;
};