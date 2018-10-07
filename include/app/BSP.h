/*************************************************
 * @file BSP.h
 * @brief Board Specific Package
 *
 *************************************************/

#include "stdint.h"

#include "OSTask.h"

/*
 * OS task wrapper
 */
class BSP : public OSTask
{
public:
    BSP(const char* name);
    ~BSP();

    void task();

private:
    // Force empty constructor to not be used
    BSP();
};
