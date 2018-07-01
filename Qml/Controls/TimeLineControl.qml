import QtQuick 2.0
import QtQuick.Controls 2.2

import Qml.Controls 0.1

import Cpp.Controls.TimeLine 0.1

TimeScaleControl {
    id: scale

    BusyIndicator {
        anchors.fill: scale
        visible: time_line.loading
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
            width: model.width

            y: 10 - (hovered ? 2 : 0)
            height: 50 + (hovered ? 4 : 0)

            border {
                color: hovered ? "red" : "gray"
                width: hovered ? 2 : 1
            }

            color: model.color

            property alias hovered: mouse_area.containsMouse

            Text {
                anchors.fill: parent

                anchors.topMargin: 10
                anchors.leftMargin: 10

                text: model.title
                elide: Qt.ElideRight
            }

            MouseArea {
                id: mouse_area

                anchors.fill: parent
                hoverEnabled: true
            }

            ToolTip.text: model.description
            ToolTip.visible: hovered
        }
    }

    function simulate(itemsCount) { time_line.simulate(itemsCount) }
}
