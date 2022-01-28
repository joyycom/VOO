import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import TTSComponent 1.0
import "../widget"

Item {
    width:parent.width
    height: 30


    TTSComponent {
        id: ttsComponent

        onBindComponent: {
            console.log("onbind............")
        }
    }

    Text {
        id: labelLanguage
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: editPropertyPanelHMargin
        anchors.rightMargin: editPropertyPanelHMargin
        anchors.leftMargin: editPropertyPanelHMargin
        color: "#FFFFFF"
        text: qsTr("Language")
    }

    GridView {
        id: languageGridView
        anchors.margins: 4
        height: 30
        anchors.top: labelLanguage.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: editPropertyPanelHMargin
        anchors.rightMargin: editPropertyPanelHMargin
        cellWidth: 90
        model:ttsComponent.languageModel
        cellHeight: 30
        clip: true
        delegate: gridLanguageDelegate
    }

    Text {
        id: labelVoice
        anchors.top: languageGridView.bottom
        anchors.topMargin: 15
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: editPropertyPanelHMargin
        anchors.leftMargin: editPropertyPanelHMargin
        visible: voiceGridView.count > 0
        color: "#FFFFFF"
        text: qsTr("Voice")
    }

    GridView {
        id: voiceGridView
        anchors.margins: 4
        anchors.top: labelVoice.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: editPropertyPanelHMargin
        anchors.rightMargin: editPropertyPanelHMargin
        model: ttsComponent.voiceModel
        cellWidth: 85
        cellHeight: 85
        clip: true
        delegate: gridVoiceDelegate
    }

    Component {
        id: gridLanguageDelegate
        ColumnLayout {
            id: wrapper
            width: 90
            height: 25
            Rectangle {
                width: 84
                height: 25
                border.color: isSelected ? "#02E4D6" : "#000000"
                border.width: 1
                color: "#252629"

                Text {
                    Layout.alignment: Qt.AlignHCenter|Qt.AlignVCenter
                    id: title
                    text: name
                    anchors.centerIn: parent
                    color: isSelected ?"#FFFFFF":"#BFBFBF"

                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        ttsComponent.selectLanguageIndex(index)
                    }
                }
            }
        }
    }



    Component {
        id: gridVoiceDelegate
        ColumnLayout {
            id: wrapper
            width: 85
            height: 85
            Rectangle {
                width: 80
                height: 80
                border.color: isSelected ? "#02E4D6" : "#000000"
                border.width: 1
                color: "#252629"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("TTSComponent click:"+wrapper.parent+" "+index)
                        ttsComponent.selectVoiceIndex(index)
                    }
                }

                Image {
                    id: downloadIcon
                    width: 20
                    height: 20
                    anchors.right: parent.right
                    anchors.rightMargin: 4
                    anchors.top: parent.top
                    anchors.topMargin: 4
                    visible: isSelected && isDownloaded
                    source: "qrc:/image/assets/res_add.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            ttsComponent.addCurVoiceResource()
                        }
                    }
                }
                Image {
                    id: downloading_icon
                    anchors.fill: downloadIcon
                    visible: isSelected && !isDownloaded
                    source: "qrc:/image/assets/res_downloading.png"
                    onVisibleChanged: {
                        if(visible){
                            rotationAnimation.start();
                        }else{
                            rotationAnimation.stop();
                        }
                    }
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

                Text {
                    Layout.alignment: Qt.AlignHCenter|Qt.AlignVCenter
                    id: title
                    width: 80
                    height: 40
                    text: name
                    wrapMode: Text.WordWrap
                    maximumLineCount: 2
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    leftPadding: 2
                    rightPadding: 2
                    fontSizeMode: Text.Fit
                    anchors.centerIn: parent
                    color: isSelected ?"#FFFFFF":"#BFBFBF"

                }

            }
        }
    }

    Component.onCompleted: {
        onComponentLoaded(ttsComponent)

    }
}
