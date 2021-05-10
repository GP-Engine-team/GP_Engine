﻿#include <Firearm/GunMagazine.hpp>
File_GENERATED

namespace GPG
{

GunMagazine::GunMagazine(const Bullet& bulletStored, unsigned int magazineCapacity, unsigned int bulletsRemaining) noexcept
    : m_bulletStored    {bulletStored},
      m_magazineCapacity{magazineCapacity},
      m_bulletsRemaining{bulletsRemaining}
{
}


void GunMagazine::triggeredBullet()
{
    if (m_bulletsRemaining)
    {
        --m_bulletsRemaining;
        m_bulletStored.triggered();
    }
}


void GunMagazine::reload()
{
    m_bulletsRemaining = m_magazineCapacity;
}


bool GunMagazine::isEmpty() const
{
    return m_bulletsRemaining == 0u;
}

}


