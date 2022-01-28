import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3

Item {
    id: sliderRoot
    property alias slider: control
    property var title
    property var valueSuffix: ""
    property double from: 0
    property double to: 100
    property double stepSize: 1
    property var value

    signal sliderValueChanged(var beginValue,var endValue);
    property double beginTouchValue : 0

    width: parent.width
    height: parent.height


    RowLayout {
        spacing: 15
        width:parent.width
        height: parent.height
        anchors.fill: parent
        Text {
            text:title
            Layout.minimumWidth: 60
            height: 20
            color: control.enabled?"white":"#3B3E47"
        }

        Slider {
            id: control
            from: sliderRoot.from
            to: sliderRoot.to
            stepSize: sliderRoot.stepSize
            value: sliderRoot.value
            enabled: sliderRoot.to >= sliderRoot.from
            Layout.fillWidth: true
            Layout.minimumWidth: 100

            onPressedChanged: {
                if(pressed){
                    beginTouchValue = value
                }else{
                    sliderRoot.sliderValueChanged(beginTouchValue,value)
                }

            }

            background: Rectangle {
                x: 0
                y: (parent.height - height) / 2
                height:2
                width: parent.width
                color: "#3B3E47"

                Rectangle {
                    width: control.visualPosition * parent.width - slideHandler.width / 2
                    height: parent.height
                    color: "white"
                }
            }
            //这个拖动按钮可以找个图片替换
            handle: Item {
                id: slideHandler
                x: control.visualPosition * (parent.width)- width / 2
                y: (control.height - height) / 2
                width: 14
                height: 14
                Rectangle {
                    id: rect_in
                    anchors.centerIn: parent
                    width: 10
                    height: 10
                    radius: width / 2
                    color: control.enabled?"white":"#3B3E47"
                }
            }
            onValueChanged: {
                sliderRoot.value = value
            }
        }

        Rectangle {
            color: "#2D2F33"
            radius:2
            width:48
            height:20

            Text {
                anchors.fill: parent
                text: control.value.toFixed(1) + valueSuffix
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: control.enabled?"#BFBFBF":"#3B3E47"
            }
        }
    }

    Component.onCompleted: {
    }
}
