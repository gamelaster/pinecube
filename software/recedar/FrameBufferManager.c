#include "FrameBufferManager.h"
#include "recedar_hooks.h"

// -RC status=not-needed-yet
FrameBufferManager* FrameBufferManagerCreate(int num)
{
    return _FrameBufferManagerCreate(num);
}