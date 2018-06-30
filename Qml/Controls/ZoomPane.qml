import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

ColumnLayout {
    spacing: 10

    signal zoomIn()
    signal zoomOut()
    signal zoomToFill()

    property int pixelSize: 18

    Button {
        text: "+"
        font.pixelSize: pixelSize

        onClicked: zoomIn()
    }

    Button {
        text: "-"
        font.pixelSize: pixelSize

        onClicked: zoomOut()
    }

    Button {
        text: "[  ]"
        font.pixelSize: pixelSize

        onClicked: zoomToFill()
    }

    Item { Layout.fillHeight: true }
}
