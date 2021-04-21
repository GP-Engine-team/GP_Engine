namespace GPE
{
void InputManager::bindInput(int key, const std::string& action) noexcept
{
    m_actionMap.emplace(key, action);
}

int InputManager::addComponent(InputComponent& input) noexcept
{
    int key = static_cast<int>(m_inputComponents.size());
    m_inputComponents.emplace(key, &input);

    return key;
}

void InputManager::updateComponent(InputComponent* newPointerInputComponent, int key) noexcept
{
    m_inputComponents[key] = newPointerInputComponent;
}

void InputManager::removeComponent(int key) noexcept
{
    m_inputComponents.erase(key);
}

inline const Cursor& InputManager::getCursor() const noexcept
{
    return m_cursor;
}

void InputManager::setInputMode(const std::string& inputMode) noexcept
{
    m_currentInputMode = inputMode;
}

void InputManager::setCursorTrackingState(bool trackState) noexcept
{
    m_cursor.tracked = trackState;
}

inline const std::string& InputManager::getInputMode() noexcept
{
    return m_currentInputMode;
}
} // namespace GPE