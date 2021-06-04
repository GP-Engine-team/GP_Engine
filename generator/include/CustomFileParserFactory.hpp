#pragma once

#include <RefurekuGenerator/Parsing/FileParser.h>
#include <RefurekuGenerator/Parsing/FileParserFactory.h>

#include "ComponentGenerator.h"
#include "DefaultInspectPropertyRule.h"
#include "InspectPropertyRule.h"
#include "SerializePropertyRule.h"

class CustomFileParserFactory : public rfk::FileParserFactory<rfk::FileParser>
{
private:
    SerializePropertyRule      serializePropertyRule;
    InspectPropertyRule        inspectPropertyRule;
    ComponentGenerator         componentGenerator;
    DefaultInspectPropertyRule defaultInspectPropertyRule;

public:
    CustomFileParserFactory() noexcept
    {
        parsingSettings.propertyParsingSettings.complexPropertyRules.emplace_back(&serializePropertyRule);
        parsingSettings.propertyParsingSettings.complexPropertyRules.emplace_back(&inspectPropertyRule);
        parsingSettings.propertyParsingSettings.complexPropertyRules.emplace_back(&defaultInspectPropertyRule);
        parsingSettings.propertyParsingSettings.simplePropertyRules.emplace_back(&componentGenerator);
    }
};