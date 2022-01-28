import QtQuick 2.0

Item {
    property var preHeight: -1
    property var preWidth: -1
    property var points: []
    property var bgColor: "#000000"
    property var pointOffset: 0.2

    property var fgColor: Qt.rgba(0.035, 0.18, 0.318, 1);
    property var verticalMargin: 10
    signal dataChanged(var datas);

    function setFloatPoints(datas){
        var dd = []
        for(var i=0;i<datas.length;++i){
            dd.push(Qt.point( i * 1.0 * width / datas.length, (1 - datas[i]) * height))
        }
        points = dd
        pointOffset = 0.5 * width/datas.length
        canvas.requestPaint();

        console.log(" onPointsChanged:"+points)
    }

    onWidthChanged:{
        if(preHeight > 0 && preWidth > 0){
            for(var i=0;i<points.length;++i){
                points[i].x = points[i].x * width / preWidth
                points[i].y = points[i].y * height / preHeight
            }
        }

        preHeight = height
        preWidth = width
    }


    function getPoints(maxValue){
        var dd = []
        for(var i=0;i<points.length;++i){
            dd.push(Math.max(Math.min((1.0 - points[i].y * 1.0 / height) * maxValue,maxValue),0))
        }

        return dd
    }

    function setPoints(datas, maxValue){
        var dd = []
        for(var i=0;i<datas.length;++i){
            dd.push(Qt.point( i * 1.0 * width / datas.length, (1 - datas[i] / maxValue) * height))
        }
        points = dd
        pointOffset = 0.5 * width/datas.length
        canvas.requestPaint();

        console.log(" onPointsChanged:"+points)
    }


    function revert(){
        for(var i=0;i<points.length/2;++i){
            var temp = points[i].y
            points[i].y = points[points.length - i - 1].y
            points[points.length - i - 1].y = temp
        }
        canvas.requestPaint()
        dataChanged(getPoints(255))
    }

    function random(){
        for(var i=0;i<points.length;++i){
            points[i].y = Math.random() * height
        }
        canvas.requestPaint()
        dataChanged(getPoints(255))
    }

    function smoothData(){
        for(var i=1;i<points.length-1;++i){
            var pre = points[i - 1].y
            var next = points[i + 1].y
            points[i].y = pre * 0.3333 + points[i].y * 0.3333 + next * 0.333
        }
        canvas.requestPaint()
        dataChanged(getPoints(255))
    }

    Canvas {
        id:canvas
        anchors.fill: parent


        onPaint:{
            var ctx = canvas.getContext('2d');
            ctx.fillStyle = bgColor
            ctx.fillRect(0,0,width,height)
            drawLine(ctx)

        }


        function drawLine(ctx){
            ctx.strokeStyle = fgColor;
            ctx.lineWidth = 1;

            ctx.beginPath();

            ctx.moveTo(0,height);
            for(var i=0;i<points.length;++i){
                var point = points[i];
                ctx.lineTo(point.x,point.y);
            }
            ctx.lineTo(width,height);
            ctx.closePath();
            ctx.fillStyle = fgColor;
            ctx.fill();
        }

        MouseArea{
            id:mouseArea
            anchors.fill: parent
            preventStealing:true
            property var lastMouseX: -1

            onPressed: {
                if(lastMouseX < 0){
                    lastMouseX = mouseX
                }

                updatePoints()
            }


            onReleased: {
                console.log("onReleased: mouseY:"+mouseY)

                updatePoints()
                lastMouseX = -1;
                dataChanged(getPoints(255))

            }

            onCanceled: {
                console.log("onCanceled: mouseY:"+mouseY)

                updatePoints()
                lastMouseX = -1;
                dataChanged(getPoints(255))
            }


            function updatePoints(){

                var minMouseX = Math.min(lastMouseX,mouseX) - pointOffset
                var maxMouseX = Math.max(lastMouseX,mouseX) + pointOffset
                console.log("minMouseX :"+minMouseX+","+maxMouseX + " mouseY:"+mouseY)

                for(var i=0;i<points.length;++i){
                    if(points[i].x > minMouseX && points[i].x < maxMouseX){
                        points[i].y = mouseY
                    }
                }
                lastMouseX = mouseX
                canvas.requestPaint();

            }


            onPositionChanged: {
                updatePoints()

            }


        }
    }
}
