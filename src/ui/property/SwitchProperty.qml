import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import "../widget/"
import SwitchComponent 1.0

Item {
    width:parent.width
    height: 40


    SwitchComponent{
        id:switchComponent

        onBindComponent: {
            switchCheckBox.text = switchComponent.switchText()
            switchCheckBox.checked = switchComponent.switchValue()
            console.log("onBindComponent After:"+ siliderValue + " Text: "+switchCheckBox.value)
        }
        onSwitchValueChanged: {
            switchCheckBox.checked = switchComponent.switchValue()
        }

    }

    BasicCheckBox{
        id:switchCheckBox
        height: 25
        width: 80
        Layout.fillWidth: true
        text: ""
        indicatorWidth:10
        textColor: "#FFFFFF"
        backgroundColor:"#00000000"
        indicatorColor: "#666666"
        checked: false
        onCheckedChanged: {
            switchComponent.setSwitchValue(checked)
        }
    }

    Component.onCompleted: {
        console.log("Component==Switch:"+index + " switchComponent:"+switchComponent)
        onComponentLoaded(switchComponent)
    }
}
