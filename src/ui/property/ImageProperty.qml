import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import "../widget"
import ImageComponent 1.0

Item {
    property alias title: title.text
    property int paramIndex
    property int value
    width:parent.width
    height: 50

    ImageComponent{
        id:imageComponent

        onBindComponent: {
            image.source = "image://skyimage/" + imageComponent.imagePath()+"?type=2&width=40&height=40";
            title.text = imageComponent.labelText()
        }

        onImagePathUpdate: {
            image.source = "image://skyimage/" + imageComponent.imagePath()+"?type=2&width=40&height=40";
        }

    }

    RowLayout {
        id:imageLayout
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: editPropertyPanelHMargin
        anchors.rightMargin: editPropertyPanelHMargin

        Text{
            id:title
            text:""
            width: 16
            height: 20
            color:"#FFFFFF"
        }

        Image{
            width: 40
            height:40
            source:""
            sourceSize.height: 40
            sourceSize.width: 30
            id:image
            cache: false
            Layout.leftMargin: 20
            fillMode: Image.PreserveAspectCrop
        }

        Rectangle{
            color: "#2D2F33"
            radius:2
            width:48
            height:20

            Text{
                anchors.fill: parent
                text:qsTr("Select")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color:"#BFBFBF"
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    selectImgDialog.open()
                }
            }
        }
        Item{
            Layout.fillWidth: true
        }
    }

    FileDialog {
        id: selectImgDialog
        title: qsTr("Select an Image file")
        nameFilters: ["Image Files (*.bmp *.png *.jpg *.jepg)"]
        onAccepted: {
            imageComponent.updateImage(fileUrl)
        }
    }
    Component.onCompleted: {
        onComponentLoaded(imageComponent)
    }
}
