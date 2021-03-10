﻿#include "Editor/EditorStartup.hpp"
#include "Engine/Core/Debug/Assert.hpp"
#include "Editor/Editor.hpp"
#include "Game.hpp"
#include "Engine/Core/System/SystemsManager.hpp"

using namespace GPE;

EditorStartup::EditorStartup() : m_editor(new Editor())
{

}

void EditorStartup::EditorStartup::StartGame()
{
	if (m_game != nullptr)
	{
		delete m_game;
	}
	m_game = new Game();
}

void EditorStartup::CloseGame()
{
	if (m_game != nullptr)
	{
		delete m_game;
		m_game = nullptr;
	}
}

void EditorStartup::update()
{
	GPE_ASSERT(m_editor != nullptr, "m_editor should be valid since we've just ran the editor.");

	auto update = [&](double fixedUnscaledDeltaTime, double deltaTime)
	{
		if (m_game != nullptr)
			m_game->update(fixedUnscaledDeltaTime, deltaTime);

		m_editor->update(fixedUnscaledDeltaTime, deltaTime);
	};
	auto fixedUpdate = [&](double fixedUnscaledDeltaTime, double fixedDeltaTime)
	{
		if (m_game != nullptr)
			m_game->fixedUpdate(fixedUnscaledDeltaTime, fixedDeltaTime);

		m_editor->fixedUpdate(fixedUnscaledDeltaTime, fixedDeltaTime);
	};
	auto render = [&]()
	{
		if (m_game != nullptr)
			m_game->render();

		m_editor->render();
	};
	GPE::SystemsManager::getInstance()->timeSystem.update(update, fixedUpdate, render);
}

EditorStartup::~EditorStartup()
{
	if (m_game != nullptr)
	{
		destroyGameInstance(m_game);
	}

	GPE_ASSERT(m_editor != nullptr, "m_editor should be valid since we've just ran the editor.");
	delete m_editor;
}
