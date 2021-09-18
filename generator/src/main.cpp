#include <Kodgen/Misc/Filesystem.h>
#include <Kodgen/Misc/DefaultLogger.h>
#include <Kodgen/Parsing/FileParser.h>
#include <Kodgen/CodeGen/Macro/MacroCodeGenUnit.h>

#include "RefurekuGenerator/Parsing/FileParser.h"
#include "RefurekuGenerator/CodeGen/CodeGenManager.h"
#include "RefurekuGenerator/CodeGen/MacroCodeGenUnitSettings.h"
#include "RefurekuGenerator/CodeGen/ReflectionCodeGenModule.h"

#include "EngineCodeGenModule.hpp"

void loadEngineSettings(kodgen::ILogger& logger, kodgen::CodeGenManagerSettings& codeGenMgrSettings,
    kodgen::ParsingSettings& parsingSettings, kodgen::MacroCodeGenUnitSettings& codeGenUnitSettings,
    fs::path&& settingsFilePath, const std::string& engineDir)
{
    parsingSettings.setCompilerExeName("msvc");
    parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/Refureku/Refureku/Refureku/Library/Include");
    //parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/include/RapidXML");
    parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/gpm/include");
    parsingSettings.addProjectIncludeDirectory(engineDir);
    parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/include");
    parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/include/PhysX");
    parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party");
    parsingSettings.addProjectIncludeDirectory(engineDir + "/include");
    parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/imgui");

    for (auto& a : parsingSettings.getProjectIncludeDirectories())
    {
        std::cout << a.string() << std::endl;
    }

    codeGenUnitSettings.setOutputDirectory(engineDir + "/include/Generated/");

    codeGenMgrSettings.addSupportedFileExtension(".h");
    codeGenMgrSettings.addSupportedFileExtension(".hpp");

    codeGenMgrSettings.addToProcessDirectory(engineDir + "/include/engine");
    //codeGenMgrSettings.addToProcessFile("F:/Thomas/ProgProjects/GP_Engine/engine/include/Engine/ECS/Component/Camera.hpp");
    //parsingSettings.shouldLogDiagnostic = true;

    codeGenMgrSettings.addIgnoredDirectory(engineDir + "/include/Generated");


    parsingSettings.propertyParsingSettings.classMacroName     = "RFKClass";
    parsingSettings.propertyParsingSettings.structMacroName    = "RFKStruct";
    parsingSettings.propertyParsingSettings.fieldMacroName     = "RFKField";
    parsingSettings.propertyParsingSettings.enumMacroName      = "RFKEnum";
    parsingSettings.propertyParsingSettings.enumValueMacroName = "RFKEnumValue";
    parsingSettings.propertyParsingSettings.variableMacroName  = "RFKVariable";
    parsingSettings.propertyParsingSettings.methodMacroName    = "RFKMethod";
    parsingSettings.propertyParsingSettings.functionMacroName  = "RFKFunction";
    parsingSettings.propertyParsingSettings.namespaceMacroName = "RFKNamespace";

    codeGenUnitSettings.setClassFooterMacroPattern("##CLASSNAME##_GENERATED");
    codeGenUnitSettings.setHeaderFileFooterMacroPattern("File_GENERATED");
    codeGenUnitSettings.setGeneratedHeaderFileNamePattern("##FILENAME##.rfk.h");
    codeGenUnitSettings.setGeneratedSourceFileNamePattern("##FILENAME##.cpp.h");
}

void generateEngineFiles(const std::string& engineDir, bool forceRegenerateAll = false)
{
    std::cerr << "GENERATE ENGINE" << std::endl;


    kodgen::DefaultLogger logger;

    rfk::FileParser fileParser;
    fileParser.logger = &logger;

    rfk::CodeGenManager codeGenMgr;
    codeGenMgr.logger = &logger;

    rfk::MacroCodeGenUnitSettings codeGenUnitSettings;
    kodgen::MacroCodeGenUnit      codeGenUnit;
    codeGenUnit.logger = &logger;
    codeGenUnit.setSettings(codeGenUnitSettings);

    rfk::ReflectionCodeGenModule reflectionCodeGenModule;
    codeGenUnit.addModule(reflectionCodeGenModule);

    EngineCodeGenModule engineModule;
    codeGenUnit.addModule(engineModule);

    // Load settings
    logger.log("Working Directory: " + fs::current_path().string(), kodgen::ILogger::ELogSeverity::Info);

    // loadSettings(logger, codeGenMgr.settings, fileParser.getSettings(), codeGenUnitSettings,
    // "RefurekuTestsSettings.toml"); //For tests
    loadEngineSettings(logger, codeGenMgr.settings, fileParser.getSettings(), codeGenUnitSettings,
                 std::forward<fs::path>(engineDir), engineDir);

    // Parse
    kodgen::CodeGenResult genResult = codeGenMgr.run(fileParser, codeGenUnit, forceRegenerateAll);
}

