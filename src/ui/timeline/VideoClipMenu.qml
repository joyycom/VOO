import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QmlUtils 1.0
import "../basicmenubar"

BasicMenu {
    property bool selectCurrent: true
    property real clipDuration: 0
    id: menu
    itemSpacing: 2
    topPadding: 5
    bottomPadding: 5
    background: Rectangle {
        implicitWidth: Math.max(text1.width, text2.width, text3.width,
                                text4.width, text5.width) + 20
        color: "#2D2F33"
    }
    BasicMenuItem {
        height: 26
        buttonColor: highlighted ? "#0DFFFFFF" : "#2D2F33"
        background: Rectangle {
            x: 5
            width: parent.width - x * 2
            height: parent.height
            color: parent.buttonColor
        }
        contentItem: Item {
            Text {
                id: text1
                x: 10
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Copy Clip")
                font.pixelSize: 12
                color: "#BFBFBF"
            }
        }
        onTriggered: {
            timelineController.copyClip()
        }
    }
    BasicMenuItem {
        height: 26
        enabled: selectCurrent
        buttonColor: highlighted && selectCurrent ? "#0DFFFFFF" : "#2D2F33"
        background: Rectangle {
            x: 5
            width: parent.width - x * 2
            height: parent.height
            color: parent.buttonColor
        }
        contentItem: Item {
            Text {
                id: text2
                x: 10
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Copy the current frame of the clip")
                font.pixelSize: 12
                color: "#BFBFBF"
                opacity: selectCurrent ? 1 : 0.5
            }
        }
        onTriggered: {
            timelineController.copyCurrentFrameFromClip()
        }
    }
    BasicMenuItem {
        height: 26
        buttonColor: highlighted ? "#0DFFFFFF" : "#2D2F33"
        background: Rectangle {
            x: 5
            width: parent.width - x * 2
            height: parent.height
            color: parent.buttonColor
        }
        contentItem: Item {
            Text {
                id: text3
                x: 10
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Copy the first frame of the clip")
                font.pixelSize: 12
                color: "#BFBFBF"
            }
        }
        onTriggered: {
            timelineController.copyFirstFrameFromClip()
        }
    }
    BasicMenuItem {
        height: 26
        buttonColor: highlighted ? "#0DFFFFFF" : "#2D2F33"
        background: Rectangle {
            x: 5
            width: parent.width - x * 2
            height: parent.height
            color: parent.buttonColor
        }
        contentItem: Item {
            Text {
                id: text4
                x: 10
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Copy the last frame of the clip")
                font.pixelSize: 12
                color: "#BFBFBF"
            }
        }
        onTriggered: {
            timelineController.copyEndFrameFromClip()
        }
    }
    BasicMenuItem {
        height: 26
        buttonColor: highlighted ? "#0DFFFFFF" : "#2D2F33"
        background: Rectangle {
            x: 5
            width: parent.width - x * 2
            height: parent.height
            color: parent.buttonColor
        }
        contentItem: Item {
            Text {
                id: text5
                x: 10
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Replace Clip")
                font.pixelSize: 12
                color: "#BFBFBF"
            }
        }
        onTriggered: {
            fileDialog.open()
        }
        FileDialog {
            id: fileDialog
            title: qsTr("Select an Video")
            selectMultiple: false
            nameFilters: ["Media files (*.mp4 *.avi *.m4v *.mkv *.mpg *.f4v *.flv *.rmvb *.mov *.rm *.wmv *.3gp *.png *.jpeg *.jpg *.webp *.bmp *.tiff *.gif)"]
            onAccepted: {
                var path = fileUtils.urlToFile(fileUrl)
                var isVideo = fileUtils.isVideoExtension(path)
                var duration = fileUtils.getMediaDuration(path)
                if (isVideo && duration < clipDuration) {
                    timelineToastMgr.show(
                                qsTr("The current video is shorter than the original clip, so replacement is not supported"),
                                3000)
                    return
                }
                var component = Qt.createComponent(
                            "qrc:/ui/dialog/UpdateResourceDialog.qml")
                var page = component.createObject(editorPage)
                page.resPath = path
                page.isVideo = isVideo
                page.clipDuration = clipDuration
                page.resDuration = duration
                page.show()
            }
        }
        FileUtils {
            id: fileUtils
        }
    }
}
