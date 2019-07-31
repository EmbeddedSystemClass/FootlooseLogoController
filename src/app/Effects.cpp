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

void StaticColorEffect::reset(uint32_t tick) { (void)tick; }

void StaticColorEffect::apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset)
{
    (void)tick;
    (void)dmxOffset;
    for (auto& effect : m_fixtures)
    {
        effect.setColor(suggestedColor);
    }
}

void CycleColorEffect::reset(uint32_t tick)
{
    (void)tick;
    cnt = 0;

    m_color.setRed(0);
    m_color.setGreen(0);
    m_color.setBlue(0);
}

void CycleColorEffect::apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset)
{
    (void)tick;
    (void)dmxOffset;
    (void)suggestedColor;
    cnt++;
    Color color = Color(cnt, cnt, cnt);
    for (auto& effect : m_fixtures)
    {
        effect.setColor(color);
    }
}

CycleRGBPerFixtureEffect::CycleRGBPerFixtureEffect()
{
    m_colors[0] = Color::RED();
    m_colors[1] = Color::GREEN();
    m_colors[2] = Color::BLUE();
}

void CycleRGBPerFixtureEffect::reset(uint32_t tick) { (void)tick; }

void CycleRGBPerFixtureEffect::apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset)
{
    (void)dmxOffset;
    (void)suggestedColor;
    tick /= 500;
    uint8_t effectID = (tick / 3) % m_fixtures.size();
    uint8_t colorID  = tick % 3;

    // turn all off
    for (auto& effect : m_fixtures) effect.setColor(Color::BLACK());

    auto fixture_front = m_fixtures.begin();
    std::advance(fixture_front, effectID);
    fixture_front->setColor(m_colors[colorID]);
}

StrobeEffect::StrobeEffect() {}

void StrobeEffect::reset(uint32_t tick) {}

void StrobeEffect::apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset)
{
    tick /= (20 + (dmxOffset * 5));
    tick %= 5;

    if (tick != 0) suggestedColor = Color::BLACK();

    for (auto& effect : m_fixtures) effect.setColor(suggestedColor);
}

RunningEffect::RunningEffect() {}

void RunningEffect::reset(uint32_t tick) {}

void RunningEffect::apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset)
{
    (void)dmxOffset;
    (void)suggestedColor;
    tick /= (50 + (dmxOffset * 10));
    uint8_t effectID = (tick / 3) % m_fixtures.size();

    // turn all off
    for (auto& effect : m_fixtures) effect.setColor(Color::BLACK());

    auto fixture_front = m_fixtures.begin();
    std::advance(fixture_front, effectID);
    fixture_front->setColor(suggestedColor);
}

const uint8_t HSVlights[61] = {0,   4,   8,   13,  17,  21,  25,  30,  34,  38,  42,  47,  51,  55,  59,  64,  68,  72,  76,  81,  85,
                               89,  93,  98,  102, 106, 110, 115, 119, 123, 127, 132, 136, 140, 144, 149, 153, 157, 161, 166, 170, 174,
                               178, 183, 187, 191, 195, 200, 204, 208, 212, 217, 221, 225, 229, 234, 238, 242, 246, 251, 255};

Color GetRainbowColor(uint32_t tick)
{
    Color retVal;
    tick %= 360;
    if (tick < 60)
    {
        retVal.setRed(255);
        retVal.setGreen(HSVlights[tick]);
        retVal.setBlue(0);
    }
    else if (tick < 120)
    {
        retVal.setRed(HSVlights[120 - tick]);
        retVal.setGreen(255);
        retVal.setBlue(0);
    }
    else if (tick < 180)
    {
        retVal.setRed(0), retVal.setGreen(255);
        retVal.setBlue(HSVlights[tick - 120]);
    }
    else if (tick < 240)
    {
        retVal.setRed(0), retVal.setGreen(HSVlights[240 - tick]);
        retVal.setBlue(255);
    }
    else if (tick < 300)
    {
        retVal.setRed(HSVlights[tick - 240]), retVal.setGreen(0);
        retVal.setBlue(255);
    }
    else
    {
        retVal.setRed(255), retVal.setGreen(0);
        retVal.setBlue(HSVlights[360 - tick]);
    }
    return retVal;
}

RainbowEffect::RainbowEffect() {}

void RainbowEffect::reset(uint32_t tick) {}

void RainbowEffect::apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset)
{
    Color color;
    tick /= (dmxOffset + 1);
    tick %= 360;
    color = GetRainbowColor(tick);
    for (auto& effect : m_fixtures) effect.setColor(color);
}

RainbowSpreadEffect::RainbowSpreadEffect() {}

void RainbowSpreadEffect::reset(uint32_t tick) {}

void RainbowSpreadEffect::apply(std::list<RGBFixture>& m_fixtures, uint32_t tick, Color suggestedColor, uint8_t dmxOffset)
{
    Color color;
    tick /= (dmxOffset + 1);
    tick = (360 - (tick % 360));

    uint32_t offSetPerFixture = 360 / m_fixtures.size();
    uint32_t ii               = 0;
    for (auto& effect : m_fixtures)
    {
        ii++;
        effect.setColor(GetRainbowColor(tick + (offSetPerFixture * ii)));
    }
}
