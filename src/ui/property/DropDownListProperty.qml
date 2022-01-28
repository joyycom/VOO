import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import "../widget/"
import DropdownListComponent 1.0

Item {
    width:parent.width
    height: 40

    property var isBinding: false


    DropdownListComponent{
        id:dropdownComponent

        onBindComponent: {
            isBinding = true
            dropdownComboBoxTitle.text = dropdownComponent.dropdownText()
            dropdownComboBox.model = dropdownComponent.dataList
            console.log("onBindComponent:"+dropdownComponent.dataList)
            dropdownComboBox.currentIndex = dropdownComponent.curDataIndex()
            isBinding = false
        }

        onCurDataIndexChanged: {
            dropdownComboBox.currentIndex = dropdownComponent.curDataIndex()
        }
    }

    Item{
        width: parent.width
        Layout.fillWidth: true
        height: 30
        Text {
            id: dropdownComboBoxTitle
            width: 52
            height: parent.height
            text: ""
            color: "#FFFFFF"
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 12
            anchors.verticalCenter: parent.verticalCenter
        }

        BasicComboBox {
            id:dropdownComboBox
            editable: false
            height:parent.height
            width:160
            anchors.leftMargin: 20
            anchors.left: dropdownComboBoxTitle.right
            anchors.top:dropdownComboBoxTitle.top
            backgroundColor: "#2D2F33"
            itemNormalColor: "#2D2F33"
            borderColor: "transparent"
            itemHighlightColor: "#2D2F33"
            itemHeight:26
            showCount: 15
            onCurrentIndexChanged: {
                if(!isBinding){
                    dropdownComponent.selectDataIndex(currentIndex)
                }
            }
        }
    }


    Component.onCompleted: {
        console.log("Component==DropdownList:"+index + " DropdownListComponent:"+dropdownComponent)
        onComponentLoaded(dropdownComponent)
    }
}
