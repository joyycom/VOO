import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import FramelessWindow 1.0
import QtQml 2.12
import "../widget"

FramelessWindow {
    id: root
    modality: Qt.ApplicationModal
    width: 334
    height: 267
    resizable: false
    onlyAllowListMovable: true
    allowTouchWidget: [titleRect, dialogTitle]
    Connections {
        id: connect
        target: editorDatasource
        onVersionUpdateResult: {
            if (update) {
                root.exit()
                var component = Qt.createComponent(
                            "qrc:/ui/dialog/UpdateFound.qml")
                var page = component.createObject(editorPage)
                page.force = force
                page.version = version
                page.desc = desc
                page.downloadUrl = downloadUrl
                page.show()
            } else {
                toastMgr.show(qsTr("You are currently the latest version"),
                              3000)
            }
        }
    }
    Rectangle {
        color: "#15171C"
        radius: 4
        border.color: "#000000"
        border.width: 2
        anchors.fill: parent
        TitleBar {
            id: titleRect
            width: parent.width - 4
            height: 42
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 2
            anchors.leftMargin: 2
            color: "#000000"
            onCloseClicked: {
                exit()
            }
            Label {
                id: dialogTitle
                text: qsTr("Check for update")
                font.pixelSize: 12
                color: "#BFBFBF"
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Image {
            id: logoImage
            width: 80
            height: 80
            sourceSize.width: width
            sourceSize.height: height
            source: "qrc:/image/assets/bg_logo.png"
            anchors.top: titleRect.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 26
        }
        Label {
            id: versionLabel
            anchors.top: logoImage.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 18
            text: qsTr(
                      "Current version number:") + editorDatasource.appVersion()
            font.pixelSize: 12
            color: "#BFBFBF"
        }
        Button {
            width: 258
            height: 36
            anchors.top: versionLabel.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 28
            text: qsTr("Check for update")
            onClicked: {
                editorDatasource.checkVersionUpdate()
            }
        }
    }
    ToastManager {
        id: toastMgr
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
    }
    function exit() {
        connect.destroy()
        close()
    }
}
