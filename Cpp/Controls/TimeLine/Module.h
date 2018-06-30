#include <QtQml/QQmlEngine>

#include "TimeLine.h"
#include "TimeLineItemModel.h"

namespace ControlsModule
{
namespace TimeLineModule
{
void registerQmlTypes()
{
    qmlRegisterType<TimeLine>("Cpp.Controls.TimeLine", 0, 1, "TimeLine");
    qmlRegisterType<TimeLineItemModel>("Cpp.Controls.TimeLine", 0, 1, "TimeLineItemModel");
}

} // namespace TimeLineModule
} // namespace ControlsModule

