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
    tick /= 100;
    uint8_t effectID = (tick / 3) % m_fixtures.size();
    uint8_t colorID  = tick % 3;

    // turn all off
    for (auto& effect : m_fixtures) effect.setColor(Color::BLACK());

    auto fixture_front = m_fixtures.begin();
    std::advance(fixture_front, effectID);
    fixture_front->setColor(m_colors[colorID]);
}
