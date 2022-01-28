import QtQuick 2.12
import QtQuick.Controls 2.12

Flickable {
    id: root
    interactive: false
    clip: true
    Repeater {
        anchors.fill: parent
        model: keyFrameTimes
        Image {
            width: 20
            height: 20
            anchors.verticalCenter: parent.verticalCenter
            x: (modelData - preEndTime) * timeValue - root.x - width / 2 - pausePosition
            fillMode: Image.PreserveAspectCrop
            sourceSize.width: 20
            sourceSize.height: 20
            source: Math.abs(
                        timelineController.cursorTime - modelData) * timeValue
                    > 10 ? "qrc:/image/assets/timeline_keyframe.png" : "qrc:/image/assets/timeline_keyframe_selected.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    timelineController.cursorTime = modelData
                }
            }
        }
    }
}
