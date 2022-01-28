import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import SaveOfEffectComponent 1.0
import "../widget"

Item {
    width:parent.width
    height: 30


    SaveOfEffectComponent {
        id: saveComponent

        onBindComponent: {
            button.text = saveComponent.labelText()

        }
    }

    Button {
        id: button
        anchors.right: parent.right

        onClicked: {
            saveComponent.saveEffect()
        }
    }

    Component.onCompleted: {
        onComponentLoaded(saveComponent)

    }
}
