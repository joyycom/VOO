import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle {
    property alias contentX: videoClip.contentX
    property int imageWidth: 28
    property int cache: 4 //左右各缓存4个
    property int offset
    color: "#202020"
    Timer {
        id: imageTimer
        interval: 250
        running: false
        repeat: false
        onTriggered: {
            updateImage()
        }
    }
    onContentXChanged: {
        imageTimer.start()
    }
    Component.onCompleted: {
        updateImage()
    }
    function updateImage() {
        offset = Math.floor(contentX / imageWidth) - cache
    }
    Flickable {
        id: videoClip
        anchors.fill: parent
        interactive: false
        Repeater {
            anchors.fill: parent
            model: clipWidth / imageWidth + cache * 2
            Image {
                width: imageWidth
                height: parent.height
                x: (index + offset) * imageWidth
                fillMode: Image.PreserveAspectCrop
                source: {
                    if (index < cache) {
                        return ""
                    }
                    updateVideoClip
                    var scale = (timelineController.getTimeScale() * fps)
                    var time = (index + offset) * imageWidth / scale
                    if (time < 0) {
                        return ""
                    }
                    return "image://skyimage/" + resource + "?type=1&time="
                            + time + "&width=" + imageWidth * 2 + "&height=" + imageWidth
                            * 2 + "&scale=" + scale + "&speed="
                            + speed + "&curvedSpeed=" + curvedSpeed + "&clip="
                            + clipRoot.itemIndex() + "&track=" + trackRoot.itemIndex()+"&frameInterval="+imageWidth / scale
                }
            }
        }
    }

    //    Image {
    //        id: image
    //        width: 12
    //        height: 12
    //        anchors.left: parent.left
    //        anchors.bottom: parent.bottom
    //        anchors.leftMargin: 2
    //        anchors.bottomMargin: 2
    //        source: iconPath
    //        fillMode: Image.PreserveAspectFit
    //    }
    //    Label {
    //        anchors.left: image.right
    //        anchors.verticalCenter: image.verticalCenter
    //        font.pixelSize: 12
    //        anchors.leftMargin: 2
    //        text: resourceName
    //        color: "white"
    //    }
    TextField {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 2
        anchors.leftMargin: 2 - pausePosition
        color: "#FFFFFF"
        font.pixelSize: 8
        text: (parent.width / timeValue).toFixed(2).toString() + "s"
        leftPadding: 2
        rightPadding: 2
        topPadding: 0
        bottomPadding: 0
        enabled: false
        background: Rectangle {
            color: "#66000000"
            radius: 1
        }
    }
}
