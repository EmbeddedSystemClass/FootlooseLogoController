/*************************************************
 * @file Effects.h
 * @brief Effects  interface
 *
 *************************************************/
#pragma once

#include <list>
#include <string>
#include <utility>
#include "stdint.h"

#include "app/Color.h"
#include "app/EffectsController.h"
#include "app/RGBFixture.h"

class StaticColorEffect : public EffectBase
{
public:
    void reset(uint32_t tick);

    void apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset);

private:
};
