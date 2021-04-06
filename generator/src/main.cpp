#include <Kodgen/Misc/DefaultLogger.h>
#include <RefurekuGenerator/CodeGen/FileGenerationUnit.h>
#include <RefurekuGenerator/CodeGen/FileGenerator.h>
#include <RefurekuGenerator/Parsing/FileParser.h>
#include <RefurekuGenerator/Parsing/FileParserFactory.h>

#include "CustomFileParserFactory.hpp"

void generateEngineFiles(const std::string& engineDir)
{
    std::cout << "GENERATE ENGINE" << std::endl;

    CustomFileParserFactory fileParserFactory;
    rfk::FileGenerator      fileGenerator;
    rfk::FileGenerationUnit fileGenerationUnit;

    // Set logger
    kodgen::DefaultLogger logger;

    fileParserFactory.logger = &logger;
    fileGenerator.logger     = &logger;

    kodgen::FileGenerationSettings& settings = fileGenerator.settings;
    fileParserFactory.parsingSettings.setCompilerExeName("msvc");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/include/Refureku");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/include/RapidXML");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/gpm/include");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir);
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/src");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/include");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/include");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/imgui");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/include/Refureku");

    for (auto& a : fileParserFactory.parsingSettings.getProjectIncludeDirectories())
    {
        std::cout << a.string() << std::endl;
    }

    settings.setOutputDirectory(engineDir + "/include/Generated/");

    settings.supportedExtensions = {".h", ".hpp"};

    settings.addToParseDirectory(engineDir + "/include/engine");

    settings.addIgnoredDirectory(engineDir + "/include/Generated");
    settings.addIgnoredDirectory(engineDir + "/include/engine/Core");
    // settings.addIgnoredDirectory(engineDir + "/include/engine/Serialization");

    // You will need to setup parsing settings and generation settings here.
    // Either load settings from a settings file, or set them by calling the appropriate methods.
    fileGenerator.generateFiles(fileParserFactory, fileGenerationUnit, false);
}

void generateGameFiles(const std::string& gameDir, const std::string& engineDir)
{
    std::cout << "GENERATE GAME" << std::endl;

    CustomFileParserFactory fileParserFactory;
    rfk::FileGenerator      fileGenerator;
    rfk::FileGenerationUnit fileGenerationUnit;

    // Set logger
    kodgen::DefaultLogger logger;

    fileParserFactory.logger = &logger;
    fileGenerator.logger     = &logger;

    kodgen::FileGenerationSettings& settings = fileGenerator.settings;
    if (fileParserFactory.parsingSettings.setCompilerExeName("msvc"))
    {
        std::cout << "Compiler Set" << std::endl;
    }
    else
    {
        std::cout << "Compiler Not Set" << std::endl;
    }
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/include/Refureku");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/include/Engine/");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/imgui");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/gpm/src");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/gpm/include");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/include");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/gameDir/include");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/src");
    fileParserFactory.parsingSettings.addProjectIncludeDirectory(engineDir + "/include");
    settings.setOutputDirectory(gameDir + "/include/Generated/");
    settings.supportedExtensions = {".h", ".hpp"};
    settings.addToParseDirectory(gameDir + "/include/");
    settings.addIgnoredDirectory(gameDir + "/include/Generated/");

    // You will need to setup parsing settings and generation settings here.
    // Either load settings from a settings file, or set them by calling the appropriate methods.
    fileGenerator.generateFiles(fileParserFactory, fileGenerationUnit, true);
}

// First Arg : Engine  (default : "../engine/")
// Second Arg : Game   (default : "../projects/GPGame/")
int main(int argc, char** argv)
{
    std::cout << "=== Refureku Generator Running" << std::endl;

    if (argc > 1)
    {

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
    }
    else
    {
        const char* p2 =
            "C:\\Users\\Utilisateur\\Downloads\\GP_EngineLAST\\GP_Engine\\projects\\GPGame\\/../../engine/";
        const char* p1 = "C:\\Users\\Utilisateur\\Downloads\\GP_EngineLAST\\GP_Engine\\projects\\GPGame\\/";

        generateGameFiles(p1, p2);
        generateEngineFiles(p2);
    }
    return 0;
}