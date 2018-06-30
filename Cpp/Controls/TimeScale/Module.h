#include <QtQml/QQmlEngine>

#include "TimeScaleItem.h"
#include "TimeScaleModel.h"

namespace ControlsModule
{
namespace TimeScaleModule
{
void registerQmlTypes()
{
    qmlRegisterType<TimeScaleItem>("Cpp.Controls.TimeScale", 0, 1, "TimeScaleItem");
    qmlRegisterType<TimeScaleModel>("Cpp.Controls.TimeScale", 0, 1, "TimeScaleModel");
}

} // namespace TimeScaleModule
} // namespace ControlsModule

