import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Repeater {

    property int cache: 2
    property int offset: Math.floor(scrollView.contentX / rulerWidth) - cache
    function getText(index) {
        if (index % numFrame == 0) {
            var timeFrame = index * allFrame
            if (allFrame >= fps || (timeFrame / numFrame) % fps == 0) {
                //108000是1小时以上的显示
                var hours = Math.floor(timeFrame / numFrame / 108000)
                if (hours > 0) {
                    var hoursStr = hours < 10 ? "0" + hours : hours
                    var seconds = timeFrame / numFrame % 108000
                    return hoursStr + ":" + Qt.formatTime(
                                new Date(1000 * seconds / fps), "mm:ss")
                } else {
                    seconds = timeFrame / numFrame % 108000
                    return Qt.formatTime(new Date(1000 * seconds / fps),
                                         "mm:ss")
                }
            } else {
                return (index * allFrame / numFrame % fps) + "f"
            }
        } else {
            return ""
        }
    }
    model: scrollView.width / rulerWidth + cache * 2
    Item {
        width: rulerWidth
        height: parent.height
        x: (index + offset) * rulerWidth
        visible: (index + offset) >= 0
        Rectangle {
            id: line
            width: 1
            height: (index + offset) % numFrame == 0 ? 8 : 4
            anchors.bottomMargin: (index + offset) % numFrame == 0 ? 0 : 2
            anchors.bottom: parent.bottom
            color: index < cache ? "transparent" : "#747474"
        }
        Label {
            anchors.bottom: line.top
            anchors.horizontalCenter: line.horizontalCenter
            anchors.bottomMargin: 8
            text: getText(index + offset)
            font.pixelSize: 12
            color: "#747474"
        }
    }
}
