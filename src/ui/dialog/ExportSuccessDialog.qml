import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.0
import ExportDialogDataSource 1.0
import QmlUtils 1.0
import "../widget"

Rectangle {
    property var outputDir
    id: exportSuccessDialog
    color: "#15171C"
    radius: 4
    border.color: "#000000"
    border.width: 2

    Image {
        id: img
        width: 220
        height: 220
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 30
        anchors.topMargin: 24
        visible: previewCoverPath !== null
        fillMode: Image.PreserveAspectCrop
        source: "file:/" + previewCoverPath
    }
    Rectangle {
        anchors.fill: img
        visible: !img.visible
        color: Qt.platform.os === "osx" ? "#191B21" : "transparent"
        LinearGradient {
            anchors.fill: parent
            start: Qt.point(parent.width, 0)
            end: Qt.point(0, parent.height)
            visible: Qt.platform.os !== "osx"
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#0A0B0E"
                }
                GradientStop {
                    position: 1
                    color: "#191B21"
                }
            }
        }
        Image {
            id: emptyImage
            width: 80
            height: 80
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 56
            source: "qrc:/image/assets/export_empty_cover.png"
        }
        Label {
            anchors.top: emptyImage.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 14
            font.pixelSize: 14
            color: "#3B3E47"
            text: qsTr("No Cover")
        }
    }

    Image {
        id: successfulImg
        width: 48
        height: 48
        anchors.top: parent.top
        anchors.topMargin: 55
        anchors.left: img.right
        anchors.leftMargin: 158
        source: "qrc:/image/assets/exported_success.png"
    }
    Label {
        text: qsTr("Export Successfully")
        font.pixelSize: 18
        font.bold: true
        color: "#FFFFFF"
        anchors.top: successfulImg.bottom
        anchors.topMargin: 15
        anchors.horizontalCenter: successfulImg.horizontalCenter
    }
    Label {
        text: qsTr("It has been saved on your computer, go check it out now!")
        font.pixelSize: 12
        color: "#747474"
        anchors.top: successfulImg.bottom
        anchors.topMargin: 49
        anchors.horizontalCenter: successfulImg.horizontalCenter
    }

    Button {
        id: openBtn
        width: 70
        height: 26
        anchors.right: parent.right
        anchors.bottom: bottomRect.top
        anchors.rightMargin: 95
        anchors.bottomMargin: 60
        text: qsTr("Open")
        onClicked: {
            fileUtils.openDir(outputDir)
            close()
        }
    }

    Button {
        id: completeBtn
        width: 70
        height: 26
        anchors.right: openBtn.left
        anchors.rightMargin: 10
        anchors.verticalCenter: openBtn.verticalCenter
        text: qsTr("Finish")
        onClicked: {
            close()
        }
    }

    Rectangle {
        id: bottomRect
        width: parent.width - 4
        height: 48
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: 2
        anchors.leftMargin: 2
        color: "#212328"
    }
}
