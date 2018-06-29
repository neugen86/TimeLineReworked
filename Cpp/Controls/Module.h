#include "TimeLine/Module.h"
#include "TimeScale/Module.h"

namespace ControlsModule
{
void registerQmlTypes()
{
    TimeLineModule::registerQmlTypes();
    TimeScaleModule::registerQmlTypes();
}

} // namespace ControlsModule
