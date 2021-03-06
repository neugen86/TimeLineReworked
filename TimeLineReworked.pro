QT += quick
CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = .

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Cpp/Backend.h \
    Cpp/Controls/Common.h \
    Cpp/Controls/Module.h \
    Cpp/Controls/TimeUtils.h \
    Cpp/Controls/TimeScale/Module.h \
    Cpp/Controls/TimeScale/TimeScaleItem.h \
    Cpp/Controls/TimeScale/TimeScaleModel.h \
    Cpp/Controls/TimeScale/ScaleTilesFactory.h \
    Cpp/Controls/TimeLine/Module.h \
    Cpp/Controls/TimeLine/TimeLine.h \
    Cpp/Controls/TimeLine/TimeLineItemModel.h \
    Cpp/Controls/TimeLine/TimeLineUtils.h


SOURCES += \
    main.cpp \
    Cpp/Backend.cpp \
    Cpp/Controls/TimeUtils.cpp \
    Cpp/Controls/TimeScale/TimeScaleItem.cpp \
    Cpp/Controls/TimeScale/TimeScaleModel.cpp \
    Cpp/Controls/TimeScale/ScaleTilesFactory.cpp \
    Cpp/Controls/TimeLine/TimeLine.cpp \
    Cpp/Controls/TimeLine/TimeLineItemModel.cpp \
    Cpp/Controls/TimeLine/TimeLineUtils.cpp

