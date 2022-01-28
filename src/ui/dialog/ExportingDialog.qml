import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.0
import ExportDialogDataSource 1.0
import "../widget"

Rectangle {
    property var savePath
    property var videoName
    property var coverType
    property var exportTemplate
    property var inputDeviceList
    property var doContribute
    property var templateCnName
    property var templateEnName
    property var templateType
    property var templateTeam
    property var templateOverwrite
    property var packSize
    property var e_commerceCfgPath

    //0是导出，1是投稿
    property int mode: 0
    property int progress: 0
    property string name: datasource.getTitle()
    property string rate: datasource.getBitRate()
    property int frameRate: datasource.getFps()
    property string resolution: datasource.getFrame()
    function startExport() {
        datasource.startExport(savePath, videoName, coverType, exportTemplate,
                               doContribute, inputDeviceList, templateCnName,
                               templateEnName, templateType, templateTeam,
                               templateOverwrite, e_commerceCfgPath)
    }
    id: exportingDialog
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
    Rectangle {
        height: img.height * (100 - progress) / 100
        anchors.left: img.left
        anchors.right: img.right
        anchors.bottom: img.bottom
        color: "#70000000"
        visible: mode === 0 && img.visible
    }
    Label {
        id: progressingLbl
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.left: img.right
        anchors.leftMargin: 44
        font.pixelSize: 18
        color: "#FFFFFF"
        text: qsTr(mode === 0 ? qsTr("Exporting... ") : qsTr(
                                    "Contributed...")) + progress.toString(
                  ) + "%"
    }

    Rectangle {
        width: 276
        height: 1
        anchors.top: progressingLbl.bottom
        anchors.topMargin: 15
        anchors.left: progressingLbl.left
        color: "#10FFFFFF"
    }

    Label {
        id: videoNameLabel
        width: 60
        height: 16
        anchors.top: progressingLbl.bottom
        anchors.topMargin: 33
        anchors.left: progressingLbl.left
        font.pixelSize: 12
        color: "#747474"
        text: qsTr("Video Name: ")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Label {
        anchors.left: videoNameLabel.right
        anchors.leftMargin: 20
        anchors.verticalCenter: videoNameLabel.verticalCenter
        font.pixelSize: 12
        color: "#747474"
        text: name
    }

    Label {
        id: videoTime
        width: 60
        height: 16
        anchors.top: videoNameLabel.bottom
        anchors.topMargin: 14
        anchors.left: progressingLbl.left
        font.pixelSize: 12
        color: "#747474"
        text: qsTr("Video Duration: ")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Label {
        anchors.left: videoTime.right
        anchors.leftMargin: 20
        anchors.verticalCenter: videoTime.verticalCenter
        font.pixelSize: 12
        color: "#747474"
        text: videoDuration.toString() + " s"
    }

    Label {
        id: videoSize
        width: 60
        height: 16
        anchors.top: videoTime.bottom
        anchors.topMargin: 14
        anchors.left: progressingLbl.left
        font.pixelSize: 12
        color: "#747474"
        text: qsTr("Estimated Size: ")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Label {
        anchors.left: videoSize.right
        anchors.leftMargin: 20
        anchors.verticalCenter: videoSize.verticalCenter
        font.pixelSize: 12
        color: "#747474"
        text: packSize
    }

    Label {
        id: videoRate
        width: 60
        height: 16
        anchors.top: videoSize.bottom
        anchors.topMargin: 14
        anchors.left: progressingLbl.left
        font.pixelSize: 12
        color: "#747474"
        text: qsTr("Bitrate: ")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Label {
        anchors.left: videoRate.right
        anchors.leftMargin: 20
        anchors.verticalCenter: videoRate.verticalCenter
        font.pixelSize: 12
        color: "#747474"
        text: rate
    }

    Label {
        id: videoFrameRate
        width: 60
        height: 16
        anchors.top: videoRate.bottom
        anchors.topMargin: 14
        anchors.left: progressingLbl.left
        font.pixelSize: 12
        color: "#747474"
        text: qsTr("Frame Rate: ")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Label {
        anchors.left: videoFrameRate.right
        anchors.leftMargin: 20
        anchors.verticalCenter: videoFrameRate.verticalCenter
        font.pixelSize: 12
        color: "#747474"
        text: frameRate.toString() + "fps"
    }

    Label {
        id: videoResolution
        width: 60
        height: 16
        anchors.top: videoFrameRate.bottom
        anchors.topMargin: 14
        anchors.left: progressingLbl.left
        font.pixelSize: 12
        color: "#747474"
        text: qsTr("Resolution: ")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Label {
        anchors.left: videoResolution.right
        anchors.leftMargin: 20
        anchors.verticalCenter: videoResolution.verticalCenter
        font.pixelSize: 12
        color: "#747474"
        text: resolution
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

        Button {
            id: cancelBtn
            width: 70
            height: 26
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 22
            anchors.bottomMargin: 11
            text: qsTr("Cancel")
            onClicked: {
                datasource.cancel()
                exportDialog.close()
            }
        }
    }
}
