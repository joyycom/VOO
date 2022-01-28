import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import "../widget"
import SimpleComponent 1.0

Item {
    property var component: null
    property var config: component.extraInputConfig()

    clip: true
    height: 120
    width:parent.width
    visible: component.hasInputConfig()

    SimpleComponent {
        id: inputComponent
    }
    Connections {
        target: component
        onInputDeviceChanged: {
            visible = component.hasInputConfig()
            config = component.extraInputConfig()
        }
    }
    Text {
        id: inputTitle
        text: "Input Config"
        width: parent.width
        Layout.fillWidth: true
        height: 25
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
            text: config
            wrapMode: "WrapAtWordBoundaryOrAnywhere"
            background: Rectangle {
                color: "#2D2F33"
                radius: 2
            }
            onTextChanged: {
                component.inputConfigEditingFinished(text)
            }
        }
    }

    Component.onCompleted: {
        console.log("EditInputProperty onCompleted:" + component)
        if (component === null) {
            component = inputComponent
            console.log("EditInputProperty attachComponent1:" + component
                        + " transformComponent: " + inputComponent)
            onComponentLoaded(inputComponent)

            console.log("EditInputProperty attachComponent:" + component)
        } else {
            console.log("EditInputProperty Component.onCompleted:" + component)
        }
    }
}
