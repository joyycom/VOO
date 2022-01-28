import QtQuick 2.12
import "../basicmenubar"

BasicMenu {
    property bool selectCurrent: true
    id: menu
    itemSpacing: 2
    topPadding: 5
    bottomPadding: 5
    background: Rectangle {
        implicitWidth: text.width + 20
        color: "#2D2F33"
    }
    BasicMenuItem {
        height: 26
        buttonColor: highlighted ? "#0DFFFFFF" : "#2D2F33"
        background: Rectangle {
            x: 5
            width: parent.width - x * 2
            height: parent.height
            color: parent.buttonColor
        }
        contentItem: Item {
            Text {
                id: text
                x: 10
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Copy Clip")
                font.pixelSize: 12
                color: "#BFBFBF"
            }
        }
        onTriggered: {
            timelineController.copyClip()
        }
    }

    BasicMenuItem {
        height: 26
        buttonColor: highlighted ? "#0DFFFFFF" : "#2D2F33"
        background: Rectangle {
            x: 5
            width: parent.width - x * 2
            height: parent.height
            color: parent.buttonColor
        }
        contentItem: Item {
            Text {
                id: editEffect
                x: 10
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Edit Effect")
                font.pixelSize: 12
                color: "#BFBFBF"
            }
        }
        onTriggered: {
            timelineController.editEffect(trackIndex(),itemIndex())
        }
    }
}
