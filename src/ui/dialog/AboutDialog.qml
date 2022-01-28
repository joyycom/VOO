import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import ProjectHistoryListModel 1.0
import GuideDataSource 1.0
import FramelessWindow 1.0
import "../widget"

FramelessWindow {
    property string appVersion: "v0.0.1"
    id: aboutPage
    objectName: "AboutPage"
    modality: Qt.ApplicationModal
    width: 300
    height: 200
    resizable: false
    onlyAllowListMovable: true
    allowTouchWidget: [titleRect, dialogTitle]
    color: "#15171C"
    title: qsTr("About")

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
                aboutPage.close()
            }
            Label {
                id: dialogTitle
                text: qsTr("About")
                font.pixelSize: 12
                color: "#BFBFBF"
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Image {
            width: 76
            height: 30
            source: "qrc:/image/assets/guide_logo.png"
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: titleRect.bottom
                topMargin: 20
            }
        }
        Text {
            id: version
            width: parent.width
            height: 20
            text: appVersion
            color: "#ffffff"
            font.pixelSize: 14
            anchors {
                bottom: parent.bottom
                bottomMargin: 20
            }
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
    Shortcut {
        sequence: "Esc"
        onActivated: close()
    }

    Shortcut {
        sequence: "Enter"
        onActivated: close()
    }
}
