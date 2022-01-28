import QtQuick 2.12
import QtQuick.Templates 2.12 as T
import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12


T.MenuBar {
    id: control

    property color backgroundColor: "white"
    property color borderColor: "black"

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    font{
//        pixelSize: 16
    }
    delegate: BasicMenuBarItem { }

    contentItem: Row {
        spacing: control.spacing
        Repeater {
            model: control.contentModel
        }
    }

    //背景在MenuBarItem之下，我把MenuBarItem的background高度去了1px
    background: Rectangle {
        implicitHeight: 40
        color: control.backgroundColor

//        Rectangle {
//            color: control.borderColor
//            width: parent.width
//            height: 1
//            anchors.bottom: parent.bottom
//        }
    }
}
