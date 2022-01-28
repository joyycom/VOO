import QtQuick 2.0

Item {
    property var points: []
    property var bgColor: "#2C2C2C"
    property var fgColor: Qt.rgba(1,1,1, 1);
    property var axisColor: "#747474";
    property var verticalMargin: 10
    property var progress: -1
    property var pointSize:4
    property bool isPressed: false
    signal selectedChanged(int index);
    signal dataChanged();
    signal progressChangedByUser(var progress);

    function setPoints(datas){
        var dd = []
        for(var i=0;i<datas.length;++i){
            dd.push(transformPoint(datas[i]))
        }
        points = dd
        canvas.requestPaint();

        console.log(" onPointsChanged:"+points)
    }

    function updateProgress(p){
        if(Math.abs(progress - p) * width >= 1){
            progress = p
            canvas.requestPaint()
        }
    }

    function getPoints() {
        var datas = []
        var yContentHeight = height/2 - verticalMargin
        for(var i=0;i<points.length;++i){
            var y = points[i].y >= height/2 ? 1 - (points[i].y - height/2) / yContentHeight *0.9:(height/2- points[i].y) / yContentHeight * 9 + 1
            datas.push(Qt.point(points[i].x / width ,y))
        }
        return datas
    }

    function transformPoint(data){
        var yContentHeight = height/2 - verticalMargin
        var y = data.y >= 1 ? (10 - data.y) * yContentHeight /9 + verticalMargin:(1- data.y) * yContentHeight / 0.9 + height / 2
        var x = data.x * width
        return Qt.point(x ,y)
    }

    function deleteCurPoint(){
        var latelyIndex = canvas.findLatelyPointIndex(progress * width);
        if(latelyIndex > 0 && latelyIndex < points.length - 1){
            points.splice(latelyIndex,1)
            canvas.requestPaint();

            dataChanged();
            selectedChanged(-latelyIndex)
        }
    }

    function addCurPoint(){
        var latelyIndex = canvas.findLatelyPointIndex(progress * width);
        if(latelyIndex < 0){
            var x = progress*width
            var y = canvas.calBezierValueY(points[-latelyIndex - 1],points[-latelyIndex],x)
            points.splice(-latelyIndex,0,Qt.point(x,y))
            canvas.requestPaint();

            dataChanged();
            selectedChanged(-latelyIndex)
        }
    }

    Canvas {
        id:canvas
        anchors.fill: parent


        onPaint:{
            var ctx = canvas.getContext('2d');
            ctx.fillStyle = bgColor
            ctx.fillRect(0,0,width,height)

            drawAxis(ctx)
            //start point
            drawBezierCurves(ctx)

            drawProgressLine(ctx)
        }


        function drawAxis(ctx){
            ctx.strokeStyle = "#000000"
            ctx.lineWidth = 1;
            ctx.setLineDash([2,2])
            ctx.beginPath();
            ctx.moveTo(0, verticalMargin)
            ctx.lineTo(width,verticalMargin)
            ctx.stroke();

            ctx.beginPath();
            ctx.moveTo(0,canvas.height - verticalMargin)
            ctx.lineTo(width,canvas.height - verticalMargin)
            ctx.stroke();

            ctx.beginPath();
            ctx.lineWidth = 1;
            ctx.setLineDash([10000,0])
            ctx.moveTo(0,0.5 * canvas.height)
            ctx.lineTo(width,0.5 * canvas.height)
            ctx.stroke();


            ctx.font = "12px Arial"
            ctx.textAlign = left
            ctx.textBaseline = verticalMargin
            ctx.fillStyle = axisColor
            ctx.fillText("10x",15, verticalMargin - 5)

            ctx.textBaseline = canvas.height/2
            ctx.fillText("1x",15,height/2 - 5)

            ctx.textBaseline = canvas.height - verticalMargin
            ctx.fillText("0.1x",15,canvas.height - verticalMargin - 5)


        }

        function drawBezierCurves(ctx){
            if(points.length < 2){
                return;
            }

            ctx.strokeStyle = fgColor;
            ctx.lineWidth = 2;

            ctx.beginPath();
            ctx.setLineDash([10,0])

            for(var i=1;i<points.length;++i){
                var prePoint = points[i-1];
                ctx.moveTo(prePoint.x,prePoint.y);
                var curPoint = points[i];
                var controlX = prePoint.x+(-prePoint.x + curPoint.x)/2
                ctx.bezierCurveTo(controlX,prePoint.y,controlX,curPoint.y,curPoint.x,curPoint.y)
            }
            ctx.stroke();
            var curProgressX = progress * width
            for(var i=1;i<points.length-1;++i){
                ctx.fillStyle = bgColor
                ctx.fillRect(points[i].x - pointSize,points[i].y-pointSize,pointSize * 2,pointSize*2)

                ctx.moveTo(points[i].x+pointSize+1,points[i].y);

                if(i === mouseArea.selectIndex || Math.abs(points[i].x - curProgressX) < pointSize){
                    ctx.fillStyle = fgColor
                    ctx.fillRect(points[i].x - pointSize,points[i].y - pointSize, 2* pointSize, 2*pointSize);
                }else{
                    ctx.strokeRect(points[i].x - pointSize,points[i].y - pointSize, 2* pointSize, 2*pointSize);
                }
            }

            var triggleSize = pointSize
            ctx.beginPath();
            ctx.strokeStyle = fgColor
            ctx.moveTo(points[0].x,points[0].y - triggleSize)
            ctx.lineTo(points[0].x+triggleSize*1.732,points[0].y )
            ctx.lineTo(points[0].x,points[0].y + triggleSize)
            ctx.closePath();
            ctx.fillStyle = fgColor;
            ctx.fill();

            ctx.beginPath();
            ctx.strokeStyle = fgColor
            ctx.moveTo(width - triggleSize*1.732,points[points.length-1].y - triggleSize)
            ctx.lineTo(width,points[points.length-1].y )
            ctx.lineTo(width - triggleSize*1.732,points[points.length-1].y + triggleSize)
            ctx.closePath();
            ctx.fillStyle = fgColor;
            ctx.fill();

        }

        function drawProgressLine(ctx){

            ctx.strokeStyle = "#02E4D6";
            ctx.lineWidth = 1;
            ctx.beginPath();
            ctx.setLineDash([10000,0])
            ctx.moveTo(progress * width,0)
            ctx.lineTo(progress * width,height)
            ctx.closePath();
            ctx.stroke()
        }

        function findLatelyPointIndex(posX){
            var latelyIndex = -1;
            for(var i=0;i<points.length;++i){
                if(Math.abs(points[i].x-posX)<pointSize|| (latelyIndex >= 0 && Math.abs(points[i].x-posX) < Math.abs(points[latelyIndex].x-posX))){
                    latelyIndex = i;
                }else if(latelyIndex < 0 && i > 0 && points[i-1].x < posX && posX < points[i].x){
                    latelyIndex = -i;
                    break;
                }
            }
            return latelyIndex;
        }


        function sign(x) {
            x = +x ;// convert to a number
            if (x === 0 || isNaN(x))
                return x;
            return x > 0 ? 1 : -1;
        }

        function calcBizierValueXbyT(p0,p1,p2,p3,t){
            return p0.x*t*t*t + 3*p1.x*t*t*(1-t)+ 3*p2.x*t*(1-t)*(1-t)+p3.x*(1-t)*(1-t)*(1-t);
        }

        function calcBizierValueYbyT(p0,p1,p2,p3,t){
            return p0.y*t*t*t + 3*p1.y*t*t*(1-t)+ 3*p2.y*t*(1-t)*(1-t)+p3.y*(1-t)*(1-t)*(1-t);
        }

        function calBezierValueY(p0,p3,x){
            var controlX = p0.x+(-p0.x + p3.x)/2
            var p1 = Qt.point(controlX,p0.y)
            var p2 = Qt.point(controlX,p3.y)
            var left = 0
            var right = 1
            var leftX =calcBizierValueXbyT(p0,p1,p2,p3,left)
            var rightX = calcBizierValueXbyT(p0,p1,p2,p3,right)

            while(true){
                var center = (left + right)/2
                var centerX = calcBizierValueXbyT(p0,p1,p2,p3,center)

                if(Math.abs(centerX - x) < 0.1){
                    return calcBizierValueYbyT(p0,p1,p2,p3,center);
                }else if(sign(centerX - x) === sign(rightX - x)){
                    right = center
                    rightX = centerX
                }else if(sign(centerX - x) === sign(leftX - x)){
                    left = center
                    leftX = centerX
                }
            }

        }




        MouseArea{
            id:mouseArea
            anchors.fill: parent
            property var selectIndex: -1
            onPressed: {
                isPressed = true
                progress = mouseX / width

                var nestIndex = canvas.findLatelyPointIndex(mouseX);

                if(nestIndex >= 0 && Math.abs(points[nestIndex].y-mouseY)<pointSize){
                    selectIndex = nestIndex;
                }

                canvas.requestPaint();

                console.log("Mouse :"+selectIndex + " nestIndex:"+nestIndex )
            }

            onMouseYChanged: {

            }

            function updatePositionValue(){
                if (selectIndex >= 0) {
                    var pointX = mouseX
                    if(selectIndex == 0 || selectIndex == points.length - 1){
                        pointX = points[selectIndex].x;
                    }else if(pointX > points[selectIndex + 1].x - pointSize){
                        pointX = points[selectIndex + 1].x - pointSize
                    }else if(pointX < points[selectIndex - 1].x + pointSize){
                        pointX = points[selectIndex - 1].x + pointSize
                    }
                    var pointY = Math.max(Math.min(mouseY,height - verticalMargin),verticalMargin)

                    progress = pointX / width
                    points[selectIndex] = Qt.point(pointX,pointY)
                }else{
                    progress = mouseX / width
                }
                progress = Math.max(progress,0)
                progressChangedByUser(progress)

            }

            onPositionChanged: {
                updatePositionValue()
                canvas.requestPaint();
            }

            onReleased: {
                updatePositionValue()
                if(selectIndex >= 0){
                    dataChanged(selectIndex)
                }
                selectIndex = -1

                canvas.requestPaint();

                var latelyIndex = canvas.findLatelyPointIndex(progress * width);
                selectedChanged(latelyIndex)
                isPressed = false
            }

            onCanceled: {
                updatePositionValue()
                if(selectIndex >= 0){
                    dataChanged(selectIndex)
                }
                selectIndex = -1

                canvas.requestPaint();

                var latelyIndex = canvas.findLatelyPointIndex(progress * width);
                selectedChanged(latelyIndex)
                isPressed = false
            }

        }
    }
}
