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
        padding: 0



        onPressedChanged: {
            if(pressed){
                beginTouchValue = value
            }else{
                parent.sliderValueChanged(beginTouchValue,value)
            }
            parent.sliderPressedChanged(pressed)

        }

        onValueChanged: {
            console.log("Slider:sliderValueChanged:"+value + " pressed:"+pressed)
        }



        background: Rectangle {
            anchors.fill: parent

            Rectangle{
                width: 2
                height: 10
                x:(parent.width - width)/2
                y:(parent.height - height)/2
                color: "white"
            }

            Rectangle {
                x: 0
                y: (parent.height - height) / 2
                height: 2
                width: parent.width
                color: "#3B3E47"

                Rectangle {
                    x:(control.visualPosition > 0.5)?parent.width/2:control.visualPosition * parent.width
                    width:(control.visualPosition > 0.5)? (control.visualPosition - 0.5) * parent.width - slideHandler.width / 2 : (0.5-control.visualPosition) * parent.width - slideHandler.width / 2
                    height: parent.height
                    color: "white"
                }
            }
        }
        //这个拖动按钮可以找个图片替换
        handle: Item {
            id: slideHandler
            x: control.visualPosition * (parent.width) - width / 2
            y: (control.height - height) / 2
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

}
