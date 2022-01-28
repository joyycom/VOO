import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQml 2.12
import "../widget"
import "Timeline.js" as Logic
import "Ruler.js" as RulerLogic

Rectangle {
    id: root
    width: parent.width
    height: 45
    color: "#15171C"

    ToolTip {
        id: tooltip
        y: -20
        leftPadding: 8
        rightPadding: 8
        topPadding: 4
        bottomPadding: 4
        contentItem: Text {
            color: "#FFFFFF"
            font.pixelSize: 12
            text: tooltip.text
        }
        background: Rectangle {
            color: "#020203"
        }
        function show(text, item) {
            tooltip.text = text
            tooltip.x = root.mapFromItem(item,
                                         -(tooltip.width - item.width) / 2, 0).x
            tooltip.visible = true
        }
    }
    Row {
        width: parent.width
        spacing: 26
        anchors.leftMargin: 30
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        Image {
            id: btnRedo
            width: 16
            height: 16
            enabled: false
            source: enabled ? "qrc:/image/assets/timeline_redo.png" : "qrc:/image/assets/timeline_redo_disable.png"
            fillMode: Image.PreserveAspectFit
            MouseArea {
                anchors.fill: parent
                drag.threshold: 10
                propagateComposedEvents: true
                hoverEnabled: true
                onClicked: {
                    console.log("redo onclick")
                    timelineController.redo()
                }
                onEntered: {
                    tooltip.show(qsTr("Redo") + (Qt.platform.os
                                                 === "osx" ? "(⌘+⇧+Z)" : "(Ctrl+Shift+Z)"),
                                 btnRedo)
                }
                onExited: {
                    tooltip.hide()
                }
            }
        }

        Image {
            id: btnUndo
            width: 16
            height: 16
            enabled: false
            source: enabled ? "qrc:/image/assets/timeline_undo.png" : "qrc:/image/assets/timeline_undo_disable.png"
            fillMode: Image.PreserveAspectFit
            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                hoverEnabled: true
                onClicked: {
                    console.log("undo onclick")
                    timelineController.undo()
                }
                onEntered: {
                    tooltip.show(qsTr("Undo") + (Qt.platform.os === "osx" ? "(⌘+Z)" : "(Ctrl+Z)"),
                                 btnUndo)
                }
                onExited: {
                    tooltip.hide()
                }
            }
        }

        Image {
            id: btnRewindVideo
            width: 16
            height: 16
            enabled: false
            source: enabled ? "qrc:/image/assets/timeline_rewind.png" : "qrc:/image/assets/timeline_rewind_disable.png"
            fillMode: Image.PreserveAspectFit
            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                hoverEnabled: true
                onClicked: {
                    timelineController.rewindVideoClip()
                    console.log("rewind onclick")
                }
                onEntered: {
                    tooltip.show(qsTr("Rewind"), btnRewindVideo)
                }
                onExited: {
                    tooltip.hide()
                }
            }
        }
        Image {
            id: btnSplit
            width: 16
            height: 16
            enabled: false
            source: enabled ? "qrc:/image/assets/timeline_split.png" : "qrc:/image/assets/timeline_split_disable.png"
            fillMode: Image.PreserveAspectFit
            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                hoverEnabled: true
                onClicked: {
                    console.log("splitClip onclick")
                    timelineController.splitClip()
                }
                onEntered: {
                    tooltip.show(qsTr("Split"), btnSplit)
                }
                onExited: {
                    tooltip.hide()
                }
            }
        }

        Image {
            id: btnDelete
            width: 16
            height: 16
            enabled: false
            source: enabled ? "qrc:/image/assets/timeline_delete.png" : "qrc:/image/assets/timeline_delete_disable.png"
            fillMode: Image.PreserveAspectFit
            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                hoverEnabled: true
                onClicked: {
                    console.log("delete onclick")
                    timelineController.removeClip()
                }
                onEntered: {
                    tooltip.show(qsTr("Delete"), btnDelete)
                }
                onExited: {
                    tooltip.hide()
                }
            }
        }

        Image {
            property int inputDeviceType: -1
            id: inputDevice
            width: 16
            height: 16
            enabled: inputDeviceType === 0 || inputDeviceType === 1
            source: {
                if (inputDeviceType === 0) {
                    return "qrc:/image/assets/timeline_input_add.png"
                } else if (inputDeviceType === 1) {
                    return "qrc:/image/assets/timeline_input_delete.png"
                } else {
                    return "qrc:/image/assets/timeline_input_add_disable.png"
                }
            }
            fillMode: Image.PreserveAspectFit
            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                hoverEnabled: true
                onClicked: {
                    inputDevice.inputDeviceType = timelineController.configInputDevice(
                                inputDevice.inputDeviceType)
                }
                onEntered: {
                    tooltip.show(qsTr("Input Device"), inputDevice)
                }
                onExited: {
                    tooltip.hide()
                }
            }
        }
        Image {
            id: btnCrop
            width: 16
            height: 16
            source: "qrc:/image/assets/timeline_crop.png"
            fillMode: Image.PreserveAspectFit
            visible: false
            MouseArea {
                anchors.fill: parent
                propagateComposedEvents: true
                hoverEnabled: true
                onClicked: {
                    var component = Qt.createComponent(
                                "qrc:/ui/dialog/CropDialog.qml")
                    var page = component.createObject(editorPage)
                    page.cropInfo = timelineController.getCropInfo()
                }
                onEntered: {
                    tooltip.show(qsTr("Crop"), btnCrop)
                }
                onExited: {
                    tooltip.hide()
                }
            }
        }
        Rectangle {
            id: timelineWatch
            width: 16
            height: 16
            color: timeRoot.timelineWatchError === "" ? "green" : "red"
            visible: timeRoot.timelineWatchChecked
            onVisibleChanged: {
                if (visible) {
                    timeRoot.watchTimeline()
                }
            }
            MouseArea {
                anchors.fill: parent
                onDoubleClicked: {
                    timeRoot.watchTimeline()
                }
                onClicked: {
                    if (timeRoot.timelineWatchError !== "") {
                        console.log("timeline watch error:\n" + timeRoot.timelineWatchError)
                    }
                }
            }
        }
        Connections {
            target: timelineController
            onRedoEnabledChanged: {
                btnRedo.enabled = enabled
                console.log("onRedoEnabledChanged:" + enabled)
            }
            onUndoEnabledChanged: {
                btnUndo.enabled = enabled
                console.log("onUndoEnabledChanged:" + enabled)
            }
            onSelectionChanged: {
                var selection = timelineController.selection
                var selected = selection[0] >= 0 && selection[1] >= 0
                btnCrop.visible = timelineController.cropVisible()
                btnDelete.enabled = selected
                btnSplit.enabled = selected && !timelineController.selection[2]
                inputDevice.inputDeviceType = timelineController.hasInputDevice(
                            selection[0], selection[1])
                console.log("onSelectionChanged:" + selected)
            }
            onClipRewindEnabledChanged: {
                btnRewindVideo.enabled = enabled
                console.log("onClipRewindEnabledChanged:" + enabled)
            }
        }
    }

    Image {
        id: zoomIn
        source: "qrc:/image/assets/timeline_zoom_in.png"
        width: 20
        height: 20
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 30
        function zoomInScale() {
            var lastPosition = cursor.x
            var zoomScaleIndex = RulerLogic.zoomIn()
            timelineController.timeScale = RulerLogic.getTimeScale(
                        zoomScaleIndex, true)
            Logic.updateCursor(lastPosition)
            ++updateVideoClip
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                zoomIn.zoomInScale()
            }
            onEntered: {
                tooltip.show(qsTr("Zoom In") + (Qt.platform.os === "osx" ? "(⌘++)" : "(Ctrl++)"),
                             zoomIn)
            }
            onExited: {
                tooltip.hide()
            }
        }
        Shortcut {
            sequences: ["Ctrl+=", StandardKey.ZoomIn]
            onActivated: {
                zoomIn.zoomInScale()
            }
        }
    }

    SeekBar {
        id: seekBar
        width: 80
        height: 20
        anchors.right: zoomIn.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 7
        onSliderPressedChanged: {
            if (!pressed) {
                ++updateVideoClip
            }
        }
        slider {
            orientation: Qt.Horizontal
            from: bestMaxScaleIndex
            to: 0
            stepSize: -1
            value: scaleIndex
            onValueChanged: {
                var sliderValue = Math.floor(seekBar.slider.value)
                if (sliderValue != scaleIndex) {
                    var lastPosition = cursor.x
                    timelineController.timeScale = RulerLogic.getTimeScale(
                                sliderValue, true)
                    Logic.updateCursor(lastPosition)
                }
            }
        }
    }
    Image {
        id: zoomOut
        source: "qrc:/image/assets/timeline_zoom_out.png"
        width: 20
        height: 20
        anchors.right: seekBar.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 7
        function zoomOutScale() {
            var lastPosition = cursor.x
            var zoomScaleIndex = RulerLogic.zoomOut()
            timelineController.timeScale = RulerLogic.getTimeScale(
                        zoomScaleIndex, true)
            Logic.updateCursor(lastPosition)
            ++updateVideoClip
        }
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                zoomOut.zoomOutScale()
            }
            onEntered: {
                tooltip.show(qsTr("Zoom Out") + (Qt.platform.os === "osx" ? "(⌘+-)" : "(Ctrl+-)"),
                             zoomOut)
            }
            onExited: {
                tooltip.hide()
            }
        }
        Shortcut {
            sequence: StandardKey.ZoomOut
            onActivated: {
                zoomOut.zoomOutScale()
            }
        }
    }
}
