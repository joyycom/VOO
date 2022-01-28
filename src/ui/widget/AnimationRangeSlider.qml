import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3

Item {
    property var title

    width: parent.width
    height: parent.height
    RowLayout {
        spacing: 1
        width: parent.width
        height: parent.height
        Text {
            text: title
            color: "#FFFFFF"
        }

        Rectangle {
            color: "#aaaaaa"
            width:48
            height:20
            Text {
                text: qsTr(control.first.value + "s")
                color: "#FFFFFF"
            }
        }

        RangeSlider {
            id: control
            height: 20
            Layout.fillWidth: true
            from: 0
            to: 100
            first.value: 0
            second.value: 100
            stepSize: 1
            background: Rectangle {
                    x: control.leftPadding
                    y: control.topPadding + control.availableHeight / 2 - height / 2
                    implicitWidth: 200
                    implicitHeight: 4
                    width: control.availableWidth
                    height: implicitHeight
                    radius: 2
                    color: "#FFFFFF"

                    Rectangle {
                        x: control.first.visualPosition * parent.width
                        width: control.second.visualPosition * parent.width - x
                        height: parent.height
                        color: "#bdbebf"
                        radius: 2
                    }
                }

            first.handle: Item {
                x: control.leftPadding + control.first.visualPosition * (control.availableWidth - width)
                y: control.topPadding + control.availableHeight / 2 - height / 2
                width: 8
                height: 8
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.width
                    radius: width / 2
                    color: "white"
                }
            }
            second.handle: Item {
                x: control.leftPadding + control.second.visualPosition * (control.availableWidth - width)
                y: control.topPadding + control.availableHeight / 2 - height / 2
                width: 8
                height: 8
                Rectangle {
                    anchors.centerIn: parent
                    width: parent.width
                    height: parent.width
                    radius: width / 2
                    color: "white"
                }
            }
        }

        Rectangle {
            color: "#aaaaaa"
            width:48
            height:20
            Text {
                text: qsTr(control.second.value + "s")
                color: "#FFFFFF"
            }
        }
    }
}
