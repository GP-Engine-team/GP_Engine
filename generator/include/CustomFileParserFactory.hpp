#pragma once

#include <RefurekuGenerator/Parsing/FileParserFactory.h>
#include <RefurekuGenerator/Parsing/FileParser.h>

#include "SerializePropertyRule.h"
#include "InspectPropertyRule.h"
#include "ComponentGenerator.h"

class CustomFileParserFactory : public rfk::FileParserFactory<rfk::FileParser>
{
private:
    SerializePropertyRule serializePropertyRule;
    InspectPropertyRule inspectPropertyRule;
	ComponentGenerator    componentGenerator;

public:
    CustomFileParserFactory() noexcept
    {
        parsingSettings.propertyParsingSettings.complexPropertyRules.emplace_back(&serializePropertyRule);
        parsingSettings.propertyParsingSettings.complexPropertyRules.emplace_back(&inspectPropertyRule);
		parsingSettings.propertyParsingSettings.complexPropertyRules.emplace_back(&componentGenerator);
    }
};