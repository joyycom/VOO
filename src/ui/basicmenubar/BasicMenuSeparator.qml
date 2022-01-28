import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12
import QtQuick.Templates 2.12 as T

T.MenuSeparator {
    id: control

    property color contentColor: "black"
    property color backgroundColor: "#15171C"

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             implicitContentHeight + topPadding + bottomPadding)

    padding: 0
    topPadding: 4
    bottomPadding: 4
    contentItem: Rectangle {
        implicitWidth: 120
        implicitHeight: 1
        color: contentColor
    }
    background: Rectangle{
        color: backgroundColor
    }
}
