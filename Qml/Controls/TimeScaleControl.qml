import QtQuick 2.0
import QtQuick.Controls 2.2

import Cpp.Controls.TimeScale 0.1

Item {
    id: control

    ListView {
        id: list

        width: control.width
        height: scale_model.height

        anchors.centerIn: control

        orientation: Qt.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.HorizontalFlick

        model: ScaleModel {
            id: scale_model
            minWidth: list.width
        }

        delegate: ScaleItem {
            data: image
        }

        MouseArea {
            anchors.fill: list

            onWheel: {
                var delta = wheel.angleDelta.y

                if (delta > 0) {
                    zoomIn()
                }
                else if (delta < 0) {
                    zoomOut()
                }
            }
        }

        ScrollBar.horizontal: ScrollBar {
            id: scroll_bar

            minimumSize: 0.03
            implicitHeight: 25

            policy: ScrollBar.AlwaysOn

            contentItem: Rectangle {
                radius: scroll_bar.implicitHeight / 2
                color: "gray"
            }

            visible: list.count
        }
    }

    function zoomIn() { scale_model.zoomIn() }
    function zoomOut() { scale_model.zoomOut() }
    function resetZoom() { scale_model.resetZoom() }
}
