/*************************************************
 * @file Effects.cpp
 * @brief Effects  interface
 *
 *************************************************/
#include "app/Effects.h"

#include <list>
#include <string>
#include <utility>
#include "stdint.h"

#include "app/Color.h"
#include "app/EffectsController.h"
#include "app/RGBFixture.h"

void StaticColorEffect::reset(uint32_t tick) {}

void StaticColorEffect::apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset)
{
    for (auto& effect : m_fixtures)
    {
        effect.setColor(suggestedColor);
    }
}
