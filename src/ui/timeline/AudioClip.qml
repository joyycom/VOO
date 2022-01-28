import AudioWavWidget 1.0
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

Rectangle {
    property alias contentX: audioClip.contentX
    property int audioWidth: 200
    property int cache: 2
    property int offset: Math.floor(contentX / audioWidth) - cache
    radius: selected ? 0 : 2
    color: "#3B3E47"
    Flickable {
        id: audioClip
        anchors.fill: parent
        interactive: false
        Repeater {
            anchors.fill: parent
            model: clipWidth / audioWidth + cache * 2
            AudioWavWidget {
                width: audioWidth
                height: parent.height
                x: (index + offset) * audioWidth
                clipIndex: clipRoot.itemIndex()
                trackIndex: trackRoot.itemIndex()
                showStartTime: (index + offset) * audioWidth * speed / timeValue
                showEndTime: ((index + offset) * audioWidth + width) * speed / timeValue
            }
        }
    }

    Image {
        id: image
        width: 12
        height: 12
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 2 - pausePosition
        anchors.bottomMargin: 2
        source: iconPath
        fillMode: Image.PreserveAspectFit
    }
    Label {
        anchors.left: image.right
        anchors.verticalCenter: image.verticalCenter
        font.pixelSize: 12
        anchors.leftMargin: 2
        text: resourceName + "(" + volume + "%)"
        color: "white"
    }
}
