#pragma once

#include <RefurekuGenerator/Parsing/FileParserFactory.h>
#include <RefurekuGenerator/Parsing/FileParser.h>

#include "SerializePropertyRule.h"

class CustomFileParserFactory : public rfk::FileParserFactory<rfk::FileParser>
{
private:
    SerializePropertyRule serializePropertyRule;

public:
    CustomFileParserFactory() noexcept
    {
        parsingSettings.propertyParsingSettings.complexPropertyRules.emplace_back(&serializePropertyRule);
    }
};