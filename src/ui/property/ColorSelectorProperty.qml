import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import "../widget"
import ColorSelectorComponent 1.0

Item {
    property int paramIndex
    property int value
    height: 40
    width: parent.width

    ColorSelectorComponent{
        id:colorSelectorComponent

        onBindComponent: {
            colorSelector.title = colorSelectorComponent.labelText()
            colorSelector.selectedColor = colorSelectorComponent.selectColor()
        }

        onColorUpdated: {
            console.log("onColorUpdated:"+color + " colorSelector.selectedColor:"+colorSelector.selectedColor)
            colorSelector.selectedColor = color
        }
    }

    ColorSelector {
        id:colorSelector
        height:40
        width: parent.width
        Layout.fillWidth: true
        onSelectedColorChanged: {
            var curColor = colorSelectorComponent.selectColor()
            colorSelectorComponent.updateColorSlient(selectedColor)
            var c = Qt.rgba(curColor.r,curColor.g,curColor.b,curColor.a)
            console.log("Background Color:"+curColor + " alpha:"+curColor.a + " c:"+c)
        }
        onSelectedColorValueChanged: {
            colorSelectorComponent.updateColorByUser(beginColor,endColor)
        }
    }


    Component.onCompleted: {
        onComponentLoaded(colorSelectorComponent)
    }
}
