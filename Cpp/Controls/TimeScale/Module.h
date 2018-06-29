#include <QtQml/QQmlEngine>

#include "ScaleItem.h"
#include "ScaleModel.h"

namespace ControlsModule
{
namespace TimeScaleModule
{
void registerQmlTypes()
{
    qmlRegisterType<ScaleItem>("Cpp.Controls.TimeScale", 0, 1, "ScaleItem");
    qmlRegisterType<ScaleModel>("Cpp.Controls.TimeScale", 0, 1, "ScaleModel");
}

} // namespace TimeScaleModule
} // namespace ControlsModule

