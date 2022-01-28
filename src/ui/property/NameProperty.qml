import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import NameComponent 1.0
import "../widget"

Item {
    width:parent.width
    height: 30

    NameComponent {
        id: nameComponent
        onBindComponent: {
            nameLabel.text = nameComponent.labelText()
            nameText.text = nameComponent.name()
        }
    }

    onHeightChanged: {
        console.log("PropertyColumnLayout Height:"+height + " implicitHeight:"+implicitHeight)
    }

    Text {
        id: nameLabel
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: editPropertyPanelHMargin
        anchors.leftMargin: editPropertyPanelHMargin
        color: "#FFFFFF"
        text: nameComponent.labelText()
    }

    Text {
        id: nameText
        anchors.verticalCenter: nameLabel.verticalCenter
        anchors.left: nameLabel.right
        anchors.leftMargin: editPropertyPanelHMargin
        color: "#FFFFFF"
        text: nameComponent.labelText()
    }

    Component.onCompleted: {
        onComponentLoaded(nameComponent)
    }
}
