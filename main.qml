import QtQuick 2.11
import QtQuick.Window 2.11

import Qml.Controls 0.1

Window {
    width: 640
    height: 480

    visible: true

    title: qsTr("TimeLineRevorked")

    TimeLineControl {
        id: time_line

        y: (parent.height - height) / 2
        width: parent.width
    }

    ValueInputControl {
        anchors.top: parent.top
        anchors.left: parent.left

        value: "1000000"
        onChanged: time_line.simulate(value)
    }

    ZoomPaneControl {
        anchors.top: parent.top
        anchors.right: parent.right

        onZoomIn: time_line.zoomIn()
        onZoomOut: time_line.zoomOut()
        onZoomToFill: time_line.resetZoom()
    }
}
