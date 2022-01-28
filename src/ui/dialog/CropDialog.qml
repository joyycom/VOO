import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import FramelessWindow 1.0
import QtQuick.Layouts 1.3
import CropPreviewPlayer 1.0
import "../widget"

FramelessWindow {
    id: window
    modality: Qt.ApplicationModal
    width: 960
    height: 674
    resizable: false
    onlyAllowListMovable: true
    allowTouchWidget: [titleRect]
    property var cropInfo
    property bool isInit: false
    property bool isVideo: false
    property real originalRatio: 1 / 1
    property real previewRatio: originalRatio
    property real mouseVideoRatio: 0
    property real tansformScale: {
        var opposite
        var adjacent
        if (contentRect.width > contentRect.height) {
            opposite = contentRect.width
            adjacent = contentRect.height
        } else {
            opposite = contentRect.height
            adjacent = contentRect.width
        }
        //角度取绝对值
        var rotation = Math.abs(rotationAngleSeekBar.slider.value)
        //修复正确要计算的角度
        var fixRotation = rotation > 90 ? 180 - rotation : rotation
        //矩形的角度
        var rectRotation = Math.atan(opposite / adjacent) * 180 / Math.PI
        //矩形的半径
        var rectRadius = Math.sqrt(opposite * opposite + adjacent * adjacent)
        //计算出矩形投影在旋转角度后的长度
        var projectionRotation = rectRotation - fixRotation
        //得出新的尺寸
        var newDistance = Math.cos(
                    projectionRotation * Math.PI / 180) * rectRadius
        //给出缩放比例
        return newDistance / adjacent
    }
    function init() {
        if (isInit || !cropInfo || contentRect.width <= 0
                || contentRect.height <= 0 || tansformScale <= 0) {
            return
        }
        var imageSource = cropInfo[0]
        var isVideo = cropInfo[1]
        var rotation = cropInfo[2]
        var ratio = cropInfo[3]
        var ratioIndex = cropInfo[4]
        var x = cropInfo[5]
        var y = cropInfo[6]
        var w = cropInfo[7]
        var h = cropInfo[8]
        if (!isVideo) {
            contentImage.source = "file:/" + imageSource
        }
        window.isVideo = isVideo
        window.originalRatio = ratio
        videoRatioSelect.currentIndex = ratioIndex
        rotationAngleSeekBar.slider.value = rotation
        borderRect.width = contentRect.width * tansformScale * w
        borderRect.height = contentRect.height * tansformScale * h
        if (x >= 0) {
            borderRect.x = contentRect.x + contentRect.width * x
        }
        if (y >= 0) {
            borderRect.y = contentRect.y + contentRect.height * y
        }
        isInit = true
    }
    onCropInfoChanged: {
        init()
    }
    Timer {
        interval: 0
        running: true
        repeat: false
        onTriggered: {
            init()
        }
    }
    Rectangle {
        color: "#222425"
        radius: 4
        anchors.fill: parent
        Item {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: titleRect.bottom
            anchors.bottom: bottomRect.top
            anchors.leftMargin: 2
            anchors.rightMargin: 2
            CropPreviewPlayer {
                id: contentVideo
                anchors.fill: contentRect
                visible: isVideo
                transform: [
                    Rotation {
                        axis {
                            x: 0
                            y: 0
                            z: 1
                        }
                        origin.x: contentImage.width / 2
                        origin.y: contentImage.height / 2
                        angle: rotationAngleSeekBar.slider.value
                    },
                    Scale {
                        origin.x: contentImage.width / 2
                        origin.y: contentImage.height / 2
                        xScale: tansformScale
                        yScale: tansformScale
                    }
                ]
            }
            Image {
                id: contentImage
                anchors.fill: contentRect
                sourceSize.width: width
                sourceSize.height: height
                fillMode: Image.PreserveAspectFit
                visible: !isVideo
                transform: [
                    Rotation {
                        axis {
                            x: 0
                            y: 0
                            z: 1
                        }
                        origin.x: contentImage.width / 2
                        origin.y: contentImage.height / 2
                        angle: rotationAngleSeekBar.slider.value
                    },
                    Scale {
                        origin.x: contentImage.width / 2
                        origin.y: contentImage.height / 2
                        xScale: tansformScale
                        yScale: tansformScale
                    }
                ]
            }
            RowLayout {
                id: progressRowLayout
                height: 16
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                visible: isVideo
                Image {
                    id: btnPlayState
                    width: 16
                    height: 16
                    sourceSize.height: width
                    sourceSize.width: height
                    Layout.leftMargin: 14
                    source: contentVideo.isPlaying ? "qrc:/image/assets/video_pause.png" : "qrc:/image/assets/video_play.png"
                    fillMode: Image.Pad
                    MouseArea {
                        anchors.fill: parent
                        propagateComposedEvents: true
                        onClicked: {
                            if (contentVideo.isPlaying) {
                                contentVideo.stop()
                            } else {
                                contentVideo.play()
                            }
                        }
                    }
                }
                Text {
                    id: playProgressTime
                    Layout.leftMargin: 14
                    color: "#7FFFFFFF"
                    text: contentVideo.formatTime(contentVideo.currentTime)
                }
                ProgressBar {
                    id: playProgress
                    Layout.leftMargin: 10
                    Layout.rightMargin: 10
                    Layout.fillWidth: true
                    background: Rectangle {
                        implicitWidth: 180
                        implicitHeight: 2
                        color: "#32FFFFFF"
                    }
                    contentItem: Item {
                        implicitWidth: parent.width
                        implicitHeight: 2
                        Rectangle {
                            width: (contentVideo.currentTime / contentVideo.resourceTime(
                                        )) * parent.width
                            height: parent.height
                            radius: 2
                            color: "#FFFFFF"
                        }
                    }
                    MouseArea {
                        width: parent.width
                        height: 14
                        anchors.verticalCenter: parent.verticalCenter
                        onPressed: {
                            contentVideo.stop()
                            var progress = Math.min(
                                        1,
                                        Math.max(0,
                                                 mouseX / playProgress.width))
                            contentVideo.currentTime = contentVideo.resourceTime(
                                        ) * progress
                        }
                        onPositionChanged: {
                            if (mouse.buttons != Qt.LeftButton) {
                                return
                            }
                            var progress = Math.min(
                                        1,
                                        Math.max(0,
                                                 mouseX / playProgress.width))
                            contentVideo.currentTime = contentVideo.resourceTime(
                                        ) * progress
                        }
                    }
                }
                Text {
                    id: playEndTime
                    color: "#7FFFFFFF"
                    text: contentVideo.formatTime(contentVideo.resourceTime())
                }
            }
            Rectangle {
                anchors.left: parent.left
                anchors.right: borderRect.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                color: "#7F000000"
            }
            Rectangle {
                id: rightShadow
                anchors.left: borderRect.right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                color: "#7F000000"
            }
            Rectangle {
                anchors.left: borderRect.left
                anchors.right: borderRect.right
                anchors.top: parent.top
                anchors.bottom: borderRect.top
                color: "#7F000000"
            }
            Rectangle {
                anchors.left: borderRect.left
                anchors.right: borderRect.right
                anchors.top: borderRect.bottom
                anchors.bottom: parent.bottom
                color: "#7F000000"
            }
            Item {
                id: maxContentRect
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: progressRowLayout.top
                anchors.topMargin: 20
                anchors.bottomMargin: 20
                anchors.leftMargin: 70
                anchors.rightMargin: 70
            }
            Item {
                id: contentRect
                width: maxContentRect.width / maxContentRect.height
                       > originalRatio ? maxContentRect.height
                                         * originalRatio : maxContentRect.width
                height: maxContentRect.width / maxContentRect.height
                        > originalRatio ? maxContentRect.height : maxContentRect.width
                                          / originalRatio
                anchors.centerIn: maxContentRect
            }
            Item {
                id: ratioContentRect
                width: contentRect.width / contentRect.height
                       > previewRatio ? contentRect.height * previewRatio : contentRect.width
                height: contentRect.width / contentRect.height
                        > previewRatio ? contentRect.height : contentRect.width / previewRatio
                anchors.centerIn: contentRect
            }
            Item {
                id: borderRect
                width: ratioContentRect.width
                height: ratioContentRect.height
                x: ratioContentRect.x
                y: ratioContentRect.y
                Rectangle {
                    width: parent.width
                    height: 2
                    anchors.top: parent.top
                    anchors.topMargin: parent.height / 3
                    color: "#7FFFFFFF"
                }
                Rectangle {
                    width: parent.width
                    height: 2
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: parent.height / 3
                    color: "#7FFFFFFF"
                }
                Rectangle {
                    width: 2
                    height: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: parent.width / 3
                    color: "#7FFFFFFF"
                }
                Rectangle {
                    width: 2
                    height: parent.height
                    anchors.right: parent.right
                    anchors.rightMargin: parent.width / 3
                    color: "#7FFFFFFF"
                }
                MouseArea {
                    property point clickPos: "0,0"
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    onPressed: {
                        clickPos = Qt.point(mouseX, mouseY)
                    }
                    onPositionChanged: {
                        var delta = Qt.point(mouseX - clickPos.x,
                                             mouseY - clickPos.y)
                        //contentRect
                        borderRect.x = Math.min(
                                    contentRect.x + contentRect.width - borderRect.width,
                                    Math.max(contentRect.x,
                                             borderRect.x + delta.x))
                        borderRect.y = Math.min(
                                    contentRect.y + contentRect.height - borderRect.height,
                                    Math.max(contentRect.y,
                                             borderRect.y + delta.y))
                    }
                }
            }
            Rectangle {
                width: 2
                anchors.top: leftTop1.bottom
                anchors.bottom: leftBottom1.top
                anchors.horizontalCenter: leftTop1.horizontalCenter
                color: "#7FFFFFFF"
            }
            Rectangle {
                width: 2
                anchors.top: rightTop1.bottom
                anchors.bottom: rightBottom1.top
                anchors.horizontalCenter: rightTop1.horizontalCenter
                color: "#7FFFFFFF"
            }
            Rectangle {
                height: 2
                anchors.left: leftTop2.right
                anchors.right: rightTop2.left
                anchors.verticalCenter: leftTop2.verticalCenter
                color: "#7FFFFFFF"
            }
            Rectangle {
                height: 2
                anchors.left: leftBottom2.right
                anchors.right: rightBottom2.left
                anchors.verticalCenter: leftBottom2.verticalCenter
                color: "#7FFFFFFF"
            }
            Rectangle {
                id: leftTop1
                width: 6
                height: 30
                anchors.horizontalCenter: borderRect.left
                anchors.top: leftTop2.top
                color: "white"
                CropDialogMouse {
                    anchors.fill: parent
                    borderRect: borderRect
                    contentRect: contentRect
                    videoRatio: mouseVideoRatio
                    isLeft: true
                    isTop: true
                }
            }
            Rectangle {
                id: leftTop2
                width: 30
                height: 6
                anchors.verticalCenter: borderRect.top
                anchors.left: leftTop1.left
                color: "white"
                CropDialogMouse {
                    anchors.fill: parent
                    borderRect: borderRect
                    contentRect: contentRect
                    videoRatio: mouseVideoRatio
                    isLeft: true
                    isTop: true
                }
            }
            Rectangle {
                id: rightTop1
                width: 6
                height: 30
                anchors.horizontalCenter: borderRect.right
                anchors.top: rightTop2.top
                color: "white"
                CropDialogMouse {
                    anchors.fill: parent
                    borderRect: borderRect
                    contentRect: contentRect
                    videoRatio: mouseVideoRatio
                    isLeft: false
                    isTop: true
                }
            }
            Rectangle {
                id: rightTop2
                width: 30
                height: 6
                anchors.verticalCenter: borderRect.top
                anchors.right: rightTop1.right
                color: "white"
                CropDialogMouse {
                    anchors.fill: parent
                    borderRect: borderRect
                    contentRect: contentRect
                    videoRatio: mouseVideoRatio
                    isLeft: false
                    isTop: true
                }
            }
            Rectangle {
                id: leftVertical
                width: 6
                height: 36
                anchors.horizontalCenter: borderRect.left
                anchors.verticalCenter: borderRect.verticalCenter
                CropDialogMouse {
                    anchors.fill: parent
                    borderRect: borderRect
                    contentRect: contentRect
                    videoRatio: mouseVideoRatio
                    isLeft: true
                    isVertical: true
                    isHorizontal: false
                }
            }
            Rectangle {
                id: rightVertical
                width: 6
                height: 36
                anchors.horizontalCenter: borderRect.right
                anchors.verticalCenter: borderRect.verticalCenter
                CropDialogMouse {
                    anchors.fill: parent
                    borderRect: borderRect
                    contentRect: contentRect
                    videoRatio: mouseVideoRatio
                    isLeft: false
                    isVertical: true
                    isHorizontal: false
                }
            }
            Rectangle {
                id: leftBottom1
                width: 6
                height: 30
                anchors.horizontalCenter: borderRect.left
                anchors.bottom: leftBottom2.bottom
                color: "white"
                CropDialogMouse {
                    anchors.fill: parent
                    borderRect: borderRect
                    contentRect: contentRect
                    videoRatio: mouseVideoRatio
                    isLeft: true
                    isTop: false
                }
            }
            Rectangle {
                id: leftBottom2
                width: 30
                height: 6
                anchors.verticalCenter: borderRect.bottom
                anchors.left: leftBottom1.left
                color: "white"
                CropDialogMouse {
                    anchors.fill: parent
                    borderRect: borderRect
                    contentRect: contentRect
                    videoRatio: mouseVideoRatio
                    isLeft: true
                    isTop: false
                }
            }
            Rectangle {
                id: rightBottom1
                width: 6
                height: 30
                anchors.horizontalCenter: borderRect.right
                anchors.bottom: rightBottom2.bottom
                color: "white"
                CropDialogMouse {
                    anchors.fill: parent
                    borderRect: borderRect
                    contentRect: contentRect
                    videoRatio: mouseVideoRatio
                    isLeft: false
                    isTop: false
                }
            }
            Rectangle {
                id: rightBottom2
                width: 30
                height: 6
                anchors.verticalCenter: borderRect.bottom
                anchors.right: rightTop1.right
                color: "white"
                CropDialogMouse {
                    anchors.fill: parent
                    borderRect: borderRect
                    contentRect: contentRect
                    videoRatio: mouseVideoRatio
                    isLeft: false
                    isTop: false
                }
            }
            Rectangle {
                id: topHorizontal
                width: 36
                height: 6
                anchors.verticalCenter: borderRect.top
                anchors.horizontalCenter: borderRect.horizontalCenter
                color: "white"
                CropDialogMouse {
                    anchors.fill: parent
                    borderRect: borderRect
                    contentRect: contentRect
                    videoRatio: mouseVideoRatio
                    isTop: true
                    isVertical: false
                    isHorizontal: true
                }
            }
            Rectangle {
                id: bottomHorizontal
                width: 36
                height: 6
                anchors.verticalCenter: borderRect.bottom
                anchors.horizontalCenter: borderRect.horizontalCenter
                color: "white"
                CropDialogMouse {
                    anchors.fill: parent
                    borderRect: borderRect
                    contentRect: contentRect
                    videoRatio: mouseVideoRatio
                    isTop: false
                    isVertical: false
                    isHorizontal: true
                }
            }
        }
        Rectangle {
            anchors.fill: parent
            border.color: "#000000"
            border.width: 2
            color: "transparent"
        }
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
                window.close()
            }
        }
        Rectangle {
            id: bottomRect
            width: parent.width - 4
            height: 40
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.topMargin: 2
            anchors.leftMargin: 2
            color: "#000000"
            Label {
                id: rotationAngle
                text: qsTr("Rotation Angle")
                font.pixelSize: 12
                color: "#FFFFFF"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 20
            }
            SeekBar {
                id: rotationAngleSeekBar
                width: 110
                height: 20
                anchors.left: rotationAngle.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 10
                onSliderPressedChanged: {

                }
                slider {
                    orientation: Qt.Horizontal
                    from: -180
                    to: 180
                    value: 0
                    onValueChanged: {

                    }
                }
            }
            Label {
                id: rotationAngleLabel
                width: 48
                height: 20
                text: rotationAngleSeekBar.slider.value + "°"
                font.pixelSize: 12
                color: "#BFBFBF"
                anchors.left: rotationAngleSeekBar.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 10
                verticalAlignment: TextEdit.AlignVCenter
                horizontalAlignment: TextEdit.AlignHCenter
                background: Rectangle {
                    anchors.fill: parent
                    color: "#2D2F33"
                }
            }
            Item {
                width: videoRatio.width + videoRatioSelect.width
                       + videoRatioSelect.anchors.leftMargin
                anchors.centerIn: parent
                Label {
                    id: videoRatio
                    text: qsTr("Video Ratio")
                    font.pixelSize: 12
                    color: "#FFFFFF"
                    anchors.verticalCenter: parent.verticalCenter
                }
                BasicComboBox {
                    id: videoRatioSelect
                    width: 100
                    height: 20
                    anchors.left: videoRatio.right
                    anchors.leftMargin: 20
                    anchors.verticalCenter: parent.verticalCenter
                    editable: false
                    backgroundColor: "#2D2F33"
                    itemNormalColor: "#2D2F33"
                    borderColor: "transparent"
                    itemHighlightColor: "#2D2F33"
                    itemHeight: 26
                    model: [qsTr(
                            "Free"), "1 : 1", "16 : 9", "9 : 16", "4 : 3", "3 : 4"]
                    onCurrentIndexChanged: {
                        if (currentIndex === 0) {
                            mouseVideoRatio = 0
                            return
                        }
                        switch (currentIndex) {
                        case 1:
                            previewRatio = 1 / 1
                            break
                        case 2:
                            previewRatio = 16 / 9
                            break
                        case 3:
                            previewRatio = 9 / 16
                            break
                        case 4:
                            previewRatio = 4 / 3
                            break
                        case 5:
                            previewRatio = 3 / 4
                            break
                        default:
                            break
                        }
                        mouseVideoRatio = previewRatio
                        borderRect.x = ratioContentRect.x
                        borderRect.y = ratioContentRect.y
                        borderRect.width = ratioContentRect.width
                        borderRect.height = ratioContentRect.height
                    }
                }
            }
            Button {
                id: confirmButton
                width: 70
                height: 26
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Confirm")
                style: 1
                onClicked: {
                    var contentWidth = contentRect.width * tansformScale
                    var contentHeight = contentRect.height * tansformScale
                    // x0= (x - rx0)*cos(a) - (y - ry0)*sin(a) + rx0 ;
                    // y0= (x - rx0)*sin(a) + (y - ry0)*cos(a) + ry0 ;
                    var rotation = rotationAngleSeekBar.slider.value * Math.PI / 180
                    var rotationX = (0 - contentRect.width / 2) * Math.cos(
                                rotation) - (0 - contentRect.height / 2) * Math.sin(
                                rotation) + contentRect.width / 2
                    var rotationY = (0 - contentRect.width / 2) * Math.sin(
                                rotation) + (0 - contentRect.height / 2) * Math.cos(
                                rotation) + contentRect.height / 2
                    var rx0 = borderRect.width / 2 + (borderRect.x - contentRect.x)
                    var ry0 = borderRect.height / 2 + (borderRect.y - contentRect.y)
                    var originalX = (rotationX - rx0) * Math.cos(
                                rotation) + (rotationY - ry0) * Math.sin(
                                rotation) + rx0
                    var originalY = -(rotationX - rx0) * Math.sin(
                                rotation) + (rotationY - ry0) * Math.cos(
                                rotation) + ry0
                    timelineController.cropVideo(
                                rotationAngleSeekBar.slider.value, videoRatioSelect.currentIndex, (borderRect.x - contentRect.x) / contentRect.width, (borderRect.y - contentRect.y) / contentRect.height,
                                (borderRect.x - (contentRect.x - (contentWidth - contentRect.width) / 2) - originalX) / contentWidth, (borderRect.y - (contentRect.y - (contentHeight - contentRect.height) / 2) - originalY) / contentHeight, borderRect.width / contentWidth,
                                borderRect.height / contentHeight)
                    window.close()
                }
            }
            Button {
                id: resetButton
                width: 70
                height: 26
                anchors.right: confirmButton.left
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Reset")
                style: 2
                onClicked: {
                    contentVideo.reset()
                    rotationAngleSeekBar.slider.value = 0
                    videoRatioSelect.currentIndex = 0
                    previewRatio = originalRatio
                    borderRect.x = ratioContentRect.x
                    borderRect.y = ratioContentRect.y
                    borderRect.width = ratioContentRect.width
                    borderRect.height = ratioContentRect.height
                }
            }
        }
    }
}
