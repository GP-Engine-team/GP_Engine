#include <Firearm/PPSH41.hpp>
#include "Generated/PPSH41.rfk.h"
File_GENERATED

namespace GPG
{

PPSH41::PPSH41(GPE::GameObject & owner) noexcept
    : Firearm(owner, GunMagazine(Bullet(5.f), 30, 30), 3.f, 0.1f)
{
}

}
