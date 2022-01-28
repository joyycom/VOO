import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    color: "transparent"
    property bool maxEnabled: false
    property bool minEnabled: false
    property Item item: minEnabled ? minButton : maxEnabled ? maxButton : closeButton
    signal closeClicked
    signal maxClicked
    signal minClicked
    Button {
        id: closeButton
        width: 20
        height: 20
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        background: Rectangle {
            anchors.fill: parent
            color: closeButton.hovered ? "#FF0F36" : "transparent"
            radius: 2
        }
        Image {
            anchors.fill: parent
            source: closeButton.hovered ? "qrc:/image/assets/dialog_close_hovered.png" : "qrc:/image/assets/dialog_close.png"
        }
        onClicked: {
            closeClicked()
        }
    }
    Button {
        id: maxButton
        width: maxEnabled ? 20 : 0
        height: 20
        anchors.right: closeButton.left
        anchors.verticalCenter: closeButton.verticalCenter
        anchors.rightMargin: maxEnabled ? 15 : 0
        background: Rectangle {
            anchors.fill: parent
            color: maxButton.hovered ? "#1AFFFFFF" : "transparent"
            radius: 2
        }
        Image {
            anchors.fill: parent
            source: "qrc:/image/assets/dialog_max.png"
        }
        onClicked: {
            maxClicked()
        }
    }
    Button {
        id: minButton
        width: minEnabled ? 20 : 0
        height: 20
        anchors.right: maxButton.left
        anchors.verticalCenter: maxButton.verticalCenter
        anchors.rightMargin: minEnabled ? 15 : 0
        background: Rectangle {
            anchors.fill: parent
            color: minButton.hovered ? "#1AFFFFFF" : "transparent"
            radius: 2
        }
        Image {
            anchors.fill: parent
            source: "qrc:/image/assets/dialog_min.png"
        }
        onClicked: {
            minClicked()
        }
    }
}
