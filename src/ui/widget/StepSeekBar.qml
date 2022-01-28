import QtQuick 2.12
import QtQuick.Controls 2.12

//音量调节Slider
Item {
    property var points: []

    property alias slider: control
    signal sliderValueChanged(var beginValue,var endValue);
    property double beginTouchValue : 0
    property double stepValue: 0

    function stepValue(){
        return control.transformValue(control.value)
    }

    function setStepValue(v){
        var index = 0;
        while(index <= points.length - 1 && points[index]<v){
            index++
        }
        index--;

        var vvv = (index + (v - points[index])/(points[index+1]-points[index])) * (control.to-control.from) / (points.length - 1)
        control.value = vvv
        console.log(" Index:"+index + " VVV:"+v + "  -> "+vvv)
    }

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

        function transformValue(v){
            var percent = (points.length-1) * v/(to-from)
            var index = parseInt(percent, 10)
            if(percent === index){
                return points[index]
            }else{
                return points[index] + (points[index +1]-points[index])*(percent - index)
            }
        }


        onPressedChanged: {
            if(pressed){
                beginTouchValue = value
            }else{
                parent.sliderValueChanged(transformValue(beginTouchValue),transformValue(value))
            }
        }

        onValueChanged:{
            stepValue = transformValue(value)
        }

        background: Rectangle {
            anchors.fill: parent
            color:"#00000000"

            Rectangle {
                x: 0
                y: (parent.height - height) / 2
                height: 2
                width: parent.width
                color: "#3B3E47"

                Rectangle {
                    x:0
                    width:control.visualPosition * parent.width - slideHandler.width / 2
                    height: parent.height
                    color: "white"
                }
            }

            Repeater{
                model: points
                Rectangle{
                    width: 2
                    height: 10
                    x:index * control.width/(points.length-1)
                    y:(parent.height - height)/2
                    color: "white"

                    onXChanged: {
                        console.log("Repeater:"+x+" Index:"+index)
                    }
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
