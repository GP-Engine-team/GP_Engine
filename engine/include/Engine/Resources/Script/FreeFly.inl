inline void FreeFly::up()
{
    getOwner().getTransform().translate(getOwner().getTransform().getVectorUp() * m_speed);
}


inline void FreeFly::down()
{
    getOwner().getTransform().translate(getOwner().getTransform().getVectorUp() * -m_speed);
}


inline void FreeFly::forward()
{
    getOwner().getTransform().translate(getOwner().getTransform().getVectorForward() * -m_speed);
}


inline void FreeFly::backward()
{
    getOwner().getTransform().translate(getOwner().getTransform().getVectorForward() * m_speed);
}


inline void FreeFly::left()
{
    getOwner().getTransform().translate(getOwner().getTransform().getVectorRight() * -m_speed);
}


inline void FreeFly::right()
{
    getOwner().getTransform().translate(getOwner().getTransform().getVectorRight() * m_speed);
}


inline void FreeFly::sprint()
{
    m_speed = 2.f;
}