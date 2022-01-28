import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import FramelessWindow 1.0

FramelessWindow {
    id: progressDialog
    objectName: "ProgressDialog"
    width: 200
    height: 200
    resizable: false
    movable: false
    modality: Qt.ApplicationModal
    property var progressText: qsTr("Rewinding the Video")
    color: "#15171C"
    title: progressText

    Connections {
        target: editorDatasource
        onHideProgressDialog: {
            close()
            console.log("onHideProgressDialog:")
        }
        onUpdateProgressDialog: {
            if (progress == 0) {
                if (mainText.text.length - progressText.length > 3) {
                    mainText.text = progressText
                } else {
                    mainText.text = mainText.text + "."
                }
            } else {
                mainText.text = progressText + "... " + Math.floor(
                            progress * 100) + " %"
            }
        }
    }

    Image {
        id: loading
        width: 80
        height: 80
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        source: "qrc:/image/assets/skyeditor.png"
    }

    Text {
        id: mainText
        width: parent.width
        height: 20
        text: progressText
        color: "#ffffff"
        font.pixelSize: 14
        anchors {
            top: loading.bottom
            topMargin: 20
        }
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    Button {
        width: 70
        height: 30
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 10
        hoverEnabled: true
        anchors.bottomMargin: 15
        text: qsTr("Cancel")
        onClicked: {
            editorDatasource.cancelProgressDialog(1)
        }
    }
}
