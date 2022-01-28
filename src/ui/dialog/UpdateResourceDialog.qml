import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import FramelessWindow 1.0
import UpdateResourcePreviewPlayer 1.0
import "../widget"

FramelessWindow {
    property var resPath
    property bool isVideo: false
    property real clipDuration: 0
    property real resDuration: 0
    property real pixelTime: clipDuration / scrollRect.width
    property real startTime: pixelTime * (imageList.contentX + scrollRect.x)
    property real endTime: pixelTime * (imageList.contentX + scrollRect.x + scrollRect.width)
    property real currentTime: isVideo ? 0 : clipDuration
    modality: Qt.ApplicationModal
    width: 360
    height: 510
    resizable: false
    onlyAllowListMovable: true
    allowTouchWidget: [titleRect]
    onCurrentTimeChanged: {
        player.setTime(startTime + currentTime, startTime, endTime)
    }
    Rectangle {
        color: "#222425"
        radius: 4
        border.color: "#000000"
        border.width: 2
        anchors.fill: parent
        TitleBar {
            id: titleRect
            width: parent.width - 4
            height: 34
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 2
            anchors.leftMargin: 2
            color: "#000000"
            onCloseClicked: {
                close()
            }
        }
        Rectangle {
            id: previewRect
            width: 320
            height: 320
            anchors.top: titleRect.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 20
            color: "#000000"
            Image {
                anchors.fill: parent
                source: "image://skyimage/" + resPath + "?type=2&width=" + width
                        + "&height=" + height
                fillMode: Image.PreserveAspectFit
                visible: !isVideo
            }
            UpdateResourcePreviewPlayer {
                id: player
                anchors.fill: parent
                path: resPath
                visible: isVideo
            }
        }
        Item {
            id: cropRect
            width: isVideo ? 280 : 168
            height: 30
            anchors.top: previewRect.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 24
            clip: true
            ListView {
                id: imageList
                width: parent.width
                height: 28
                anchors.centerIn: parent
                orientation: ListView.Horizontal
                interactive: false
                contentX: -30
                model: Math.ceil(
                           (isVideo ? resDuration : clipDuration) / pixelTime / 28)
                delegate: Image {
                    width: 28
                    height: parent.height
                    source: {
                        if (isVideo) {
                            var videoTime = clipDuration / scrollRect.width * width * index
                            return "image://skyimage/" + resPath + "?type=1&time="
                                    + videoTime + "&width=" + width + "&height=" + height
                        } else {
                            return "image://skyimage/" + resPath + "?type=2&width="
                                    + width + "&height=" + height
                        }
                    }
                    fillMode: Image.PreserveAspectCrop
                }
                Timer {
                    id: timer
                    interval: 10
                    running: false
                    repeat: false
                    onTriggered: {
                        imageList.contentX = -scrollRect.x
                    }
                }
                Component.onCompleted: {
                    timer.start()
                }
                onContentXChanged: {
                    player.setTime(startTime + currentTime, startTime, endTime)
                }
            }
            Rectangle {
                id: scrollRect
                width: 168
                height: parent.height
                anchors.centerIn: parent
                color: "transparent"
                border.width: 1
                border.color: "#FFFFFF"
                Rectangle {
                    id: cursor
                    width: 2
                    height: parent.height - 2
                    anchors.verticalCenter: parent.verticalCenter
                    x: (scrollRect.width - 1) * currentTime / clipDuration
                    color: "#02E4D6"
                    Timer {
                        id: cursorTimer
                        interval: 1000 / 30
                        repeat: true
                        triggeredOnStart: true
                        onTriggered: {
                            if (currentTime + startTime > endTime) {
                                currentTime = endTime - startTime
                                cursorTimer.stop()
                            } else {
                                currentTime += 1 / 30
                            }
                        }
                    }
                }
                MouseArea {
                    property point clickPos: "0,0"
                    property real lastContentX
                    property bool isMove: false
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    acceptedButtons: Qt.LeftButton
                    onPressed: {
                        isMove = false
                        if (isVideo) {
                            lastContentX = imageList.contentX
                            clickPos = Qt.point(mouseX, mouseY)
                        }
                    }
                    onPositionChanged: {
                        var delta = Qt.point(mouse.x - clickPos.x,
                                             mouse.y - clickPos.y)
                        if (!isMove && Math.abs(delta.x) >= 1) {
                            isMove = true
                            cursorTimer.stop()
                            cursorShape = Qt.OpenHandCursor
                        }
                        if (isVideo) {
                            imageList.contentX = Math.min(
                                        imageList.contentWidth - scrollRect.width - scrollRect.x,
                                        Math.max(-scrollRect.x,
                                                 lastContentX - delta.x))
                        }
                    }
                    onReleased: {
                        cursorShape = Qt.PointingHandCursor
                        cursorTimer.stop()
                        if (!isMove) {
                            currentTime = clipDuration * mouseX / scrollRect.width
                        }
                        if (isVideo) {
                            player.updateImage()
                        }
                    }
                }
            }
            Rectangle {
                anchors.left: parent.left
                anchors.right: scrollRect.left
                anchors.top: imageList.top
                anchors.bottom: imageList.bottom
                color: "#80000000"
            }
            Rectangle {
                anchors.left: scrollRect.right
                anchors.right: parent.right
                anchors.top: imageList.top
                anchors.bottom: imageList.bottom
                color: "#80000000"
            }
        }
        Image {
            id: btnPlayState
            width: 16
            height: 16
            sourceSize.height: width
            sourceSize.width: height
            anchors.right: cropRect.left
            anchors.verticalCenter: cropRect.verticalCenter
            anchors.rightMargin: 8
            source: cursorTimer.running ? "qrc:/image/assets/video_pause.png" : "qrc:/image/assets/video_play.png"
            fillMode: Image.Pad
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (cursorTimer.running) {
                        cursorTimer.stop()
                    } else {
                        if (Math.abs(currentTime + startTime - endTime) < 0.0000001) {
                            currentTime = 0
                        }
                        cursorTimer.start()
                    }
                }
            }
        }
        Text {
            id: playProgressTime
            anchors.top: cropRect.bottom
            anchors.horizontalCenter: cropRect.horizontalCenter
            anchors.topMargin: 10
            color: "#FFFFFF"
            text: player.formatTime(pixelTime * cursor.x)
        }
        Rectangle {
            id: bottomRect
            width: parent.width - 4
            height: 40
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.bottomMargin: 2
            anchors.leftMargin: 2
            color: "#15171C"
            BasicCheckBox {
                id: checkBox
                height: 16
                text: qsTr("Reuse the original video effect")
                indicatorWidth: 10
                leftPadding: 0
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 12
                textColor: "#FFFFFF"
                backgroundColor: "transparent"
                font.pixelSize: 12
                radius: 1
                checked: true
            }
            Button {
                id: okBtn
                width: 70
                height: 26
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 12
                style: 1
                text: qsTr("Replace")
                onClicked: {
                    close()
                    var _resDuration = isVideo ? resDuration : clipDuration
                    if (endTime <= _resDuration) {
                        timelineController.updateClipResource(resPath,
                                                              startTime,
                                                              endTime,
                                                              checkBox.checked)
                    } else {
                        timelineController.updateClipResource(
                                    resPath,
                                    startTime - (endTime - _resDuration),
                                    _resDuration, checkBox.checked)
                    }
                }
            }
            Button {
                width: 70
                height: 26
                anchors.right: okBtn.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 10
                style: 2
                text: qsTr("Cancel")
                onClicked: {
                    close()
                }
            }
        }
    }
}
