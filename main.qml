import QtQuick 2.11
import QtQuick.Window 2.11

import Qml.Controls 0.1

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    TimeLineControl {
        anchors.fill: parent
    }
}
