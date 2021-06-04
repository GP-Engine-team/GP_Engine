namespace GPE
{
void InputManager::bindInput(int key, const std::string& action) noexcept
{
    using const_iterator    = std::unordered_multimap<int, std::string>::const_iterator;
    const const_iterator it = m_actionMap.find(key);

    if (it != m_actionMap.end() && it->second == action)
        return;

    m_actionMap.emplace(key, action);
}

int InputManager::addComponent(InputComponent& input) noexcept
{
    m_key++;
    m_inputComponents.emplace(m_key, &input);

    return m_key;
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
    m_previousInputMode = std::move(m_currentInputMode);
    m_currentInputMode  = inputMode;
}

void InputManager::restorePreviousInputMode() noexcept
{
    std::swap(m_previousInputMode, m_currentInputMode);
}

void InputManager::setCursorTrackingState(bool trackState) noexcept
{
    m_cursor.tracked = trackState;
}

bool InputManager::getCursorTrackingState() const noexcept
{
    return m_cursor.tracked;
}

inline const std::string& InputManager::getInputMode() noexcept
{
    return m_currentInputMode;
}
} // namespace GPE
