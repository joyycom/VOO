import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
import "../widget"
import EffectInputListComponent 1.0

Item {
    width:parent.width
    height: 40

    property var isBinding: false

    EffectInputListComponent{
        id:inputListComponent

        onBindComponent: {
            isBinding = true
            inputListComboBoxTitle.text = inputListComponent.dropdownText()
            inputListComboBox.model = inputListComponent.inputTypes()
            console.log("onBindComponent1:"+inputListComboBox.currentIndex)

            inputListComboBox.currentIndex = inputListComponent.getInputType()
            inputThumb.source = "image://skyimage/" + getCustomResPath()+"?type=2&width=40&height=40";

            isBinding = false;
            console.log("onBindComponent2:"+inputListComboBox.currentIndex)

        }

        onCustomResPathChanged: {
            inputThumb.source = "image://skyimage/" + getCustomResPath()+"?type=2&width=40&height=40";
        }

    }

    Item{
        width: parent.width
        Layout.fillWidth: true
        height: 30
        Text {
            id: inputListComboBoxTitle
            width: 52
            height: parent.height
            text: ""
            color: "#FFFFFF"
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }

        BasicComboBox {
            id:inputListComboBox
            editable: false
            height:parent.height
            width:100
            anchors.leftMargin: 20
            anchors.left: inputListComboBoxTitle.right
            anchors.top:inputListComboBoxTitle.top
            backgroundColor: "#2D2F33"
            itemNormalColor: "#2D2F33"
            borderColor: "transparent"
            itemHighlightColor: "#2D2F33"
            itemHeight:26
            showCount: 15
            onCurrentIndexChanged: {
                if(isBinding){
                    return;
                }
                console.log("onCurrentIndexChanged:"+currentIndex)

                inputListComponent.setInputTypeIndex(currentIndex)
            }
        }

        Image{
            id:inputThumb
            width: 30
            height:30
            source:""
            sourceSize.height: 30
            sourceSize.width: 30
            cache: false
            visible: inputListComboBox.currentText === "path"
            Layout.leftMargin: 20
            anchors.left: inputListComboBox.right
            fillMode: Image.PreserveAspectCrop
        }

        Button{
            height:30
            width: 80
            visible: inputListComboBox.currentText === "path"
            text:qsTr("Select")
            anchors.left: inputThumb.right
            anchors.leftMargin: 10
            onClicked: {
                selectImgDialog.open()
            }
        }

    }


    FileDialog {
        id: selectImgDialog
        title: qsTr("Select an media file")
        nameFilters: ["Image Files (*.bmp *.png *.jpg *.jepg *.mp4)"]
        onAccepted: {
            inputListComponent.setCustomResPath(fileUrl)
        }
    }


    Component.onCompleted: {
        console.log("Component==DropdownList:"+index + " DropdownListComponent:"+inputListComponent)
        onComponentLoaded(inputListComponent)
    }
}
