import QtQuick 2.12
import QtQuick.Controls 2.12
import "../basicmenubar"

Rectangle {
    property int select: 0
    property var selectText: "1:1"
    property real frameScale: 1
    property int frameWidth: 1
    property int frameHeight: 1
    width: 40
    height: 18
    color: "#15171C"
    radius: 2
    border.color: "white"
    border.width: 1
    Text {
        id: contentText
        text: selectText
        font.pixelSize: 12
        color: "white"
        anchors.centerIn: parent
    }
    BasicMenu {
        id: menu
        topPadding: 5
        bottomPadding: 5
        background: Rectangle {
            implicitWidth: 110
            color: "#2D2F33"
        }
        BasicMenuItem {
            height: 26
            buttonColor: highlighted ? "#0DFFFFFF" : "#2D2F33"
            background: Rectangle {
                x: 5
                width: parent.width - x * 2
                height: parent.height
                color: parent.buttonColor
                radius: 2
            }
            contentItem: Item {
                Image {
                    x: 16
                    y: 16
                    anchors.verticalCenter: text.verticalCenter
                    source: "qrc:/image/assets/res_checked.png"
                    visible: select == -1
                }
                Text {
                    id: text
                    x: 40
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Customize")
                    font.pixelSize: 12
                    color: select == -1 ? "#FFFFFF" : "#BFBFBF"
                }
            }
            onTriggered: {
                var component = Qt.createComponent(
                            "qrc:/ui/dialog/SettingDialog.qml")
                var settingDialog = component.createObject(editorPage)
                settingDialog.show()
            }
        }
        BasicMenuItem {
            height: 26
            buttonColor: highlighted ? "#0DFFFFFF" : "#2D2F33"
            background: Rectangle {
                x: 5
                width: parent.width - x * 2
                height: parent.height
                color: parent.buttonColor
                radius: 2
            }
            contentItem: Item {
                Image {
                    x: 16
                    y: 16
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/image/assets/res_checked.png"
                    visible: select == 0
                }
                Text {
                    x: 40
                    anchors.verticalCenter: parent.verticalCenter
                    text: "1:1"
                    font.pixelSize: 12
                    color: select == 0 ? "#FFFFFF" : "#BFBFBF"
                }
            }
            onTriggered: {
                select = 0
                selectText = "1:1"
                changeFrame()
            }
        }
        BasicMenuItem {
            height: 26
            buttonColor: highlighted ? "#0DFFFFFF" : "#2D2F33"
            background: Rectangle {
                x: 5
                width: parent.width - x * 2
                height: parent.height
                color: parent.buttonColor
                radius: 2
            }
            contentItem: Item {
                Image {
                    x: 16
                    y: 16
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/image/assets/res_checked.png"
                    visible: select == 1
                }
                Text {
                    x: 40
                    anchors.verticalCenter: parent.verticalCenter
                    text: "16:9"
                    font.pixelSize: 12
                    color: select == 1 ? "#FFFFFF" : "#BFBFBF"
                }
            }
            onTriggered: {
                select = 1
                selectText = "16:9"
                changeFrame()
            }
        }
        BasicMenuItem {
            height: 26
            buttonColor: highlighted ? "#0DFFFFFF" : "#2D2F33"
            background: Rectangle {
                x: 5
                width: parent.width - x * 2
                height: parent.height
                color: parent.buttonColor
                radius: 2
            }
            contentItem: Item {
                Image {
                    x: 16
                    y: 16
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/image/assets/res_checked.png"
                    visible: select == 2
                }
                Text {
                    x: 40
                    anchors.verticalCenter: parent.verticalCenter
                    text: "9:16"
                    font.pixelSize: 12
                    color: select == 2 ? "#FFFFFF" : "#BFBFBF"
                }
            }
            onTriggered: {
                select = 2
                selectText = "9:16"
                changeFrame()
            }
        }

        BasicMenuItem {
            height: 26
            buttonColor: highlighted ? "#0DFFFFFF" : "#2D2F33"
            background: Rectangle {
                x: 5
                width: parent.width - x * 2
                height: parent.height
                color: parent.buttonColor
                radius: 2
            }
            contentItem: Item {
                Image {
                    x: 16
                    y: 16
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/image/assets/res_checked.png"
                    visible: select == 3
                }
                Text {
                    x: 40
                    anchors.verticalCenter: parent.verticalCenter
                    text: "4:3"
                    font.pixelSize: 12
                    color: select == 3 ? "#FFFFFF" : "#BFBFBF"
                }
            }
            onTriggered: {
                select = 3
                selectText = "4:3"
                changeFrame()
            }
        }
        BasicMenuItem {
            height: 26
            buttonColor: highlighted ? "#0DFFFFFF" : "#2D2F33"
            background: Rectangle {
                x: 5
                width: parent.width - x * 2
                height: parent.height
                color: parent.buttonColor
                radius: 2
            }
            contentItem: Item {
                Image {
                    x: 16
                    y: 16
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/image/assets/res_checked.png"
                    visible: select == 4
                }
                Text {
                    x: 40
                    anchors.verticalCenter: parent.verticalCenter
                    text: "3:4"
                    font.pixelSize: 12
                    color: select == 4 ? "#FFFFFF" : "#BFBFBF"
                }
            }
            onTriggered: {
                select = 4
                selectText = "3:4"
                changeFrame()
            }
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            menu.x = -(menu.width - parent.width) / 2
            menu.y = -(menu.height + 8)
            menu.open()
        }
    }
    function setPreviewFrame(index) {
        select = index
        switch (index) {
        case -1:
            selectText = qsTr("Customize")
            break
        case 0:
            selectText = "1:1"
            break
        case 1:
            selectText = "16:9"
            break
        case 2:
            selectText = "9:16"
            break
        case 3:
            selectText = "4:3"
            break
        case 4:
            selectText = "3:4"
            break
        default:
            break
        }
    }
    function changeFrame() {
        var wh = previewController.changeFrame(select)
        frameWidth = wh[0]
        frameHeight = wh[1]
        if (frameHeight > 0) {
            frameScale = frameWidth / frameHeight
        }
        frameScale = frameWidth / frameHeight
    }
    function frameConfig(config) {
        setPreviewFrame(config[0])
        frameWidth = config[2]
        frameHeight = config[3]
        if (frameHeight > 0) {
            frameScale = frameWidth / frameHeight
        }
    }
}
