import QtQuick 2.12

Item {
    id: itemRoot
    width: fill ? 96 : 84
    height: fill ? 100 : 84
    x: fill ? 100000000 : 0
    y: fill ? 100000000 : 0
    property alias backgroundColor: frame3.color
    property alias thumb: thumb
    property alias extension: extensionText.text
    property Item capture: !single ? this : frame3
    property int selectCount: 0
    property bool isMouseSelected: false
    property bool single: selectCount <= 1
    property bool saving: false
    property bool fill: !single && saving
    Rectangle {
        id: frame1
        width: 84
        height: 84
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 4
        border.color: "#0C7E79"
        border.width: 1.5
        color: "#000000"
        visible: fill
    }
    Rectangle {
        id: frame2
        width: 84
        height: 84
        anchors.right: frame1.right
        anchors.bottom: frame1.bottom
        anchors.bottomMargin: 4
        anchors.rightMargin: 4
        border.color: "#06B1A8"
        border.width: 1.5
        color: "#000000"
        visible: fill
    }
    Rectangle {
        id: frame3
        width: 84
        height: 84
        anchors.top: parent.top
        anchors.topMargin: fill ? 8 : 0
        color: "transparent"
        property alias saving: itemRoot.saving
        Image {
            id: thumb
            width: 80
            height: 80
            fillMode: Image.PreserveAspectCrop
            anchors.centerIn: parent
        }
        Rectangle {
            id: shadow
            height: 20
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: "#00000000"
                }
                GradientStop {
                    position: 1.0
                    color: "#80000000"
                }
            }
        }
        Text {
            id: extensionText
            anchors.fill: shadow
            color: "#ffffff"
            anchors.right: shadow.right
            anchors.rightMargin: 4
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignRight
        }

        Rectangle {
            anchors.fill: parent
            border.color: "#02E4D6"
            border.width: (isMouseSelected || saving) ? 1.5 : 0
            color: "transparent"
        }
    }
    Rectangle {
        width: 20
        height: 20
        radius: 20
        anchors.top: parent.top
        anchors.right: parent.right
        color: "#02E4D6"
        visible: fill
        Text {
            anchors.centerIn: parent
            text: selectCount
            color: "#000000"
            font.pixelSize: 12
        }
    }
}
