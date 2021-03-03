#include <Kodgen/Misc/DefaultLogger.h>
#include <RefurekuGenerator/Parsing/FileParser.h>
#include <RefurekuGenerator/Parsing/FileParserFactory.h>
#include <RefurekuGenerator/CodeGen/FileGenerator.h>
#include <RefurekuGenerator/CodeGen/FileGenerationUnit.h>

#include "CustomFileParserFactory.hpp"

void generateEngineFiles(const std::string& engineDir)
{
    std::cout << "GENERATE ENGINE" << std::endl;

    CustomFileParserFactory fileParserFactory;
    rfk::FileGenerator      fileGenerator;
    rfk::FileGenerationUnit fileGenerationUnit;

    //Set logger
    kodgen::DefaultLogger logger;

    fileParserFactory.logger = &logger;
    fileGenerator.logger = &logger;

    kodgen::FileGenerationSettings& settings = fileGenerator.settings;
    fileParserFactory.parsingSettings.setCompilerExeName("msvc");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/include/Refureku");
    settings.setOutputDirectory(engineDir + "/include/Generated/");
    settings.supportedExtensions = { ".h", ".hpp" };
    settings.addToParseDirectory(engineDir + "/include/Engine/");
    settings.addIgnoredDirectory(engineDir + "/include/Generated/");

    //You will need to setup parsing settings and generation settings here.
    //Either load settings from a settings file, or set them by calling the appropriate methods.
    fileGenerator.generateFiles(fileParserFactory, fileGenerationUnit);
}

void generateGameFiles(const std::string& gameDir, const std::string& engineDir)
{
    std::cout << "GENERATE GAME" << std::endl;

    CustomFileParserFactory fileParserFactory;
    rfk::FileGenerator      fileGenerator;
    rfk::FileGenerationUnit fileGenerationUnit;

    //Set logger
    kodgen::DefaultLogger logger;

    fileParserFactory.logger = &logger;
    fileGenerator.logger = &logger;

    kodgen::FileGenerationSettings& settings = fileGenerator.settings;
    fileParserFactory.parsingSettings.setCompilerExeName("msvc");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/include/Refureku");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/include/Engine/");
    settings.setOutputDirectory(gameDir + "/include/Generated/");
    settings.supportedExtensions = { ".h", ".hpp" };
    settings.addToParseDirectory(gameDir + "/include/");
    settings.addIgnoredDirectory(gameDir + "/include/Generated/");

    //You will need to setup parsing settings and generation settings here.
    //Either load settings from a settings file, or set them by calling the appropriate methods.
    fileGenerator.generateFiles(fileParserFactory, fileGenerationUnit);
}


// First Arg : Engine  (default : "../enigne/") 
// Second Arg : Game   (default : "../projects/GPGame/")
int main(int argc, char** argv)
{
    std::cout << "=== Refureku Generator Running" << std::endl;
    for (int i = 0; i < argc; i++)
    {
        std::cout << "argv[" << i << "] = " << argv[i] << std::endl;
    }
    switch (argc)
    {
    case 2:
        generateEngineFiles(argv[1]);
        break;

    case 3:
        generateGameFiles(argv[2], argv[1]);
        break;

    default:
        std::cout << "invalid number of arguments" << std::endl;
        break;
    }

    return 0;
}