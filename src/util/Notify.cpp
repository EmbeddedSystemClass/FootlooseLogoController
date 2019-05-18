/*************************************************
 * @file Notify.cpp
 * @brief Notify
 *
 *************************************************/
#include "util/Notify.h"

#include <string>
#include <vector>
#include "stdint.h"

template <typename T>
void Notify<T>::notifySubscribers(T val)
{
    for (CallBackData call : m_callbackFunction)
    {
        call.first(val, call.second);
    }
}

template <typename T>
void Notify<T>::subscribe(subscribeCallback function, void* context)
{
    m_callbackFunction.push_back(CallBackData(function, context));
}
