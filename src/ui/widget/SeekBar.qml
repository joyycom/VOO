import QtQuick 2.12
import QtQuick.Controls 2.12

//音量调节Slider
Item {
    property alias slider: control
    signal sliderPressedChanged(var pressed)
    signal sliderValueChanged(var beginValue,var endValue);
    property double beginTouchValue : 0

    Slider {
        id: control
        height: parent.height
        width: parent.width
        value: 5
        from: 0
        to: 100
        stepSize: 1
        anchors.centerIn: parent
        leftPadding: 7
        rightPadding: 7

        onPressedChanged: {
            if(pressed){
                beginTouchValue = value
            }else{
                parent.sliderValueChanged(beginTouchValue,value)
            }
            parent.sliderPressedChanged(pressed)

        }

        background: Rectangle {
            x: control.horizontal ? control.leftPadding : (parent.width - width ) / 2
            y: control.horizontal ? (parent.height - height) / 2 : 0
            height: control.horizontal ? 2 : parent.height
            width: control.horizontal ? parent.width - control.leftPadding - control.rightPadding : 2
            color: "#3B3E47"

            Rectangle {
                width: control.horizontal ? control.visualPosition * (parent.width) + control.leftPadding - slideHandler.width / 2 : parent.width
                height: control.horizontal ? parent.height : control.visualPosition
                                             * parent.height - slideHandler.height / 2
                color: "white"
            }
        }
        //这个拖动按钮可以找个图片替换
        handle: Item {
            id: slideHandler
            x: control.horizontal ? control.visualPosition * (parent.width - control.leftPadding - control.rightPadding)
                                    - width / 2 + control.leftPadding : (control.width - width) / 2
            y: control.horizontal ? (control.height - height) / 2 : control.visualPosition
                                    * (parent.height) - height / 2
            width: 14
            height: 14
            Rectangle {
                id: rect_in
                anchors.centerIn: parent
                width: 10
                height: 10
                radius: width / 2
                color: "white"
            }
        }
    }

//        MouseArea{
//            anchors.fill: parent
//            //避免和handle冲突
//            acceptedButtons: Qt.NoButton
//            onWheel: {
//                if(wheel.angleDelta.y<0){
//                    control.decrease();
//                }else{
//                    control.increase();
//                }
//            }

//        }


}
