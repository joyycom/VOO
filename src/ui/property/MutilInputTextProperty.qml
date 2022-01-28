import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import "../widget"
import MutliInputTextComponent 1.0

Item {
    clip: true
    height: 120
    width:parent.width
    property var preText: ""


    MutliInputTextComponent {
        id: inputComponent
        onBindComponent: {
            inputTitle.text = labelText()
            inputTextField.text = getText()
        }

    }

    Text {
        id: inputTitle
        text: ""
        width: parent.width
        Layout.fillWidth: true
        height: 20
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        minimumPixelSize: 24
        minimumPointSize: 36
        color: "#999999"
    }

    ScrollView {
        width: parent.width
        height: 100
        anchors.top: inputTitle.bottom
        TextArea {
            id: inputTextField
            color: "white"
            selectByMouse: true
            text: inputComponent.text
            wrapMode: "WrapAtWordBoundaryOrAnywhere"
            background: Rectangle {
                color: "#2D2F33"
                radius: 2
            }
            onTextChanged: {
                inputComponent.setTextBySlient(text)
            }
            onFocusChanged: {
                if(focus){
                    preText = text
                }else{
                    inputComponent.setText(preText,text)
                }
            }
        }
    }

    Component.onCompleted: {
        onComponentLoaded(inputComponent)
    }
}
