import QtQuick 2.12
import QtQuick.Controls 2.12

Item {
    id:scalePointArea
    property string pointType: ""
    property real scaleX: 1
    property real scaleY: 1
    property real targetWidth:100
    property real targetHeight: 100
    property real roateRangle: 0

    transform: [
        Scale {
            xScale: 1/scaleX
            yScale: 1/scaleY
        }
    ]


    signal scaleXYChanged(var scaleX,var scaleY);
    signal scaleXYChangedOnRelease(var scaleX,var scaleY,var startScaleX,var startScaleY)

    function getCursorShape(angle){
        var targetAngle = -angle;

        if(pointType === "left_top" || pointType === "right_bottom"){
            targetAngle += 135
        }else{
            targetAngle += 45

        }
        while(targetAngle < 0){
            targetAngle+=360;
        }
        var index = Math.round(targetAngle%180 / 45)
        var cursorShapes = [Qt.SizeHorCursor,Qt.SizeBDiagCursor,Qt.SizeVerCursor,Qt.SizeFDiagCursor,Qt.SizeHorCursor]
        console.log("getCursorShape: pointType:"+pointType+"  angle:"+angle+" ->"+index)
        return cursorShapes[index]
    }

    onRoateRangleChanged: {
        mouseArea.cursorShape = getCursorShape(roateRangle)
    }

    width: 12
    height: 12

    Rectangle{
        color: "#ffffff"
        width: 10
        height: 10
        radius: 5
        anchors.centerIn: parent
    }

    MouseArea {
        id:mouseArea
        anchors.fill: parent
        hoverEnabled: true
        drag.target: parent
        drag.axis: Drag.XAndYAxis
        cursorShape: getCursorShape(roateRangle)
        property real startx
        property real starty
        property real startScaleX
        property real startScaleY
        property real startWidth
        property real startHeight

        onPressed: {
            startx = mapToGlobal(mouseX, mouseY).x
            starty = mapToGlobal(mouseX, mouseY).y
            startScaleX = scaleX
            startScaleY = scaleY
            startHeight = targetHeight * scaleY
            startWidth = targetWidth * scaleX
        }
        onPositionChanged: {
            if (mouse.buttons === Qt.LeftButton) {
                var scale = calcScaleXY(mouse)

                scaleXYChanged(scale.x, scale.y)
            }
        }

        function calcScaleXY(mouse){
            var curPoint = mapToGlobal(mouseX, mouseY)
            var curHeight = startHeight
            var curWidth = startWidth

            if(pointType.includes("top")){
                curHeight = startHeight + (starty - curPoint.y ) * 2
            }else{
                curHeight = startHeight - (starty - curPoint.y ) * 2
            }

            if(pointType.includes("left")){
                curWidth = startWidth + (startx - curPoint.x ) * 2
            }else{
                curWidth = startWidth + (curPoint.x - startx) * 2
            }
            var curScaleX = startScaleX * curWidth/ startWidth
            var curScaleY = startScaleY * curHeight / startHeight

            if (mouse.modifiers & Qt.ShiftModifier) {
                curScaleX = Math.min(curScaleX,curScaleY)
                curScaleY = Math.min(curScaleX,curScaleY)
            }
            console.log(" curHeight:"+curHeight+" curWidth:"+curWidth+" ScaleX:"+curScaleX + " ScaleY:"+curScaleY + " ScaleX:"+scaleX)

            return {x:curScaleX,y:curScaleY}
        }

        onReleased: {
            var scale = calcScaleXY(mouse)
            scaleXYChangedOnRelease(scale.x, scale.y, startScaleX,startScaleY)
        }
    }
}
