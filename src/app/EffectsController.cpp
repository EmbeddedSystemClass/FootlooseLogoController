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
#include "app/DMXTransmitter.h"
#include "app/RGBFixture.h"
#include "app/TaskState.h"
#include "queue.hpp"
#include "task.h"
#include "thread.hpp"

EffectBase::~EffectBase() {}

EffectsController::EffectsController(const char* Name, TaskHandle_t taskToNotify, uint8_t ID, cpp_freertos::Queue& inputQueue,
                                     cpp_freertos::Queue& outputQueue, uint32_t interval)
    : cpp_freertos::Thread(Name, 500, 2)
    , TaskState(taskToNotify, ID)
    , m_lastEffect(NULL)
    , m_inputQueue(inputQueue)
    , m_outputQueue(outputQueue)
    , m_interval(interval)
    , m_dmxSelector(0)
    , m_testModePin1(NULL)
    , m_testModePin2(NULL)
    , m_testEffectCounter(0)
    , m_effectMode(EffectModeDmx)
{
}

EffectsController::EffectsController(const char* Name, TaskHandle_t taskToNotify, uint8_t ID, cpp_freertos::Queue& inputQueue,
                                     cpp_freertos::Queue& outputQueue, uint32_t interval, GPIOpin* testMode1, GPIOpin* testMode2)
    : cpp_freertos::Thread(Name, 500, 2)
    , TaskState(taskToNotify, ID)
    , m_lastEffect(NULL)
    , m_inputQueue(inputQueue)
    , m_outputQueue(outputQueue)
    , m_interval(interval)
    , m_dmxSelector(0)
    , m_testModePin1(testMode1)
    , m_testModePin2(testMode2)
    , m_testEffectCounter(0)
    , m_effectMode(EffectModeDmx)
{
}

void EffectsController::addFixture(RGBFixture fixture)
{
    m_fixtures.push_front(fixture);
    m_fixtures.sort(listCompareFixtureLocation);
}

void EffectsController::addEffect(EffectBase& effect, DmxRange range)
{
    DmxEffect newEffect(effect, range);
    m_effects.push_front(newEffect);

    m_effects.sort(listCompareDmxRange);

    // REPORTWARNING("Adding new effect with range: " + std::to_string(range.first) + " to " + std::to_string(range.first))

    if (isEffectOverlapping())
    {
        REPORTWARNING("Overlapping effects configuration")
    }
}

void EffectsController::Run()
{

    while (1)
    {
        bool     inTestMode  = false;
        uint32_t currentTick = xTaskGetTickCount();

        if (!m_inputQueue.IsEmpty())
        {
            uint8_t dmxInput[4];
            m_inputQueue.Dequeue(dmxInput);
            m_suggestedColor = Color(dmxInput[DMX_CHN_RED], dmxInput[DMX_CHN_GREEN], dmxInput[DMX_CHN_BLUE]);
            m_dmxSelector    = dmxInput[DMX_CHN_MODE];
        }

        if (m_testModePin1 != NULL && m_testModePin2 != NULL)
        {
            if (m_testModePin1)
            {
                inTestMode = true;
                applyTestEffect(m_testModePin2);
            }
        }

        if (!inTestMode)
        {

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
        }

        DMXTransmitter::DMXQueueItem output;

        // insert all updates in the queue
        for (auto& fixture : m_fixtures)
        {
            bool createNewMessage = false;
            // check if we can use existing message
            if ((fixture.getDmxChannelCount() + output.channelCount) <= DMXTransmitter::DMXQueueItem::channelDataLength)
            {
                if ((output.channelCount + output.startAddress) == fixture.getDmxStartAddress())
                {
                    fixture.getDmxData(&output.channeldata[output.channelCount]);
                    output.channelCount += fixture.getDmxChannelCount();
                }
                else
                {
                    // message not aligned
                    createNewMessage = true;
                }
            }
            else
            {
                // message full send it
                createNewMessage = true;
            }

            if (createNewMessage)
            {
                // send old message
                m_outputQueue.Enqueue(&output, 10);

                output.channelCount = fixture.getDmxChannelCount();
                output.startAddress = fixture.getDmxStartAddress();
                fixture.getDmxData(output.channeldata);
            }

            // send last message
            //   m_outputQueue.Enqueue(&output);
        }

        m_outputQueue.Enqueue(&output, 10);
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

bool EffectsController::isEffectOverlapping() { return false; }

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

bool EffectsController::listCompareFixtureLocation(const RGBFixture a, const RGBFixture b)
{
    bool retVal = false;

    if (a.getXLocation() < b.getXLocation())
    {
        retVal = true;
    }
    return retVal;
}

void EffectsController::applyTestEffect(bool selectEffect)
{
    m_testEffectCounter++;
    if (selectEffect)
    {
        // test pattern 1
        // cycle R-G-B-W

        uint8_t colorID = (m_testEffectCounter >> 3) % 4;

        Color color;

        switch (colorID)
        {
        case 0:
            color.setRed(255);
            color.setGreen(0);
            color.setBlue(0);
            break;
        case 1:
            color.setRed(0);
            color.setGreen(255);
            color.setBlue(0);
            break;
        case 2:
            color.setRed(0);
            color.setGreen(0);
            color.setBlue(255);
            break;
        case 3:
        default:
            color.setRed(255);
            color.setGreen(255);
            color.setBlue(255);
            break;
        }

        for (auto& fixture : m_fixtures)
        {
            fixture.setBrightness(255);
            fixture.setColor(color);
        }
    }
    else
    {
        // test pattern 2
        uint8_t fixtureID = (m_testEffectCounter >> 3) % m_fixtures.size();

        Color color(255, 255, 255);

        // turn all fixtures off
        for (auto& fixture : m_fixtures)
        {
            fixture.setBrightness(0);
        }

        // turn selected fixture on
        std::list<RGBFixture>::iterator it = m_fixtures.begin();
        std::advance(it, fixtureID);

        it->setBrightness(255);
        it->setColor(color);
    }
}

void EffectsController::setMode(EffectMode mode) { m_effectMode = mode; }

EffectsController::EffectMode EffectsController::getMode() { return m_effectMode; }
