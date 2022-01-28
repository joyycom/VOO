import QtQuick 2.0
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQml 2.12

Item{
    id:colorSelectRoot
    property alias title: colorTitle.text
    property color selectedColor : Qt.rgba(1,1,1,1)
    signal selectedColorValueChanged(var beginColor,var endColor);
    property bool transparentLayoutVisible: true


    Text{
        id:colorTitle
        width:60
        height: 25
        verticalAlignment: Qt.AlignVCenter
        horizontalAlignment: Qt.AlignLeft
        text:"颜色"
        color:"#FFFFFF"
        anchors.top: parent.top

    }

    onSelectedColorChanged: {
       console.log("onSelectedColorChanged:"+selectedColor+"  selectedColorRectangle:"+selectedColorRectangle.color)
    }

    Rectangle {
        width:80
        height: 25
        color:"#2C2E32"
        radius: 2
        anchors.top: parent.top
        anchors.left: colorTitle.right

        Rectangle{
            id:selectedColorRectangle
            width: 60
            height: 20
            x:4
            y:2.5
            color:Qt.rgba(selectedColor.r,selectedColor.g,selectedColor.b,selectedColor.a)
        }



        ColorDialog {
            id: colorDialog
            title: qsTr("Choose Color")
            showAlphaChannel: true
            color: selectedColor
            onAccepted: {
                var preColor = Qt.rgba(selectedColor.r,selectedColor.g,selectedColor.b,selectedColor.a)
                selectedColor = Qt.rgba(color.r,color.g,color.b,color.a)
                console.log("colorSelector change by ColorDialog")

                selectedColorValueChanged(preColor,selectedColor)
            }
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                colorDialog.open();
            }
        }

        Image{
            source:"qrc:/image/assets/dropdown.png"
            anchors.right: parent.right
            width:16
            height: 16
            y:4
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 2

        }

    }
}