void loadGameSettings(kodgen::ILogger& logger, kodgen::CodeGenManagerSettings& codeGenMgrSettings,
                        kodgen::ParsingSettings& parsingSettings, kodgen::MacroCodeGenUnitSettings& codeGenUnitSettings,
                        fs::path&& settingsFilePath, const std::string& engineDir, const std::string& gameDir)
{
    if (parsingSettings.setCompilerExeName("msvc"))
    {
        std::cout << "Compiler Set" << std::endl;
    }
    else
    {
        std::cout << "Compiler Not Set" << std::endl;
    }

    parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/Refureku/Refureku/Refureku/Library/Include");
    parsingSettings.addProjectIncludeDirectory(engineDir + "/include/Engine");
    parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/imgui");
    parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party");
    parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/gpm/include");
    parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/include/PhysX");
    parsingSettings.addProjectIncludeDirectory(engineDir + "/third_party/include");
    parsingSettings.addProjectIncludeDirectory(engineDir + "/gameDir/include");
    parsingSettings.addProjectIncludeDirectory(engineDir + "/include");
    parsingSettings.addProjectIncludeDirectory(gameDir + "/include");

    codeGenUnitSettings.setOutputDirectory(gameDir + "/include/Generated/");

    codeGenMgrSettings.addSupportedFileExtension(".h");
    codeGenMgrSettings.addSupportedFileExtension(".hpp");

    codeGenMgrSettings.addToProcessDirectory(gameDir + "/include/");
     //codeGenMgrSettings.addToProcessFile("F:/Thomas/ProgProjects/GP_Engine/projects/GPGame/include/myScript.hpp");
     //parsingSettings.shouldLogDiagnostic = true;

    codeGenMgrSettings.addIgnoredDirectory(gameDir + "/include/Generated/");

    parsingSettings.propertyParsingSettings.classMacroName = "RFKClass";
    parsingSettings.propertyParsingSettings.structMacroName = "RFKStruct";
    parsingSettings.propertyParsingSettings.fieldMacroName = "RFKField";
    parsingSettings.propertyParsingSettings.enumMacroName  = "RFKEnum";
    parsingSettings.propertyParsingSettings.enumValueMacroName = "RFKEnumValue";
    parsingSettings.propertyParsingSettings.variableMacroName = "RFKVariable";
    parsingSettings.propertyParsingSettings.methodMacroName = "RFKMethod";
    parsingSettings.propertyParsingSettings.functionMacroName = "RFKFunction";
    parsingSettings.propertyParsingSettings.namespaceMacroName = "RFKNamespace";

    codeGenUnitSettings.setClassFooterMacroPattern("##CLASSNAME##_GENERATED");
    codeGenUnitSettings.setHeaderFileFooterMacroPattern("File_GENERATED");
    codeGenUnitSettings.setGeneratedHeaderFileNamePattern("##FILENAME##.rfk.h");
    codeGenUnitSettings.setGeneratedSourceFileNamePattern("##FILENAME##.cpp.h");
}

void generateGameFiles(const std::string& gameDir, const std::string& engineDir, bool forceRegenerateAll = false)
{
    std::cout << "GENERATE GAME" << std::endl;

    kodgen::DefaultLogger logger;

    rfk::FileParser fileParser;
    fileParser.logger = &logger;

    rfk::CodeGenManager codeGenMgr;
    codeGenMgr.logger = &logger;

    rfk::MacroCodeGenUnitSettings codeGenUnitSettings;
    kodgen::MacroCodeGenUnit      codeGenUnit;
    codeGenUnit.logger = &logger;
    codeGenUnit.setSettings(codeGenUnitSettings);

    rfk::ReflectionCodeGenModule reflectionCodeGenModule;
    codeGenUnit.addModule(reflectionCodeGenModule);

    EngineCodeGenModule engineModule;
    codeGenUnit.addModule(engineModule);

    // Load settings
    logger.log("Working Directory: " + fs::current_path().string(), kodgen::ILogger::ELogSeverity::Info);

    // loadSettings(logger, codeGenMgr.settings, fileParser.getSettings(), codeGenUnitSettings,
    // "RefurekuTestsSettings.toml"); //For tests
    loadGameSettings(logger, codeGenMgr.settings, fileParser.getSettings(), codeGenUnitSettings,
                       std::forward<fs::path>(engineDir), engineDir, gameDir);

    // Parse
    kodgen::CodeGenResult genResult = codeGenMgr.run(fileParser, codeGenUnit, forceRegenerateAll);
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
            generateEngineFiles(argv[1], false);
            break;

        case 3:
            generateGameFiles(argv[2], argv[1], false);
            break;

        default:
            std::cout << "invalid number of arguments" << std::endl;
            break;
        }
    }
    else
    {
        const char* p2 = "..\\engine";
        const char* p1 = "..\\projects\\GPGame\\";

        generateEngineFiles(p2, false);
        generateGameFiles(p1, p2, true);
    }
    return 0;
}