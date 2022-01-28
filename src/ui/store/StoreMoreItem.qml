import QtQuick 2.12
import "Store.js" as Logic
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import "../basicmenubar"

Item {
    id: itemRoot
    width: 90
    height: 110
    property bool isItemHover: false
    property bool addItemPressed: false
    property bool isOverlap: false
    property alias isMouseSelected: iconContainer.isMouseSelected
    property alias selectCount: iconContainer.selectCount

    function dragEnable() {
        if (storeDragWindow) {
            return true
        } else {
            return false
        }
    }
    Rectangle {
        width: 84
        height: 104
        color: "transparent"
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 3
        }
        visible: index == 0

        Image {
            width: 84
            height: 84
            source: isItemHover ? "qrc:/image/assets/video_add_border_rectangle_hover.png" : "qrc:/image/assets/video_add_border_rectangle.png"
        }

        Image {
            id: add_more_icon
            width: 48
            height: 48
            source: isItemHover ? "qrc:/image/assets/video_add_hover.png" : "qrc:/image/assets/video_add.png"
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                topMargin: 8
            }
        }
        Text {
            text: qsTr("Import Folder")
            width: parent.width
            height: 20
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: add_more_icon.bottom
            }
            color: isItemHover ? "#FFFFFF" : "#BFBFBF"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                isItemHover = true
            }
            onExited: {
                isItemHover = false
            }
            onClicked: {
                //打开目录
                moreOpenDialog.open()
            }
        }
    }
    Rectangle {
        width: 84
        height: 104
        color: "transparent"
        visible: index != 0
        anchors.centerIn: parent

        MouseArea {
            width: parent.width
            height: parent.height
            drag.target: null
            hoverEnabled: true
            preventStealing: true
            onEntered: {
                isItemHover = true
            }
            onExited: {
                if (!addItemPressed) {
                    isItemHover = false
                }
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
                    if (storeDragWindow.item !== iconContainer.capture) {
                        storeDragWindow.item = iconContainer.capture
                    } else {
                        storeDragWindow.image.opacity = 1
                    }
                    timeLineRec.isStoreDrag = true
                    Logic.onPositionChanged(iconContainer.capture,
                                            storeDragWindow, mouse,
                                            startMouseX, startMouseY, "MORE")
                }
            }
            onReleased: {
                if (mouse.button !== Qt.LeftButton) {
                    return
                }
                if (!isMove) {
                    moreLocalList.focus = true
                    moreLocalList.selectIndex = index
                }
                if (dragEnable()) {
                    storeDragWindow.opacity = 0
                    storeDragWindow.image.opacity = 0
                    Logic.onReleased(
                                iconContainer.capture,
                                storeDragWindow, mouse, 3,
                                selectCount > 0 ? moreLocalList.selectList : [index])
                    timeLineRec.isStoreDrag = false
                }
            }
        }

        StoreContainer {
            id: iconContainer
            thumb {
                source: fileCover
                onStatusChanged: {
                    if (status === Image.Error) {
                        thumb.source = "qrc:/image/assets/icon_default_effect.png"
                    }
                    console.log("MoreItem thumb state chagne :" + state)
                }
            }
        }
        Text {
            text: fileName
            width: parent.width
            height: 20
            clip: true
            elide: Text.ElideRight
            anchors {
                bottom: parent.bottom
            }
            color: isItemHover ? "#FFFFFF" : "#BFBFBF"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
        }
        Image {
            id: more_add_icon
            width: 20
            height: 20
            anchors.top: iconContainer.top
            anchors.right: iconContainer.right
            anchors.topMargin: 4
            anchors.rightMargin: 4
            visible: isItemHover
            source: "qrc:/image/assets/res_add.png"
            MouseArea {
                anchors.fill: parent
                onPressed: {
                    addItemPressed = true
                }
                onReleased: {
                    addItemPressed = false
                }
                onClicked: {
                    moreLocalList.focus = false
                    datasource.addLocalMoreResource([index])
                }
            }
        }
    }
}
