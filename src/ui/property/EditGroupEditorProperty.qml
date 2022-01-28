import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import "../widget/"
import EffectGroupEditorComponent 1.0

Item {
    width:parent.width
    height: 40


    EffectGroupEditorComponent{
        id:effectGroupComponent

        onBindComponent: {
            var curGroup = effectGroupComponent.getCurGroup()
            dropdownComboBoxTitle.text = effectGroupComponent.dropdownText()
            dropdownComboBox.model = effectGroupComponent.dataList()
            dropdownComboBox.currentIndex = dropdownComboBox.model.indexOf(curGroup)
            console.log("onBindComponent:"+effectGroupComponent.dataList())
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
            editable: true
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
            onCurrentTextChanged: {
                console.log("onCurrentTextChanged:"+currentText)

                effectGroupComponent.setCurGroup(currentText)
            }

            onEditTextChanged: {
                console.log("onEditTextChanged:"+editText)

                effectGroupComponent.setCurGroup(editText)
            }

        }
    }


    Component.onCompleted: {
        console.log("Component==DropdownList:"+index + " EffectGroupEditorComponent:"+effectGroupComponent)
        onComponentLoaded(effectGroupComponent)
    }
}
