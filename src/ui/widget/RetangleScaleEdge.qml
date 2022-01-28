import QtQuick 2.12
import QtQuick.Controls 2.12
import "../utils/Utils.js"  as Utils

Item {
    id:scalePointArea
    property string pointType: ""
    property real scaleX: 1
    property real scaleY: 1
    property real roateRangle: 0
    property real targetWidth:100
    property real targetHeight: 100

    transform: [
        Scale {
            xScale: (pointType === "top" || pointType === "bottom")?1:1/scaleX
            yScale: (pointType === "top" || pointType === "bottom")?1/scaleY:1
        }
    ]

    signal scaleXYChanged(var scaleX,var scaleY);
    signal scaleXYChangedOnRelease(var scaleX,var scaleY,var startScaleX,var startScaleY)


    function getCursorShape(angle){
        var targetAngle = -angle;
        if(pointType === "top" || pointType === "bottom"){
            targetAngle += 90
        }
        while(targetAngle < 0){
            targetAngle+=360;
        }
        var index = Math.round(targetAngle%180 / 45.0)
        var cursorShapes = [Qt.SizeHorCursor,Qt.SizeBDiagCursor,Qt.SizeVerCursor,Qt.SizeFDiagCursor,Qt.SizeHorCursor]
        console.log("getCursorShape: pointType:"+pointType+"  angle:"+targetAngle+" ->"+index)
        return cursorShapes[index]
    }


    onRoateRangleChanged: {
        mouseArea.cursorShape = getCursorShape(roateRangle)
    }

    MouseArea {
        id:mouseArea
        anchors.fill: parent
        hoverEnabled: true
        drag.target: parent
        drag.axis: Drag.XAndYAxis
        cursorShape: getCursorShape(roateRangle)
        preventStealing:true

        property var startPoint
        property real startScaleX
        property real startScaleY
        property real startWidth
        property real startHeight

        onPressed: {
            startPoint = mapToGlobal(mouseX, mouseY)
            startScaleX = scaleX
            startScaleY = scaleY
            startHeight = targetHeight * scaleY
            startWidth = targetWidth * scaleX

//            console.log(" startHeight:"+startHeight+" startWidth:"+startWidth+" startScaleX:"+startScaleX + " startScaleY:"+startScaleY + " roateRangle:"+Math.sign(Math.cos(roateRangle))+ " pointType:"+pointType)

        }
        onPositionChanged: {
            if (mouse.buttons === Qt.LeftButton) {
                var scale = calcScaleXY(mouse)

                scaleXYChanged(scale.x, scale.y)
            }
        }

        function calcScaleXY(mouse){
            var curPoint = mapToGlobal(mouseX, mouseY)
            var distinct = Math.sqrt((curPoint.x - startPoint.x)*(curPoint.x - startPoint.x) + (curPoint.y - startPoint.y)*(curPoint.y - startPoint.y))
            var curHeight = startHeight
            var curWidth = startWidth

            var angle = Utils.calcAngle2(startPoint,curPoint)
            if(pointType === "top"){

                var diffAngle = 90 - roateRangle - angle
                diffAngle = (diffAngle + 360)%360

//                console.log("pointType1111111:"+pointType + " angle:"+angle+" diffAngle:"+diffAngle)

                curHeight = startHeight +  (diffAngle < 90 || diffAngle > 270?1:-1)*distinct * 2
            }else if(pointType === "bottom"){

                diffAngle = 270 - roateRangle - angle
                diffAngle = (diffAngle + 360)%360

//                console.log("pointType2222222:"+pointType + " angle:"+angle+" diffAngle:"+diffAngle)

                curHeight = startHeight + (diffAngle < 90 || diffAngle > 270?1:-1)* distinct * 2
            }else if(pointType === "left"){

                diffAngle = 180 - roateRangle - angle
                diffAngle = (diffAngle + 360)%360

//                console.log("pointType333333333:"+pointType + " angle:"+angle+" diffAngle:"+diffAngle)

                curWidth = startWidth + (diffAngle < 90 || diffAngle > 270?1:-1)* distinct * 2
            }else {
                diffAngle = - roateRangle - angle
                diffAngle = (diffAngle + 360)%360
//                console.log("pointType4444444444:"+pointType + " angle:"+angle+" diffAngle:"+diffAngle)

                curWidth = startWidth + (diffAngle < 90 || diffAngle > 270?1:-1)* distinct * 2
            }
            var curScaleX = startScaleX * curWidth/ startWidth
            var curScaleY = startScaleY * curHeight / startHeight

            return {x:curScaleX,y:curScaleY}
        }

        onReleased: {
            var scale = calcScaleXY(mouse)
            console.log("onReleased pointType:"+pointType + " scale.x:"+scale.x+" scale.y:"+scale.y)

            scaleXYChangedOnRelease(scale.x, scale.y, startScaleX,startScaleY)
        }
    }
}
