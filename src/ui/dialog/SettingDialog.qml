import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import FramelessWindow 1.0
import "../widget"

FramelessWindow {
    property int selectWidth: 0
    property int selectHeight: 0
    property int lockWidth: 0
    property int lockHeight: 0
    id: settingDialog
    modality: Qt.ApplicationModal
    width: 548
    height: 360
    resizable: false
    onlyAllowListMovable: true
    allowTouchWidget: [titleRect, dialogTitle]
    Rectangle {
        color: "#15171C"
        radius: 4
        border.color: "#000000"
        border.width: 2
        anchors.fill: parent
        TitleBar {
            id: titleRect
            width: parent.width - 4
            height: 42
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 2
            anchors.leftMargin: 2
            color: "#000000"
            onCloseClicked: {
                settingDialog.close()
            }
            Label {
                id: dialogTitle
                text: qsTr("Video Settings")
                font.pixelSize: 12
                color: "#BFBFBF"
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        Label {
            id: label1
            height: 20
            anchors.left: parent.left
            anchors.top: titleRect.bottom
            anchors.leftMargin: 60
            anchors.topMargin: 30
            text: qsTr("Preset Aspect Ratio")
            font.pixelSize: 12
            color: "#FFFFFF"
        }
        BasicComboBox {
            id: combo1
            width: 280
            height: 20
            anchors.left: label1.right
            anchors.verticalCenter: label1.verticalCenter
            anchors.leftMargin: 20
            editable: false
            backgroundColor: "#2D2F33"
            itemNormalColor: "#2D2F33"
            borderColor: "transparent"
            itemHighlightColor: "#2D2F33"
            itemHeight: 26
            model: ["1:1 1080*1080", "16:9 1920*1080", "9:16 1080*1920", "4:3 1440*1080", "3:4 1080*1440"]
            onCurrentIndexChanged: {
                switch (currentIndex) {
                case 0:
                    selectWidth = 1080
                    selectHeight = 1080
                    break
                case 1:
                    selectWidth = 1920
                    selectHeight = 1080
                    break
                case 2:
                    selectWidth = 1080
                    selectHeight = 1920
                    break
                case 3:
                    selectWidth = 1440
                    selectHeight = 1080
                    break
                case 4:
                    selectWidth = 1080
                    selectHeight = 1440
                    break
                default:
                    break
                }
                if (currentIndex >= 0) {
                    widthEdit.text = selectWidth
                    heightEdit.text = selectHeight
                    if (widthHeightCheckBox.checked) {
                        lockWidth = selectWidth
                        lockHeight = selectHeight
                    }
                }
            }
        }
        Label {
            id: combo1Text
            anchors.left: combo1.left
            anchors.leftMargin: 6
            anchors.verticalCenter: combo1.verticalCenter
            text: qsTr("Customize")
            font.pixelSize: 12
            color: "#BFBFBF"
            visible: combo1.currentIndex == -1
        }
        Label {
            id: label2
            height: 20
            anchors.left: label1.left
            anchors.top: label1.bottom
            anchors.topMargin: 25
            text: qsTr("Width")
            font.pixelSize: 12
            color: "#FFFFFF"
        }
        TextField {
            id: widthEdit
            implicitWidth: 48
            implicitHeight: 20
            padding: 0
            anchors.left: label2.right
            anchors.verticalCenter: label2.verticalCenter
            anchors.leftMargin: 20
            font.pixelSize: 12
            color: "#BFBFBF"
            selectionColor: "#5465FF"
            selectedTextColor: "#FFFFFF"
            verticalAlignment: TextEdit.AlignVCenter
            horizontalAlignment: TextEdit.AlignHCenter
            selectByMouse: true
            validator: RegExpValidator {
                regExp: /[0-9]+/
            }
            maximumLength: 5
            background: Rectangle {
                color: "#2D2F33"
                radius: 2
            }
            onTextEdited: {
                combo1.currentIndex = -1
                if (widthHeightCheckBox.checked && lockWidth > 0
                        && lockHeight > 0) {
                    var newHeight = Math.floor(text) / (lockWidth / lockHeight)
                    if (Math.floor(heightEdit.text) !== newHeight) {
                        heightEdit.text = newHeight
                    }
                }
            }
        }
        Label {
            id: widthPx
            text: "px"
            font.pixelSize: 12
            color: "#BFBFBF"
            anchors.left: widthEdit.right
            anchors.verticalCenter: label2.verticalCenter
            anchors.leftMargin: 5
        }

        Label {
            id: label3
            height: 20
            anchors.left: widthPx.right
            anchors.verticalCenter: label2.verticalCenter
            anchors.leftMargin: 30
            anchors.topMargin: 25
            text: qsTr("Height")
            font.pixelSize: 12
            color: "#FFFFFF"
        }
        TextField {
            id: heightEdit
            implicitWidth: 48
            implicitHeight: 20
            padding: 0
            anchors.left: label3.right
            anchors.verticalCenter: label2.verticalCenter
            anchors.leftMargin: 20
            font.pixelSize: 12
            color: "#BFBFBF"
            selectionColor: "#5465FF"
            selectedTextColor: "#FFFFFF"
            verticalAlignment: TextEdit.AlignVCenter
            horizontalAlignment: TextEdit.AlignHCenter
            selectByMouse: true
            validator: RegExpValidator {
                regExp: /[0-9]+/
            }
            maximumLength: 5
            background: Rectangle {
                color: "#2D2F33"
                radius: 2
            }
            onTextEdited: {
                combo1.currentIndex = -1
                if (widthHeightCheckBox.checked && lockWidth > 0
                        && lockHeight > 0) {
                    var newWidth = Math.floor(text) * (lockWidth / lockHeight)
                    if (Math.floor(widthEdit.text) !== newWidth) {
                        widthEdit.text = newWidth
                    }
                }
            }
        }
        Label {
            id: heightPx
            text: "px"
            font.pixelSize: 12
            color: "#BFBFBF"
            anchors.left: heightEdit.right
            anchors.leftMargin: 5
            anchors.verticalCenter: label2.verticalCenter
        }

        BasicCheckBox {
            id: widthHeightCheckBox
            height: 16
            text: qsTr("Lock Aspect Ratio")
            indicatorWidth: 10
            anchors.left: heightPx.right
            anchors.leftMargin: 30
            anchors.verticalCenter: label2.verticalCenter
            textColor: "#BFBFBF"
            backgroundColor: "transparent"
            indicatorColor: "#BFBFBF"
            radius: 1
            checked: true
            onCheckedChanged: {
                if (checked) {
                    lockWidth = Math.floor(widthEdit.text)
                    lockHeight = Math.floor(heightEdit.text)
                }
            }
        }
        Label {
            id: label4
            height: 20
            anchors.left: label1.left
            anchors.top: label2.bottom
            anchors.topMargin: 25
            text: qsTr("Frame Rate")
            font.pixelSize: 12
            color: "#FFFFFF"
        }

        BasicComboBox {
            id: combo2
            width: 60
            height: 20
            anchors.left: label4.right
            anchors.verticalCenter: label4.verticalCenter
            anchors.leftMargin: 20
            editable: false
            backgroundColor: "#2D2F33"
            itemNormalColor: "#2D2F33"
            borderColor: "transparent"
            itemHighlightColor: "#2D2F33"
            itemHeight: 26
            model: ["15", "24", "30", "60"]
            currentIndex: 2
        }

        Label {
            text: qsTr("Frames/s")
            font.pixelSize: 12
            color: "#BFBFBF"
            anchors.left: combo2.right
            anchors.verticalCenter: label4.verticalCenter
            anchors.leftMargin: 5
        }
        Label {
            id: label5
            height: 20
            anchors.left: label1.left
            anchors.top: label4.bottom
            anchors.topMargin: 25
            text: qsTr("Background Color")
            font.pixelSize: 12
            color: "#FFFFFF"
            visible: false
        }
        Rectangle {
            id: colorRect
            width: 20
            height: 20
            color: "#000000"
            anchors.left: label5.right
            anchors.verticalCenter: label5.verticalCenter
            anchors.leftMargin: 24
            visible: false
            Rectangle {
                anchors.fill: parent
                color: "transparent"
                border.color: "#33FFFFFF"
                border.width: 1
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    colorDialog.color = parent.color
                    colorDialog.open()
                }
            }
        }
        ColorDialog {
            id: colorDialog
            title: qsTr("Choose the Color")
            visible: false
            onColorChanged: {
                colorRect.color = color
            }
        }
        Button {
            id: okBtn
            width: 70
            height: 26
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 30
            anchors.bottomMargin: 30
            text: qsTr("Confirm")
            style: 1
            onClicked: {
                close()
                var selectIndex = combo1.currentIndex
                var width = Math.floor(widthEdit.text)
                var height = Math.floor(heightEdit.text)
                var fpsIndex = combo2.currentIndex
                videoPreviewItem.previewFrame.frameScale
                        = videoPreviewItem.previewController.changeVideoConfig(
                            width, height, fpsIndex)
                setPreviewFrame(selectIndex)
                videoPreviewItem.previewFrame.frameWidth = width
                videoPreviewItem.previewFrame.frameHeight = height
            }
        }

        Button {
            id: cancelBtn
            width: 70
            height: 26
            anchors.right: okBtn.left
            anchors.bottom: parent.bottom
            anchors.rightMargin: 10
            anchors.bottomMargin: 30
            text: qsTr("Cancel")
            onClicked: {
                close()
            }
        }
    }
    function setPreviewFrame(index) {
        videoPreviewItem.previewFrame.select = index
        switch (index) {
        case -1:
            videoPreviewItem.previewFrame.selectText = qsTr("Customize")
            break
        case 0:
            videoPreviewItem.previewFrame.selectText = "1:1"
            break
        case 1:
            videoPreviewItem.previewFrame.selectText = "16:9"
            break
        case 2:
            videoPreviewItem.previewFrame.selectText = "9:16"
            break
        case 3:
            videoPreviewItem.previewFrame.selectText = "4:3"
            break
        case 4:
            videoPreviewItem.previewFrame.selectText = "3:4"
            break
        default:
            break
        }
    }
    Component.onCompleted: {
        var config = videoPreviewItem.previewController.getFrameConfig()
        combo1.currentIndex = config[0]
        combo2.currentIndex = config[1]
        widthEdit.text = config[2]
        heightEdit.text = config[3]
        //colorRect.color = config[4]
    }
    Shortcut {
        sequences: ["Enter", "Return"]
        onActivated: {
            okBtn.clicked()
        }
    }
}
