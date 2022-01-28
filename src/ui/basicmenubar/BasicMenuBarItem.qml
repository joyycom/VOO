import QtQuick 2.12
import QtQuick.Templates 2.12 as T
import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12

T.MenuBarItem {
    id: control

    property color textColor: control.highlighted ? "#ffffff" : "#BFBFBF"
    property color backgroundColor: "transparent"

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding,
                             implicitIndicatorHeight + topPadding + bottomPadding)

    //spacing: 6
    padding: 0
    leftPadding: 12
    rightPadding: 12

    //icon.width: 24
    //icon.height: 24
    //icon.color: control.palette.buttonText

    contentItem: Text {
        text: control.text
        font: control.font
        //opacity: enabled ? 1.0 : 0.3
        color: control.textColor
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        renderType: Text.NativeRendering
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitHeight: 40
        height: control.height-1
        color: control.backgroundColor
    }
}
