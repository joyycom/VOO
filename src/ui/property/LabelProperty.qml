import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import LabelComponent 1.0
import "../widget"

Item {
    width:parent.width
    height: 30


    LabelComponent {
        id: labelComponent
    }

    Text {
        id: videoEditPropertyTitle
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: editPropertyPanelHMargin
        anchors.rightMargin: editPropertyPanelHMargin
        anchors.leftMargin: editPropertyPanelHMargin
        color: "#FFFFFF"
        text: labelComponent.labelText
    }

    Component.onCompleted: {
        onComponentLoaded(labelComponent)

    }
}
