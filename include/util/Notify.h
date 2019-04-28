/*************************************************
 * @file Notify.h
 * @brief Notify
 *
 *************************************************/
#pragma once

#include <string>
#include <vector>
#include "stdint.h"

/**
 * Notify
 */
template <class T>
class Notify
{
public:
    typedef void (*subscribeCallback)(T, void* context);

    void subscribe(subscribeCallback function, void* context);

protected:
    typedef std::pair<subscribeCallback, void*> CallBackData;
    std::vector<CallBackData>                   m_callbackFunction;

    void notifySubscribers(T);
};

template class Notify<bool>;
