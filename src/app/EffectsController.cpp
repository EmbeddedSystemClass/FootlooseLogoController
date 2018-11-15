/*************************************************
 * @file EffectsController.h
 * @brief Effects controller interface
 *
 *************************************************/
#include "app/EffectsController.h"

#include <list>
#include <string>
#include <utility>
#include "stdint.h"

#include "OSError.h"
#include "app/RGBFixture.h"
#include "app/TaskState.h"
#include "queue.hpp"
#include "task.h"
#include "thread.hpp"

EffectBase::~EffectBase() {}

EffectsController::EffectsController(const char* Name, TaskHandle_t taskToNotify, uint8_t ID, cpp_freertos::Queue& inputQueue,
                                     cpp_freertos::Queue& outputQueue, uint32_t interval)
    : cpp_freertos::Thread(Name, 512, 2)
    , TaskState(taskToNotify, ID)
    , m_lastEffect(NULL)
    , m_inputQueue(inputQueue)
    , m_outputQueue(outputQueue)
    , m_interval(interval)
    , m_dmxSelector(0)
{
}

void EffectsController::addFixture(RGBFixture fixture) { m_fixtures.push_front(fixture); }

void EffectsController::addEffect(EffectBase& effect, DmxRange range)
{
    DmxEffect newEffect(effect, range);
    m_effects.push_front(newEffect);

    m_effects.sort(listCompareDmxRange);

    REPORTWARNING("Adding new effect with range: " + std::to_string(range.first) + " to " + std::to_string(range.first))

    if (isEffectOverlapping())
    {
        //        REPORTWARNING("Overlapping effects configuration")
    }
}

void EffectsController::Run()
{

    while (1)
    {
        uint32_t currentTick = xTaskGetTickCount();

        if (!m_inputQueue.IsEmpty())
        {
            uint8_t dmxInput[4];
            m_inputQueue.Dequeue(dmxInput);
            m_suggestedColor = Color(dmxInput[DMX_CHN_RED], dmxInput[DMX_CHN_GREEN], dmxInput[DMX_CHN_BLUE]);
            m_dmxSelector    = dmxInput[DMX_CHN_MODE];
        }

        DmxEffect* currentEffect = getApplicableEffect(m_dmxSelector);

        if (currentEffect != NULL)
        {
            if (currentEffect != m_lastEffect)
            {
                m_lastEffect = currentEffect;
                currentEffect->first.reset(currentTick);
            }

            currentEffect->first.apply(m_fixtures, currentTick, m_suggestedColor, (m_dmxSelector - currentEffect->second.first));
        }
        Delay(m_interval);
    }
}

EffectsController::DmxEffect* EffectsController::getApplicableEffect(uint8_t dmxSelector)
{

    for (auto const& effect : m_effects)
    {
        DmxRange range = effect.second;
        if (range.first <= dmxSelector && range.second >= dmxSelector)
        {
            return const_cast<DmxEffect*>(&effect);
        }
    }
    return NULL;
}

bool EffectsController::isEffectOverlapping() {}

bool EffectsController::listCompareDmxRange(const DmxEffect a, const DmxEffect b)
{
    bool retVal = false;
    // x.second is the dmx range where
    // x.second.first is the start of the range and
    // x.second.second the end of the range

    if (a.second.second < b.second.first)
    {
        retVal = true;
    }

    return retVal;
}
