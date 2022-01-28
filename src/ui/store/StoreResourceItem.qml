import QtQuick 2.12
import QtQuick.Controls 2.12
import "Store.js" as Logic
import "../widget"

Item {
    id: itemRoot
    width: 90
    height: 110
    property bool itemMove: false
    property bool isItemHover: false
    function dragEnable() {
        if (storeDragWindow && exist) {
            return true
        } else {
            return false
        }
    }
    MouseArea {
        id: dragArea
        width: parent.width
        height: parent.height
        drag.target: null
        hoverEnabled: true
        preventStealing: true
        onEntered: {
            isItemHover = true
        }
        onExited: {
            isItemHover = false
        }
        property bool isMove: false
        property real startMouseX
        property real startMouseY
        onPressed: {
            if (mouse.button !== Qt.LeftButton) {
                return
            }
            isMove = false
            startMouseX = mouseX
            startMouseY = mouseY
        }
        onPositionChanged: {
            if (mouse.buttons !== Qt.LeftButton) {
                return
            }
            if (!isMove && (Math.abs(mouseX - startMouseX) > 1 || Math.abs(
                                mouseY - startMouseY) > 1)) {
                isMove = true
            }
            if (dragEnable()) {
                if (storeDragWindow.item !== iconContainer) {
                    storeDragWindow.item = iconContainer
                } else {
                    storeDragWindow.image.opacity = 1
                }
                timeLineRec.isStoreDrag = true
                Logic.onPositionChanged(iconContainer, storeDragWindow, mouse,
                                        startMouseX, startMouseY, cate)
            }
        }
        onReleased: {
            if (mouse.button !== Qt.LeftButton) {
                return
            }
            if (dragEnable()) {
                storeDragWindow.opacity = 0
                storeDragWindow.image.opacity = 0
                Logic.onReleased(iconContainer, storeDragWindow, mouse, 2, [index])
                timeLineRec.isStoreDrag = false
            }
            if (!exist) {
                resourceList.selectIndex = index
                resourceList.focus = true
                downloadingRes.push(index)
                downloadingRes = downloadingRes
                rotationAnimation.running = true
            } else {
                if (!isMove) {
                    resourceList.selectIndex = index
                    resourceList.focus = true
                }
            }
            console.log("onReleased:" + isMove + " exist:" + exist)
        }
    }

    Rectangle {
        width: 84
        height: 104
        color: "transparent"
        anchors.centerIn: parent

        Rectangle {
            width: 84
            height: 84
            id: iconContainer
            border.color: "#02E4D6"
            border.width: (isSelected || saving) ? 1.5 : 0
            color: "transparent"
            property bool saving: false
            BasicImage {
                id: item
                width: parent.width - 6
                height: parent.height - 6
                anchors.centerIn: parent
                imgSrc: cover
                animatedImgSrc: preview
                isEntered: isItemHover
            }

            /*Image {
                id: item
                width: parent.width - 6
                height: parent.height - 6
                anchors.centerIn: parent
                source: cover
                visible: !isItemHover
                fillMode: Image.PreserveAspectCrop
            }*/
            //            Image {
            //                //AnimatedImage {
            //                id: itemAnim
            //                anchors.fill: item
            //                source: cover
            //                visible: isItemHover
            //                fillMode: Image.PreserveAspectCrop
            //            }
        }
        Text {
            text: name
            width: parent.width
            height: 16
            clip: true
            elide: Text.ElideRight
            anchors {
                top: iconContainer.bottom
                topMargin: 2
                bottom: parent.bottom
                bottomMargin: 2
                right: parent.right
                left: iconContainer.left
            }
            color: "#BFBFBF"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }

        Image {
            id: downloadIcon
            width: 20
            height: 20
            anchors.right: iconContainer.right
            anchors.rightMargin: 4
            anchors.top: iconContainer.top
            anchors.topMargin: 4
            visible: !downloadingRes.includes(index)
            source: exist ? (isItemHover ? "qrc:/image/assets/res_add.png" : "") : "qrc:/image/assets/res_download.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (exist) {
                        resourceList.focus = false
                        datasource.addResourceToTimeline(index)
                    } else {
                        resourceList.selectIndex = index
                        resourceList.focus = true
                        downloadingRes.push(index)
                        downloadingRes = downloadingRes
                        rotationAnimation.running = true
                    }
                }
            }
        }
        Image {
            id: downloading_icon
            anchors.fill: downloadIcon
            visible: downloadingRes.includes(index)
            source: "qrc:/image/assets/res_downloading.png"
        }
        RotationAnimation {
            id: rotationAnimation
            target: downloading_icon
            from: 0
            to: 360
            direction: RotationAnimation.Clockwise
            duration: 500
            loops: Animation.Infinite
        }
    }
}
