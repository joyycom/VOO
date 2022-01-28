import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    function getBackgroundColor() {
        if (isEffectClip()) {
            return "#A7479D"
        } else if (isStickerClip()) {
            return "#7654FF"
        } else if (isTextClip()) {
            return "#54CCFF"
        } else if (isFilterClip()) {
            return "#66B97D"
        } else {
            return "#B75FAE"
        }
    }
    radius: selected ? 0 : 2
    color: getBackgroundColor()
    Image {
        id: image
        width: 12
        height: 12
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 2
        anchors.bottomMargin: 2
        source: iconPath
        fillMode: Image.PreserveAspectFit
    }
    Label {
        anchors.left: image.right
        anchors.verticalCenter: image.verticalCenter
        font.pixelSize: 12
        anchors.leftMargin: 2
        text: isTextClip() ? wordContent : resourceName
        color: "white"
        maximumLineCount: 1
    }
}
