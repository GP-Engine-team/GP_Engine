#pragma once

#include <RefurekuGenerator/Parsing/FileParserFactory.h>
#include <RefurekuGenerator/Parsing/FileParser.h>

#include "SerializePropertyRule.h"
#include "ComponentGenerator.h"

class CustomFileParserFactory : public rfk::FileParserFactory<rfk::FileParser>
{
private:
	SerializePropertyRule serializePropertyRule;
	ComponentGenerator    componentGenerator;

public:
	CustomFileParserFactory() noexcept
	{
		parsingSettings.propertyParsingSettings.complexPropertyRules.emplace_back(&serializePropertyRule);
		parsingSettings.propertyParsingSettings.complexPropertyRules.emplace_back(&componentGenerator);
	}
};