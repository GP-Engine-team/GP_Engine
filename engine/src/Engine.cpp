#include "Engine/Engine.hpp"

using namespace GPE;

Engine* Engine::m_instance = nullptr;

void Engine::importConfig()
{
    FILE* pFile = nullptr;
    if (fopen_s(&pFile, configName.c_str(), "r+b"))
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

    doc.parse<0>(buffer.data());

    XmlLoader   context(doc);
    std::string firstLoadedSceneStr;
    GPE::load(context, firstLoadedSceneStr, XmlLoader::LoadInfo{"firstLoadedScene", "String", 0});
    sceneManager.firstLoadedScene = firstLoadedSceneStr;
    fclose(pFile);
}

void Engine::exportConfig()
{
    FILE* pFile = nullptr;
    if (fopen_s(&pFile, configName.c_str(), "w+b"))
    {
        Log::getInstance()->logError(stringFormat("The file \"%s\" was not opened to write", configName));
        return;
    }

    XmlSaver context(doc);
    GPE::save(context, sceneManager.firstLoadedScene.string(), XmlSaver::SaveInfo{"firstLoadedScene", "String", 0});
    std::string str = docToString(doc);
    fwrite(str.data(), str.size(), 1, pFile); // header
    fclose(pFile);
}