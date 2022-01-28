import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    color: "#202020"
    ListView {
        anchors.fill: parent
        orientation: ListView.Horizontal
        interactive: false
        model: 100000000
        delegate: Image {
            width: 28
            height: parent.height
            source: "image://skyimage/" + resource + "?type=2&width=" + width + "&height=" + height
            fillMode: Image.PreserveAspectCrop
            x: -pausePosition
        }
    }
    TextField {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 2
        anchors.leftMargin: 2 - pausePosition
        color: "#FFFFFF"
        font.pixelSize: 8
        text: (parent.width / timeValue).toFixed(2).toString() + "s"
        leftPadding: 2
        rightPadding: 2
        topPadding: 0
        bottomPadding: 0
        enabled: false
        background: Rectangle {
            color: "#66000000"
            radius: 1
        }
    }
}
