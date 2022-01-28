import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import FramelessWindow 1.0
import "../widget"

FramelessWindow {
    property bool force
    property var version
    property var desc
    property var downloadUrl
    modality: Qt.ApplicationModal
    width: 334
    height: titleRect.height + contentColumn.height
    resizable: false
    onlyAllowListMovable: true
    allowTouchWidget: [titleRect, dialogTitle]
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
                close()
            }
            Label {
                id: dialogTitle
                text: qsTr("Update found")
                font.pixelSize: 12
                color: "#BFBFBF"
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        ColumnLayout {
            id: contentColumn
            width: parent.width
            anchors.top: titleRect.bottom
            Label {
                id: versionLabel
                text: qsTr("A new version is found to be updated:") + version
                font.pixelSize: 14
                color: "#BFBFBF"
                Layout.leftMargin: 25
                Layout.topMargin: 20
            }
            TextEdit {
                id: descLabel
                width: 294
                Layout.preferredWidth: width
                text: desc
                font.pixelSize: 12
                color: "#FFFFFF"
                selectByMouse: true
                wrapMode: "WrapAtWordBoundaryOrAnywhere"
                Layout.leftMargin: 25
                Layout.topMargin: 10
                readOnly: true
            }
            Row {
                id: buttonRow
                height: 26
                spacing: 20
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 10
                Layout.bottomMargin: 20
                Button {
                    id: cancelBtn
                    width: 80
                    height: 26
                    text: qsTr(force ? qsTr("Exit application") : qsTr(
                                           "Not now"))
                    onClicked: {
                        close()
                        if (force) {
                            editorPage.close()
                            editorPage.exitApp()
                        }
                    }
                }
                Button {
                    id: okButton
                    width: 80
                    height: 26
                    text: qsTr("Update Now")
                    style: 1
                    onClicked: {
                        if (!force) {
                            close()
                        }
                        Qt.openUrlExternally(downloadUrl)
                    }
                }
            }
        }
    }
}
