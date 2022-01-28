import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
import "../widget"
import ParticleFileParserComponent 1.0

Item {
    width:parent.width
    height: 40

    ParticleFileParserComponent{
        id:fileParserComponent

        onBindComponent: {
            title.text = labelText()
        }

    }

    Item{
        width: parent.width
        Layout.fillWidth: true
        height: 30
        Text {
            id: title
            width: 80
            height: parent.height
            text: ""
            color: "#FFFFFF"
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }


        Button{
            height:30
            width: 80
            visible: true
            text:qsTr("Select")
            anchors.left: title.right
            anchors.leftMargin: 10
            onClicked: {
                selectFileDialog.open()
            }
        }

    }


    FileDialog {
        id: selectFileDialog
        title: qsTr("Select an particle config file")
        nameFilters: ["Config Files (*.plist *.*)"]
        onAccepted: {
            fileParserComponent.setConfigFile(fileUrl)
        }
    }


    Component.onCompleted: {
        onComponentLoaded(fileParserComponent)
    }
}
