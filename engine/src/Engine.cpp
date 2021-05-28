#include "Engine/Engine.hpp"

using namespace GPE;

Engine* Engine::m_instance = nullptr;

void Engine::importConfig()
{
    FILE* pFile = nullptr;
    if (fopen_s(&pFile, configName.c_str(), "r"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to read", configName));
        return;
    }

    // copy the file into the buffer. Read from head to EOF
    fseek(pFile, 0, SEEK_END);
    int end = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);
    int sizeRemainig = end;

    std::string buffer;
    buffer.assign(sizeRemainig, '\0');
    fread(buffer.data(), sizeof(char), sizeRemainig, pFile); // string buffer

    rapidxml::xml_document<> doc;
    doc.parse<0>(buffer.data());

    XmlLoader   context(doc);
    std::string firstLoadedSceneStr;
    GPE::load(context, firstLoadedSceneStr, XmlLoader::LoadInfo{"firstLoadedScene", "String", 0});
    std::string str               = docToString(doc);
    sceneManager.firstLoadedScene = firstLoadedSceneStr;
    fclose(pFile);
}

void Engine::exportConfig()
{
    FILE* pFile = nullptr;
    if (fopen_s(&pFile, configName.c_str(), "w"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", configName));
        return;
    }

    rapidxml::xml_document<> doc;
    XmlSaver                 context(doc);
    std::string              firstLoadedSceneStr = sceneManager.firstLoadedScene.string();
    GPE::save(context, firstLoadedSceneStr, XmlSaver::SaveInfo{"firstLoadedScene", "String", 0});
    std::string str = docToString(doc);
    fwrite(str.data(), str.size(), 1, pFile); // header
    fclose(pFile);
}