import QtQuick 2.12
import "../basicmenubar"
import QtQuick.Controls 2.12

BasicMenu {
    background: Item {
        implicitWidth: 83
        implicitHeight: 32
    }
    Rectangle {
        width: 83
        height: 32
        color: "#2D2F33"
        radius: 2
        Rectangle {
            id: bg
            width: 75
            height: 22
            anchors.centerIn: parent
            color: "#0DFFFFFF"
            radius: 2
            visible: false
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                bg.visible = true
            }
            onExited: {
                bg.visible = false
            }
        }
        Image {
            id: logoutImage
            width: 16
            height: 16
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            anchors.leftMargin: 19
            source: "qrc:/image/assets/login_logout.png"
        }
        Label {
            id: logoutLabel
            anchors.left: logoutImage.right
            anchors.verticalCenter: logoutImage.verticalCenter
            anchors.leftMargin: 5
            text: qsTr("Log Out")
            font.pixelSize: 12
            color: "white"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                close()
                loginController.requestLogout()
            }
        }
    }
}
