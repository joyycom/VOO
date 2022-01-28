import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQml 2.12
import FramelessWindow 1.0
import "../widget"

FramelessWindow {
    id: loginDialog
    modality: Qt.ApplicationModal
    width: 400
    height: 430
    resizable: false
    allowTouchWidget: [loginBgImage, loginLogoImage]
    Timer {
        id: timer
        interval: 5000
        running: false
        repeat: false
        onTriggered: {
            tipLabel.visible = false
        }
    }
    Connections {
        target: loginController
        onShowMessage: {
            showMessage(msg)
        }
        onLoginSuccess: {
            close()
        }
    }
    function showMessage(msg) {
        loginButton.enabled = true
        tipLabel.text = msg
        tipLabel.visible = true
        timer.start()
    }
    Rectangle {
        color: "#15171C"
        radius: 4
        border.color: "#000000"
        border.width: 2
        anchors.fill: parent
        Image {
            id: loginBgImage
            anchors.fill: parent
            source: "qrc:/image/assets/login_bg.png"
        }
        TitleBar {
            width: 40
            height: 40
            anchors.right: parent.right
            onCloseClicked: {
                loginDialog.close()
            }
        }
        Image {
            id: loginLogoImage
            width: 108
            height: 43
            sourceSize.width: width
            sourceSize.height: height
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 30
            source: "qrc:/image/assets/login_logo.png"
        }
        Label {
            id: accountLabel
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 120
            anchors.leftMargin: 45
            text: qsTr("Username")
            font.pixelSize: 12
            color: "#747474"
        }
        TextField {
            id: accountEdit
            implicitWidth: 308
            implicitHeight: 36
            leftPadding: 10
            anchors.left: accountLabel.left
            anchors.top: accountLabel.bottom
            anchors.topMargin: 10
            font.pixelSize: 12
            color: "#BFBFBF"
            selectionColor: "#5465FF"
            selectedTextColor: "#FFFFFF"
            focus: true
            selectByMouse: true
            verticalAlignment: TextEdit.AlignVCenter
            placeholderText: qsTr("Phone/Email")
            background: Rectangle {
                color: "#2D2F33"
                radius: 2
            }
        }
        Label {
            id: passwordLabel
            anchors.left: accountLabel.left
            anchors.top: accountEdit.bottom
            anchors.topMargin: 15
            text: qsTr("Password")
            font.pixelSize: 12
            color: "#747474"
        }
        TextField {
            id: passwordEdit
            implicitWidth: 308
            implicitHeight: 36
            leftPadding: 10
            anchors.left: accountLabel.left
            anchors.top: passwordLabel.bottom
            anchors.topMargin: 10
            font.pixelSize: 12
            color: "#BFBFBF"
            selectionColor: "#5465FF"
            selectedTextColor: "#FFFFFF"
            selectByMouse: true
            passwordCharacter: "*"
            echoMode: TextField.Password
            verticalAlignment: TextEdit.AlignVCenter
            placeholderText: qsTr("Please enter your password")
            background: Rectangle {
                color: "#2D2F33"
                radius: 2
            }

            onAccepted: {
                loginButton.clicked()
            }
        }
        Label {
            id: tipLabel
            anchors.left: accountLabel.left
            anchors.top: passwordEdit.bottom
            anchors.topMargin: 5
            font.pixelSize: 12
            color: "#EB291C"
            visible: false
        }
        Button {
            id: loginButton
            width: 308
            height: 40
            anchors.top: passwordEdit.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 70
            text: qsTr("Login")
            style: 1
            onClicked: {
                var account = accountEdit.text
                var password = passwordEdit.text
                if (account === "") {
                    showMessage(qsTr("Username can not be empty"))
                    return
                }
                if (password === "") {
                    showMessage(qsTr("Password can not be empty"))
                    return
                }
                loginButton.enabled = false
                loginController.requestLogin(account, password)
            }
        }
        Item {
            width: regLabel.width + forgetLabel.width + line.width
                   + line.anchors.leftMargin + forgetLabel.anchors.leftMargin
            height: 18
            anchors.top: loginButton.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 15
            Label {
                id: regLabel
                anchors.left: parent.left
                anchors.topMargin: 15
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Create your Account")
                font.pixelSize: 12
                color: "#BFBFBF"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        Qt.openUrlExternally(loginController.getRegUrl())
                    }
                }
            }
            Rectangle {
                id: line
                width: 1
                height: 10
                anchors.left: regLabel.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                color: "#FFFFFF"
                opacity: 0.1
            }
            Label {
                id: forgetLabel
                anchors.left: line.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Find your Password")
                font.pixelSize: 12
                color: "#BFBFBF"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        Qt.openUrlExternally(loginController.getForgetUrl())
                    }
                }
            }
        }
    }
}
