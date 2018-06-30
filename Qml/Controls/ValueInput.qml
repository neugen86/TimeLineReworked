import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

RowLayout {
    spacing: 5

    property int pixelSize: 18

    property alias title: title.text
    property alias value: value.text

    signal changed()

    Text {
        id: title
        font.pixelSize: pixelSize
    }

    Rectangle {
        height: 40
        width: 150

        border.width: 1

        TextInput {
            id: value

            anchors.centerIn: parent

            maximumLength: 10
            font.pixelSize: pixelSize

            focus: true
        }
    }

    Button {
        text: "Ok"
        onPressed: changed()
    }
}

