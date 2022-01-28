import QtQuick 2.12
import QtQuick.Controls 2.12
import PreviewController 1.0
import SkyMedia 1.0
import SkyMediaPlayer 1.0
import QtQuick.Layouts 1.3
import QtMultimedia 5.12
import "../property"

Item {
    property alias previewController: previewController
    property alias previewFrame: previewFrame
    property alias preview: preview
    property var windowParent: null
    property var editorParent: null
    property var windowPreviewParent: null
    property var editorPreviewParent: previewLayout
    anchors.fill: parent
    id: videoPreviewItem
    states: [
        State {
            name: "editor"
            PropertyChanges {
                target: videoPreviewItem
                parent: editorParent
            }
            PropertyChanges {
                target: preview
                parent: editorPreviewParent
            }
            PropertyChanges {
                target: preview
                visible: true
            }
            PropertyChanges {
                target: preview2
                visible: false
            }
        },
        State {
            name: "window"
            PropertyChanges {
                target: preview
                parent: windowPreviewParent
            }
            PropertyChanges {
                target: videoPreviewItem
                parent: windowParent
            }
            PropertyChanges {
                target: preview
                visible: false
            }
            PropertyChanges {
                target: preview2
                visible: true
            }
        }
    ]
    PreviewController {
        id: previewController
        onFrameConfig: {
            previewFrame.frameConfig(previewController.getFrameConfig())
        }
        onVideoProgressChanged: {
            playEndTime.text = previewController.formatTime(
                        previewController.during())
            playProgress.value = progress
            playProgressTime.text = previewController.formatTime(
                        progress * previewController.during())
        }

        onResoucePreviewProgressChanged: {
            playEndTime.text = previewController.formatTime(
                        previewController.previewDuring())
            playProgress.value = progress
            playProgressTime.text = previewController.formatTime(
                        progress * previewController.previewDuring())
        }

        onPreviewTypeChanged: {
            if (type == 1 || type == 2) {
                previewLayout.visible = false
                resoucePreview.visible = true
                previewTitleLayout.visible = true
                previewImageResource.visible = false
                previewTitle.text = qsTr("Previewing")
            } else if (type == 4) {
                previewImageResource.source = "file:/" + previewController.previewResourcePath()
                previewLayout.visible = false
                previewImageResource.visible = true
                resoucePreview.visible = false
                previewTitleLayout.visible = true
                previewTitle.text = qsTr("Previewing Images")
            } else {
                previewImageResource.visible = false
                resoucePreview.visible = false
                previewLayout.visible = true
                if (type == 3) {
                    previewTitleLayout.visible = true
                    previewTitle.text = qsTr("Previewing Effects")
                } else {
                    previewTitleLayout.visible = false
                }
            }

            console.log("onPreviewTypeChanged:" + type + " Source:" + resoucePreview.source)
        }
    }
    onWindowParentChanged: {
        if (windowParent !== null) {
            windowParent.window.allowTouchWidget = windowParent.window.allowTouchWidget.concat(
                        [videoPreviewItem])
        }
    }
    Rectangle {
        id: previewLayout
        //第一层min用于实际宽高和显示宽高的对比，第二层用于窗口大小的适配
        height: Math.min(previewFrame.frameHeight,
                         parent.width / previewFrame.frameScale,
                         parent.height - previewBottomContainer.height)
        width: height * previewFrame.frameScale
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -previewBottomContainer.height / 2
        color: "#000000"
        SkyTimelineView {
            id: preview
            anchors.fill: parent
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    previewController.stop()
                }
                onClicked: {
                    propertyEditController.selectClipFromPreview(mouseX,
                                                                 mouseY, width,
                                                                 height)
                }
            }
        }
        SkyTimelineView2 {
            id: preview2
            anchors.fill: parent
            visible: false
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    previewController.stop()
                }
                onClicked: {
                    propertyEditController.selectClipFromPreview(mouseX,
                                                                 mouseY, width,
                                                                 height)
                }
            }
        }
        onWidthChanged: {
            editTransformProperty.updateUI()
            simpleTransformProperty.updateUI()
        }
        onHeightChanged: {
            editTransformProperty.updateUI()
            simpleTransformProperty.updateUI()
        }
    }

    SkyMediaPlayer {
        id: resourceMediaPlayer
        onError: {
            console.log("resourceMediaPlayer onError:" + error)
        }
        onStateChanged: {
            console.log("resourceMediaPlayer state:" + state)
            if (position === duration && state === 0) {
                resourceMediaPlayer.position = 0
                previewController.stop()
            }
        }
        onMediaStatusChanged: {
            console.log("resourceMediaPlayer mediaStatus:" + status)
        }
    }

    Image {
        id: previewImageResource
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: previewBottomContainer.top
        visible: false
        fillMode: Image.PreserveAspectFit
    }
    VideoOutput {
        id: resoucePreview
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: previewBottomContainer.top
        source: resourceMediaPlayer
        visible: false
    }
    TransformProperty {
        id: editTransformProperty
        anchors.fill: previewLayout
    }
    SimpleTransformProperty {
        id: simpleTransformProperty
        anchors.fill: previewLayout
    }
    Connections {
        target: parent.window
        onVisibilityChanged: {
            editTransformProperty.updateUI()
            simpleTransformProperty.updateUI()
        }
    }
    Component.onCompleted: {
        previewController.setSkyTimelineView(preview)
        previewController.attachMediaPlayer(resourceMediaPlayer, resoucePreview)
        console.log("previewController onCompleted")
    }
    Rectangle {
        id: previewTitleLayout
        color: "#80000000"
        width: parent.width
        height: 60
        anchors.top: parent.top
        visible: false

        Text {
            id: previewTitle
            anchors.fill: parent
            x: 20
            text: qsTr("Previewing")
            color: "#FFFFFF"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
        }
    }

    RowLayout {
        id: previewBottomContainer
        anchors.bottom: parent.bottom
        width: parent.width
        height: 36
        Image {
            id: btnPlayState
            width: 16
            height: 16
            sourceSize.height: width
            sourceSize.width: height
            Layout.leftMargin: 14
            source: previewController.isPlaying ? "qrc:/image/assets/video_pause.png" : "qrc:/image/assets/video_play.png"
            fillMode: Image.Pad
            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                onClicked: {
                    if (previewController.isPlaying) {
                        previewController.stop()
                    } else {
                        previewController.play()
                    }
                }
            }
        }

        Text {
            id: playProgressTime
            Layout.leftMargin: 12
            color: "#7FFFFFFF"
        }
        ProgressBar {
            id: playProgress
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.fillWidth: true
            background: Rectangle {
                implicitWidth: 200
                implicitHeight: 2
                color: "#32FFFFFF"
            }

            contentItem: Item {
                implicitWidth: 200
                implicitHeight: 2

                Rectangle {
                    width: playProgress.visualPosition * parent.width
                    height: parent.height
                    radius: 2
                    color: "#FFFFFF"
                }
            }
        }
        Text {
            id: playEndTime
            text: ""
            color: "#7FFFFFFF"
        }
        PreviewFrame {
            id: previewFrame
            Layout.leftMargin: 12
        }
        Image {
            id: btnMute
            width: 16
            height: 16
            sourceSize.height: width
            sourceSize.width: height
            Layout.leftMargin: 12
            source: "qrc:/image/assets/audio_unmute.png"
            fillMode: Image.Pad
            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                onClicked: {
                    if (btnMute.source == "qrc:/image/assets/audio_unmute.png") {
                        previewController.mute()
                        btnMute.source = "qrc:/image/assets/audio_mute.png"
                    } else {
                        previewController.unmute()
                        btnMute.source = "qrc:/image/assets/audio_unmute.png"
                    }
                }
            }
        }

        Image {
            id: btnWindow
            width: 16
            height: 16
            sourceSize.height: width
            sourceSize.width: height
            source: preview2.visible ? "qrc:/image/assets/window_to_video.png" : "qrc:/image/assets/video_to_window.png"
            Layout.leftMargin: 12
            Layout.rightMargin: 14
            fillMode: Image.Pad
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (preview2.visible) {
                        if (windowParent !== null) {
                            windowParent.close()
                        }
                    } else {
                        editorDatasource.showVideoPreviewDialog()
                    }
                }
            }
        }
    }
}
