import QtQuick 2.12
import QtQuick.Window 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.10
import UploadEffectController 1.0
import QmlUtils 1.0
import FramelessWindow 1.0
import "../widget"
import "../widget/Mouse.js" as Logic

FramelessWindow {
    id: uploadDialog
    property string effectPath: ""
    property string imageCoverPath: ""

    modality: Qt.ApplicationModal
    width: 600
    height: 320
    resizable: false
    onlyAllowListMovable: true
    allowTouchWidget: [titleRect, dialogTitle]
    Component.onCompleted: {
        console.log("Component.onCompleted effectPath:"+effectPath)
    }

    UploadEffectController{
        id:uploader

        onEffectInfoLoaded: {
            effectCnNameEdit.text = getEffectCnName();
            effectEnNameEdit.text = getEffectName();
            imageCoverPath = getEffectCoverPath();
            console.log("onEffectInfoLoaded:"+effectCnNameEdit.text+"  "+effectEnNameEdit.text)

        }

        onEffectTeamChanged: {
            templateTeamCombo.currentIndex = getEffectTeam()
            console.log("onEffectTeamChanged:"+getEffectTeam())

        }

        onEffectCategoryChanged: {
            effectCategoryCombo.currentIndex = getEffectCategory()
        }

        onEffectUploadResult: {
            if(code == 0){
                close()
                storeToastMgr.show(msg, 3000)
            }else{
                storeToastMgr.show(msg, 3000)
            }
        }
    }

    onEffectPathChanged: {
        console.log("onEffectPathChanged effectPath:"+effectPath)
        uploader.setEffectPath(effectPath)
    }

    FileUtils {
        id: fileUtils
    }

    Rectangle {
        color: "#15171C"
        radius: 4
        border.color: "#000000"
        border.width: 2
        anchors.fill: parent
        TitleBar {
            id: titleRect
            width: parent.width
            height: 42
            anchors.top: parent.top
            anchors.left: parent.left
            border.color: "#000000"
            border.width: 2
            color: "#000000"
            onCloseClicked: {
                uploader.cancel()
                uploadDialog.close()
            }
            Label {
                id: dialogTitle
                text: qsTr("Upload Effect")
                font.pixelSize: 12
                color: "#BFBFBF"
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
        }


        Item {
            id: scrollView
            width: parent.width
            height: 240
            anchors.top: titleRect.bottom
            clip: true

            Item{
                width: 200
                id:left
                height: parent.height

                Image{
                    id:imageCover
                    width: 160
                    height: 160
                    anchors.centerIn: parent
                    source: imageCoverPath
                    fillMode: Image.PreserveAspectCrop
                }

                Image{
                    id:imageUpload
                    width: 30
                    height: 30
                    anchors.bottom:imageCover.bottom
                    anchors.right: imageCover.right
                    source: "qrc:/image/assets/icon_upload.png"
                    fillMode: Image.PreserveAspectCrop
                }

                FileDialog {
                    id: coverSelectDialog
                    title: qsTr("Select cover image")
                    selectMultiple: false
                    nameFilters: ["Image files (*.png *.jpg *.jpeg *.webp)"] // *.mp3 *.wma *.aac *.ape *.flac *.wav
                    onAccepted: {
                        imageCoverPath = fileUrl
                    }
                }

                MouseArea{
                    id:mouseArea
                    anchors.fill: imageCover
                    onClicked: {
                        coverSelectDialog.open();
                    }

                }

                DropArea {
                    id: dropVideoArea
                    anchors.fill: imageCover
                    onDropped: {
                        if (drop.hasUrls) {
                            for (var i = 0; i < drop.urls.length; i++) {
                                if (fileUtils.isImageExtension(drop.urls[i])) {
                                    imageCoverPath = drop.urls[i]
                                }
                            }
                        }
                    }
                }

            }

            Rectangle{
                id:divider
                width: 1
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.topMargin: 40
                anchors.bottomMargin: 40

                anchors.left:left.right
                color:"#333333"

            }


            ColumnLayout {
                id: exportRect
                width: 400
                anchors.left:divider.right
                anchors.top:parent.top
                anchors.topMargin: 20
                height: implicitHeight

                Item {
                    id: templateItem
                    height: 80
                    Layout.leftMargin:20
                    Layout.rightMargin:20
                    Label {
                        id: effectNameLabel
                        height: 24
                        width: 100
                        text: qsTr("Effect name")
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: 12
                        color: "#FFFFFF"
                        anchors.top: parent.top
                        anchors.topMargin: 10
                    }
                    TextField {
                        id: effectCnNameEdit
                        implicitWidth: 200
                        implicitHeight: 24
                        anchors.left: effectNameLabel.right
                        anchors.leftMargin: 20
                        anchors.top: parent.top
                        anchors.topMargin: 10
                        color: "#BFBFBF"
                        selectionColor: "#5465FF"
                        selectedTextColor: "#FFFFFF"
                        selectByMouse: true
                        placeholderText: qsTr("Chinese name")
                        text: uploader.getEffectCnName()
                        leftPadding: 6
                        topPadding: 0

                        bottomPadding: 0
                        background: Rectangle {
                            color: "#2D2F33"
                            radius: 2
                        }
                        onTextEdited: {
                        }
                    }
                    TextField {
                        id: effectEnNameEdit
                        implicitWidth: 200
                        implicitHeight: 24
                        anchors.left: effectCnNameEdit.left
                        anchors.top: effectCnNameEdit.bottom
                        anchors.topMargin: 10
                        color: "#BFBFBF"
                        selectionColor: "#5465FF"
                        selectedTextColor: "#FFFFFF"
                        selectByMouse: true
                        placeholderText: qsTr("English name")
                        text: uploader.getEffectName()
                        leftPadding: 6
                        topPadding: 0
                        bottomPadding: 0
                        background: Rectangle {
                            color: "#2D2F33"
                            radius: 2
                        }
                        onTextEdited: {
                        }
                    }
                }
                Item {
                    height: 40
                    width: parent.width
                    Layout.leftMargin:20
                    Layout.rightMargin:20

                    Label {
                        id: effectCategoryLabel
                        height: 24
                        width: 100
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("Effect Category")
                        font.pixelSize: 12
                        color: "#FFFFFF"
                    }
                    BasicComboBox {
                        id: effectCategoryCombo
                        height: 24
                        anchors.left: effectCategoryLabel.right
                        anchors.verticalCenter: effectCategoryLabel.verticalCenter
                        anchors.leftMargin: 20
                        width: 200
                        editable: false
                        backgroundColor: "#2D2F33"
                        itemNormalColor: "#2D2F33"
                        textColor: "#BFBFBF"
                        borderColor: "transparent"
                        font.pixelSize: 12
                        itemHighlightColor: "#2D2F33"
                        itemHeight: 26
                        model: uploader.effectCates
                        currentIndex: uploader.getEffectCategory()
                    }
                }

                Item {
                    height: 40
                    width: parent.width
                    Layout.leftMargin:20
                    Layout.rightMargin:20
                    Label {
                        id: templateTeamLabel
                        height: 24
                        width: 100
                        anchors.left: parent.left
                        verticalAlignment: Text.AlignVCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: qsTr("Upload Location")
                        font.pixelSize: 12
                        color: "#FFFFFF"
                    }
                    BasicComboBox {
                        id: templateTeamCombo
                        height: 24
                        anchors.left: templateTeamLabel.right
                        anchors.verticalCenter: templateTeamLabel.verticalCenter
                        anchors.leftMargin: 20
                        width: 200
                        editable: false
                        backgroundColor: "#2D2F33"
                        itemNormalColor: "#2D2F33"
                        textColor: "#BFBFBF"
                        borderColor: "transparent"
                        font.pixelSize: 12
                        itemHighlightColor: "#2D2F33"
                        itemHeight: 26
                        model: uploader.effectTeams
                        currentIndex: uploader.getEffectTeam()
                    }
                }

                Item{
                    width: 0
                    Layout.fillHeight: true
                }


            }
        }


        Rectangle {
            id: bottomRect
            width: parent.width - 4
            height: 50
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 2
            color: "#212328"

            Button {
                id: uploadBtn
                width: 70
                height: 26
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.rightMargin: 22
                anchors.bottomMargin: 11
                text: qsTr("Upload")
                style: 1
                onClicked: {
                    uploader.uploadEffect(effectEnNameEdit.text,effectCnNameEdit.text,imageCoverPath,effectCategoryCombo.currentIndex,templateTeamCombo.currentIndex)
                }
            }
        }


    }

    Shortcut {
        sequence: "Esc"
        onActivated: close()
    }

    Shortcut {
        sequence: "Enter"
        onActivated: uploader.uploadEffect(effectEnNameEdit.text,effectCnNameEdit.text,"",0,templateTeamCombo.currentIndex)
    }
}
