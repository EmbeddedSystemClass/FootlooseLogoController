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

class CycleColorEffect : public EffectBase
{
public:
    void reset(uint32_t tick);

    void apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset);

private:
    uint8_t cnt;
    Color   m_color;
};

class CycleRGBPerFixtureEffect : public EffectBase
{
public:
    CycleRGBPerFixtureEffect();

    void reset(uint32_t tick);

    void apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset);

private:
    Color m_colors[3];
};

class RunningEffect : public EffectBase
{
public:
    RunningEffect();

    void reset(uint32_t tick);

    void apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset);
};

class StrobeEffect : public EffectBase
{
public:
    StrobeEffect();

    void reset(uint32_t tick);

    void apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset);
};

class RainbowEffect : public EffectBase
{
public:
    RainbowEffect();

    void reset(uint32_t tick);

    void apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset);
};

class RainbowSpreadEffect : public EffectBase
{
public:
    RainbowSpreadEffect();

    void reset(uint32_t tick);

    void apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset);
};
