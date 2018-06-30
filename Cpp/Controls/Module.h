#include "Common.h"
#include "TimeLine/Module.h"
#include "TimeScale/Module.h"

namespace ControlsModule
{
void registerQmlTypes()
{
    qRegisterMetaType<TimeInterval>("TimeInterval");
    qRegisterMetaType<TimeLineItemList>("TimeLineItemList");

    TimeLineModule::registerQmlTypes();
    TimeScaleModule::registerQmlTypes();
}

} // namespace ControlsModule
