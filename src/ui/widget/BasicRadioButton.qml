import QtQuick 2.12
import QtQuick.Templates 2.12 as T
import QtQuick.Controls 2.12

T.RadioButton {
    id: control

    property color textColor: "#FFFFFF"
    property color indicatorColor: checked ? "#5465FF" : "#BFBFBF"
    property var customIndicator: Rectangle {
        implicitWidth: 10
        implicitHeight: 10
        radius: 44
        border.color: indicatorColor
        border.width: 1
        color: "transparent"
        anchors.verticalCenter: customContentItem.verticalCenter
        Rectangle {
            width: 4
            height: 4
            anchors.centerIn: parent
            radius: 23
            color: indicatorColor
            visible: control.checked
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                control.checked = true
            }
        }
    }
    property var customContentItem: Text {
        verticalAlignment: Text.AlignVCenter
        text: control.text
        color: textColor
        font.pixelSize: 12
        anchors.left: customIndicator.right
        anchors.leftMargin: 6
    }

    implicitWidth: indicator.implicitWidth + contentItem.implicitWidth
                   + contentItem.anchors.leftMargin
    implicitHeight: Math.max(indicator.implicitHeight,
                             contentItem.implicitHeight)

    indicator: customIndicator
    contentItem: customContentItem
}
