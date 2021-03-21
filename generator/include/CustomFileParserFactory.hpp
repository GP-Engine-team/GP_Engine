#pragma once

#include <RefurekuGenerator/Parsing/FileParserFactory.h>
#include <RefurekuGenerator/Parsing/FileParser.h>

#include "SerializePropertyRule.h"
#include "InspectPropertyRule.h"

class CustomFileParserFactory : public rfk::FileParserFactory<rfk::FileParser>
{
private:
    SerializePropertyRule serializePropertyRule;
    InspectPropertyRule inspectPropertyRule;

public:
    CustomFileParserFactory() noexcept
    {
        parsingSettings.propertyParsingSettings.complexPropertyRules.emplace_back(&serializePropertyRule);
        parsingSettings.propertyParsingSettings.complexPropertyRules.emplace_back(&inspectPropertyRule);
    }
};