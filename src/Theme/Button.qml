import QtQuick 2.12
import QtGraphicalEffects 1.12
import QtQuick.Templates 2.12 as T

T.Button {
    id: control
    property int style: 0
    property color textColor: style === 0 ? textColor0 : style === 1 ? textColor1 : textColor2
    property color textColor0: "#BFBFBF"
    property color textColor1: control.down ? "#CCCCCC" : "#FFFFFF"
    property color textColor2: control.down ? "#737373" : "#BFBFBF"
    property color backgroundColor: style === 0 ? backgroundColor0 : style
                                                  === 1 ? backgroundColor1 : backgroundColor2
    property color backgroundColor0: control.down ? "#0FFFFFFF" : (control.hovered
                                                                   || control.highlighted) ? "#3FFFFFFF" : "#26FFFFFF"
    property color backgroundColor1: control.down ? "#324093" : (control.hovered
                                                                 || control.highlighted) ? "#6574FF" : "#5465FF"
    property color backgroundColor2: control.down ? "#171717" : (control.hovered
                                                                 || control.highlighted) ? "#3C3C3C" : "#262626"
    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)
    padding: 6
    horizontalPadding: padding + 2
    spacing: 6
    background: Rectangle {
        id: bg
        implicitWidth: parent.width
        implicitHeight: parent.height
        color: control.backgroundColor
        radius: 2
    }
    contentItem: Text {
        id: text
        width: parent.width
        height: parent.height
        text: control.text
        color: control.textColor
        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
}
