import QtQuick 2.0
import QtQuick.Controls 2.2

import Cpp.Controls.TimeScale 0.1

Item {
    id: item

    height: list.height

    property real contentX: controller.offset
    property alias contentWidth: controller.scaleWidth

    function zoomIn() { controller.zoomIn() }
    function zoomOut() { controller.zoomOut() }
    function resetZoom() { scale_model.resetZoom() }

    function setInterval(value) { scale_model.setInterval(value) }

    ListView {
        id: list

        width: item.width
        height: scale_model.height

        orientation: Qt.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.HorizontalFlick

        model: TimeScaleModel {
            id: scale_model

            minWidth: list.width

            onWidthChanged: {
                controller.setWidth(width)
            }
        }

        delegate: TimeScaleItem {
            data: image
        }

        MouseArea {
            anchors.fill: list

            onWheel: {
                var pos = wheel.x
                var delta = wheel.angleDelta.y

                if (delta > 0) {
                    controller.zoomIn(pos)
                }
                else if (delta < 0) {
                    controller.zoomOut(pos)
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

        onContentXChanged: {
            controller.setOffset(list.contentX)
        }
    }

    QtObject {
        id: controller

        property real offset: 1
        property real zoomCenter: 0
        property real scaleWidth: 0
        property bool offsetRestoring: false

        function setZoomCenter(center) {
            zoomCenter = (center !== undefined) ? center : (item.width / 2)
        }

        function setWidth(value) {
            var newOffset = offset

            if (offsetRestoring) {
                var prevPos = (offset + zoomCenter)
                var curPos = (prevPos * value / scaleWidth)

                newOffset = (curPos - zoomCenter)
                var maxOffset = (value - item.width);

                newOffset = Math.max(0, Math.min(maxOffset, newOffset));

                list.contentX = newOffset

                offsetRestoring = false
            }

            scaleWidth = value
            offset = newOffset
        }

        function setOffset(value) {
            if (!offsetRestoring) {
                offset = value
            }
        }

        function zoomIn(center) {
            offsetRestoring = true
            setZoomCenter(center)
            scale_model.zoomIn()
        }

        function zoomOut(center) {
            offsetRestoring = true
            setZoomCenter(center)
            scale_model.zoomOut()
        }
    }
}
