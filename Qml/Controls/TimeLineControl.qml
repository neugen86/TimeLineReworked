import QtQuick 2.0
import QtQuick.Controls 2.2

import Qml.Controls 0.1

import Cpp.Controls.TimeLine 0.1

TimeScaleControl {
    id: scale

    BusyIndicator {
        anchors.fill: scale
        visible: time_line.busy
    }

    TimeLine {
        id: time_line

        viewportWidth: scale.width
        viewportOffset: scale.offset
        contentWidth: scale.contentWidth

        onIntervalChanged: {
            scale.setInterval(interval)
        }
    }

    Repeater {
        model: TimeLineItemModel {
            items: time_line.visibleItems
        }

        delegate: Rectangle {
            x: model.start
            y: 10
            width: model.width
            height: 50

            color: model.color

            border {
                color: mouse_area.containsMouse ? "red" : "gray"
                width: mouse_area.containsMouse ? 2 : 1
            }

            Text {
                anchors.fill: parent

                text: model.title
                elide: Qt.ElideRight
            }

            MouseArea {
                id: mouse_area

                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip.visible: mouse_area.containsMouse
            ToolTip.text: model.description
        }
    }

    function simulate(itemsCount) { time_line.simulate(itemsCount) }
}
