/*************************************************
 * @file BSP.h
 * @brief Board Specific Package
 *
 *************************************************/

#include "stdint.h"

#include "thread.hpp"

/*
 * BSP
 */
class BSP : public cpp_freertos::Thread
{
public:
    BSP(const char* name);
    ~BSP();

    void Run();

private:
    // Force empty constructor to not be used
    BSP();
};
