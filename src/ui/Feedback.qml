import QtQuick 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.10
import QmlUtils 1.0
import FeedBackDataSource 1.0
import FramelessWindow 1.0
import "./widget"

FramelessWindow {
    id: feedbackPage
    objectName: "FeedbackPage"
    modality: Qt.ApplicationModal
    width: 700
    height: 700
    resizable: false
    onlyAllowListMovable: true
    allowTouchWidget: [titleRect, dialogTitle]
    property int select: -1
    FileUtils {
        id: fileUtils
    }
    FeedBackDataSource {
        id: datasource
        onUploadFileChanged: {
            lsuggest.text = fileName
            llimitSize.text = qsTr("File Size: ") + fileSize
        }
        onStartedFeedback: {
            close()
        }
    }
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
                feedbackPage.close()
            }
            Label {
                id: dialogTitle
                text: qsTr("Customer Feedback")
                font.pixelSize: 12
                color: "#BFBFBF"
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Rectangle {
            id: problemPick
            anchors.top: titleRect.bottom
            anchors.left: parent.left
            anchors.topMargin: 20
            anchors.leftMargin: 30
            width: parent.width - 60
            height: parent.height / 3
            color: "#00BFBFBF"
            Rectangle {
                id: txt_problemPick
                height: 30
                color: "#00BFBFBF"
                anchors {
                    top: problemPick.top
                    left: problemPick.left
                    right: problemPick.right
                }
                Text {
                    id: txt_problemPick_1
                    height: parent.height
                    text: "*"
                    color: "red"
                    font.pixelSize: 14
                    anchors.left: txt_problemPick.left
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: txt_problemPick_2
                    height: parent.height
                    text: qsTr("Problem Scenario (Single): ")
                    color: "#BFBFBF"
                    font.pixelSize: 14
                    anchors.left: txt_problemPick_1.right
                    anchors.leftMargin: 5
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: picked
                    height: parent.height
                    text: ""
                    color: "#BFBFBF"
                    font.pixelSize: 14
                    anchors.left: txt_problemPick_2.right
                    anchors.leftMargin: 20
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }

            ListModel {
                id: theModel
                ListElement {
                    name: qsTr("Crash")
                }
                ListElement {
                    name: qsTr("Caton")
                }
                ListElement {
                    name: qsTr("Format Not Supported")
                }
                ListElement {
                    name: qsTr("Import Failed")
                }
                ListElement {
                    name: qsTr("Export Failed")
                }
                ListElement {
                    name: qsTr("Video Clip")
                }
                ListElement {
                    name: qsTr("Audio Clip")
                }
                ListElement {
                    name: qsTr("Text Edit")
                }
                ListElement {
                    name: qsTr("Subtitle Recognition")
                }
                ListElement {
                    name: qsTr("Materials Problem")
                }
                ListElement {
                    name: qsTr("Playback Problem")
                }
                ListElement {
                    name: qsTr("Other Problem")
                }
                ListElement {
                    name: qsTr("Suggestion")
                }
            }

            Rectangle {
                id: choiceList_problemPick
                width: parent.width
                height: parent.height
                anchors {
                    top: txt_problemPick.bottom
                    topMargin: 20
                    left: txt_problemPick.left
                    right: txt_problemPick.right
                }
                color: "#00BFBFBF"
                Flow {
                    id: fw
                    width: parent.width
                    height: parent.height - txt_problemPick.height - fw.anchors.margins
                    flow: Flow.LeftToRight
                    spacing: 20
                    Repeater {
                        model: theModel
                        delegate: Rectangle {
                            id: delRec
                            property bool entered: false
                            color: index == select ? "#ffffff" : entered ? "#dcdcdc" : "#BFBFBF"
                            radius: 15
                            width: problemScene.width + 25
                            height: 30
                            Text {
                                id: problemScene
                                text: name
                                anchors.centerIn: parent
                                font.pixelSize: 14
                                color: "#000000"
                            }
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onEntered: {
                                    delRec.entered = true
                                }
                                onExited: {
                                    delRec.entered = false
                                }

                                onClicked: {
                                    select = index
                                    picked.text = "\"" + problemScene.text + "\""
                                }
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            id: problemDescribe
            width: problemPick.width
            height: parent.height / 4
            anchors {
                top: problemPick.bottom
                topMargin: 15
                left: problemPick.left
            }
            color: "#00BFBFBF"
            Rectangle {
                id: txt_problemDescribe
                height: 30
                color: "#00BFBFBF"
                anchors {
                    top: problemDescribe.top
                    left: problemDescribe.left
                    right: problemDescribe.right
                }
                Text {
                    id: txt_problemDescribe_1
                    height: parent.height
                    text: "*"
                    color: "red"
                    font.pixelSize: 14
                    anchors.left: parent.left
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    id: txt_problemDescribe_2
                    width: parent.width
                    height: parent.height
                    text: qsTr("Problem Description: ")
                    color: "#BFBFBF"
                    font.pixelSize: 14
                    anchors.left: txt_problemDescribe_1.right
                    anchors.leftMargin: 5
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Rectangle {
                id: write_problemDescribe
                width: parent.width
                height: parent.height - txt_problemDescribe.height
                        - write_problemDescribe.anchors.topMargin
                anchors {
                    top: txt_problemDescribe.bottom
                    topMargin: 10
                    left: txt_problemDescribe.left
                }
                radius: 2
                color: "lightgray"
                TextEdit {
                    id: input1
                    anchors.fill: parent
                    anchors.margins: 8
                    font.pixelSize: 14
                    //focus: true
                    clip: true
                    selectByMouse: true
                    bottomPadding: 8
                    wrapMode: "WrapAtWordBoundaryOrAnywhere"
                    Text {
                        anchors.left: input1.left
                        anchors.leftMargin: 2
                        text: qsTr("A specific and clear description will help us solve your problem quickly")
                        color: "#aaa"
                        visible: !input1.text
                    }
                }
            }
        }

        FileDialog {
            id: fileInputVideoDialog
            title: qsTr("Select an image file")
            nameFilters: ["Img (*.bmp *.png *.jpg *.jepg))", "Video (*.mp4)"]
            folder: shortcuts.documents
            //fileMode: FileDialog.OpenFile
            onAccepted: {
                datasource.setUploadFile(fileInputVideoDialog.fileUrl)
            }
        }

        Rectangle {
            id: uploadNcontact
            width: problemPick.width
            height: parent.height / 4
            anchors {
                top: problemDescribe.bottom
                topMargin: 25
                left: problemDescribe.left
            }
            color: "#00BFBFBF"
            Rectangle {
                id: uploadRec
                width: parent.width / 2 - contactRec.anchors.leftMargin
                height: parent.height
                anchors.top: parent.top
                anchors.left: parent.left
                color: parent.color
                Rectangle {
                    id: txt_upload
                    width: parent.width
                    height: 30
                    anchors.top: parent.top
                    anchors.left: parent.left
                    color: parent.color
                    Text {
                        text: qsTr("Upload Image/Video: ")
                        color: "#BFBFBF"
                        font.pixelSize: 14
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Rectangle {
                    id: do_upload
                    width: parent.width - 20
                    height: parent.height - txt_upload.height - this.anchors.topMargin - 10
                    anchors {
                        top: txt_upload.bottom
                        topMargin: 5
                        left: txt_upload.left
                    }
                    radius: 2
                    color: "lightgray"

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            do_upload.color = "#dcdcdc"
                            img_upload.color = "#c0c0c0"
                        }
                        onExited: {
                            do_upload.color = "lightgray"
                            img_upload.color = "#BFBFBF"
                        }
                        onClicked: {
                            fileInputVideoDialog.open()
                        }
                    }
                    Rectangle {
                        id: inRec
                        anchors {
                            margins: 10
                            top: do_upload.top
                            bottom: do_upload.bottom
                            left: do_upload.left
                            right: do_upload.right
                        }
                        radius: parent.radius
                        color: parent.color
                        Rectangle {
                            id: img_upload
                            height: inRec.height
                            width: this.height
                            anchors {
                                left: inRec.left
                                top: inRec.top
                            }
                            color: "#BFBFBF"
                            radius: parent.radius

                            Image {
                                width: img_upload.width
                                height: img_upload.height
                                anchors {
                                    centerIn: parent
                                    margins: 3
                                }

                                source: "qrc:/image/assets/file.png"
                            }
                        }

                        Label {
                            id: lsuggest
                            height: 50
                            anchors {
                                left: img_upload.right
                                leftMargin: 10
                                right: inRec.right
                                top: inRec.top
                                topMargin: 5
                            }
                            text: qsTr("It is recommended to upload the Screen Recording to help locating problem more quickly")
                            font.pixelSize: 14
                            wrapMode: Text.WordWrap
                        }

                        Label {
                            id: llimitSize
                            anchors {
                                top: lsuggest.bottom
                                left: lsuggest.left
                                right: lsuggest.right
                                bottom: inRec.bottom
                            }
                            font.pixelSize: 10
                            text: qsTr("Upload limit: Within 50MB")
                            color: "gray"
                        }
                    }
                }
            }

            Rectangle {
                id: contactRec
                width: parent.width / 2
                height: parent.height
                anchors.top: parent.top
                anchors.left: uploadRec.right
                anchors.leftMargin: 10
                color: parent.color
                Rectangle {
                    id: txt_contact
                    width: parent.width
                    height: 30
                    anchors.top: parent.top
                    anchors.left: parent.left
                    color: parent.color
                    Text {
                        text: qsTr("Contact Info (QQ, WeChat, Phone): ")
                        color: "#BFBFBF"
                        font.pixelSize: 14
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                    }
                }

                Rectangle {
                    id: num_contact
                    width: parent.width
                    height: 35
                    anchors.top: txt_contact.bottom
                    anchors.topMargin: 5
                    anchors.left: parent.left
                    color: "lightgray"
                    radius: 2
                    TextInput {
                        id: input2
                        anchors.fill: parent
                        anchors.margins: 8
                        font.pixelSize: 14
                        clip: true
                        selectByMouse: true
                        maximumLength: 15
                        Text {
                            anchors.left: input2.left
                            anchors.leftMargin: 2
                            text: qsTr("QQ numbers Prefered")
                            color: "#aaa"
                            visible: !input2.text
                        }
                        validator: RegExpValidator {
                            regExp: /[0-9]+/
                        }
                    }
                }

                Rectangle {
                    id: confirm
                    width: parent.width
                    anchors {
                        bottom: contactRec.bottom
                        bottomMargin: 10
                        right: contactRec.right
                        top: num_contact.bottom
                        topMargin: 15
                    }
                    color: parent.color
                    Button {
                        id: btn_confirm
                        width: parent.width / 2 - 5
                        height: 40
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        text: qsTr("Confirm")
                        style: 1
                        onClicked: {
                            isReady()
                        }
                    }

                    Button {
                        id: btn_cancel
                        width: parent.width / 2 - 5
                        height: 40
                        anchors.right: btn_confirm.left
                        anchors.rightMargin: 10
                        anchors.bottom: parent.bottom
                        text: qsTr("Cancel")
                        onClicked: {
                            close()
                        }
                    }
                }
            }
        }
    }

    MessageDialog {
        id: messageDialog
        visible: false
    }

    function showMessage(content) {
        messageDialog.title = content
        messageDialog.text = content
        messageDialog.visible = true
    }

    function isReady() {
        if (picked.text === "") {
            showMessage(qsTr("Please select the Problem Scenario"))
            return
        }
        if (!input1.text) {
            showMessage(qsTr("Please fill in the Problem Description"))
            return
        }
        datasource.startFeedback(picked.text, input1.text, input2.text)
    }
}
