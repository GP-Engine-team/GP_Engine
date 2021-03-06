inline void FreeFly::up()
{
    getOwner().getTransform().translate(getOwner().getTransform().getVectorUp() * m_speed *
                                        float(timeSys.getUnscaledDeltaTime()));
}

inline void FreeFly::down()
{
    getOwner().getTransform().translate(getOwner().getTransform().getVectorUp() * -m_speed *
                                        float(timeSys.getUnscaledDeltaTime()));
}

inline void FreeFly::forward()
{
    getOwner().getTransform().translate(getOwner().getTransform().getVectorForward() * m_speed *
                                        float(timeSys.getUnscaledDeltaTime()));
}

inline void FreeFly::backward()
{
    getOwner().getTransform().translate(getOwner().getTransform().getVectorForward() * -m_speed *
                                        float(timeSys.getUnscaledDeltaTime()));
}

inline void FreeFly::left()
{
    getOwner().getTransform().translate(getOwner().getTransform().getVectorRight() * -m_speed *
                                        float(timeSys.getUnscaledDeltaTime()));
}

inline void FreeFly::right()
{
    getOwner().getTransform().translate(getOwner().getTransform().getVectorRight() * m_speed *
                                        float(timeSys.getUnscaledDeltaTime()));
}

inline void FreeFly::walk()
{
    m_speed /= m_speedCoef;
}

inline void FreeFly::sprint()
{
    m_speed *= m_speedCoef;
}