/*************************************************
 * @file BSP.h
 * @brief Board Specific Package
 *
 *************************************************/

#include "stdint.h"

//#include "OSError.h"
//#include "OSTask.h"

#include "thread.hpp"

/*
 * OS task wrapper
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

    //    static OSError* m_osError;
};
