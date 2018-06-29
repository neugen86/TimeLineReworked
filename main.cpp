#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "Cpp/Controls/Module.h"

void registerQmlTypes()
{
    ControlsModule::registerQmlTypes();
}

int main(int argc, char *argv[])
{
    registerQmlTypes();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.addImportPath(QStringLiteral("qrc:/"));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
