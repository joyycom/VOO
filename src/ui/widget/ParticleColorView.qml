import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

Item {
    id:root
    signal gradientListDataChanged(var list);

    Rectangle {
        height: 10
        width: parent.width
        id: gradientRect
        visible: true
        gradient: Gradient {
            id:gradient
            orientation:Gradient.Horizontal


        }
    }

    ListModel{
        id:gradientList


        onDataChanged: {
            updateGradientRect()
        }

    }
    MouseArea{
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: {

            var targetIndex = -1
            for (var idx = 0; idx < gradientList.count;idx++){
                var pos = gradientList.get(idx).position * width;
                console.log("onClicked:"+pos + " -> "+mouseX)

                if(Math.abs(mouseX - pos) < 10){
                    targetIndex = idx;
                    break;
                }else if(mouseX < pos){
                    targetIndex = - idx
                    break;
                }else if(mouseX > pos){
                    targetIndex = -1 - idx
                }
            }

            console.log("onDoubleClicked:"+targetIndex + " "+mouse.button)
            if(mouse.button === Qt.RightButton){
                if(targetIndex >= 0){
                    gradientList.remove(targetIndex)
                    updateGradientRect()
                    notifyGradientListDataChanged();
                }
            }else if(mouse.button === Qt.LeftButton){
                if(targetIndex < 0){
                    targetIndex = -targetIndex
                    var prePoint = targetIndex>=1?gradientList.get(targetIndex-1):gradientList.get(targetIndex)
                    var nextPoint = targetIndex<gradientList.count?gradientList.get(targetIndex):gradientList.get(targetIndex-1)

                    var percent = nextPoint === prePoint? 1:(mouseX/width - prePoint.position)/(nextPoint.position - prePoint.position)

                    var preColor = hexToRgb(prePoint.color)
                    var nextColor = hexToRgb(nextPoint.color)

                    var red = (preColor.r + percent * (nextColor.r - preColor.r))/255
                    var green = (preColor.g + percent * (nextColor.g - preColor.g))/255
                    var blue = (preColor.b + percent * (nextColor.b - preColor.b))/255
                    var color = Qt.rgba(red,green,blue,1)

                    gradientList.insert(targetIndex,{ position: mouseX/width,color: color.toString()})

                    notifyGradientListDataChanged();
                }
                colorDialog.curEditIndex = targetIndex
                colorDialog.color = gradientList.get(targetIndex).color
                colorDialog.open()
            }

        }
    }

    function notifyGradientListDataChanged(){

        gradientListDataChanged(getGradientListData());
    }

    function getGradientListData(){
        var list = []
        for(var i=0;i<gradientList.count;++i){
            list.push({color:gradientList.get(i).color,position:gradientList.get(i).position})
        }

        return list;
    }

    function setGradientListData(list){
        gradientList.clear()
        for(var i=0;i<list.length;++i){
            gradientList.append({color:list[i]["color"],position:list[i]["position"]})
        }
        updateGradientRect()

    }

    function updateGradientRect(){

        gradientRect.update()

        var newStops = [];
        for (var idx = 0; idx < gradientList.count;idx++){
            var s1 = stopComponent.createObject(gradientRect, gradientList.get(idx));
            newStops.push(s1);
        }
        gradient.stops = newStops;
        gradientRect.update()
    }

    function revert(){
        for(var i=0;i<gradientList.count;++i){
            var pos = gradientList.get(i).position
            gradientList.setProperty(i,"position", 1 - pos)
            gradientList.move(i,0,1)
        }
        updateGradientRect()
        notifyGradientListDataChanged();
    }

    function random(){
        for(var i=0;i<gradientList.count;++i){
            var ccc = Qt.rgba(Math.random(),Math.random(),Math.random(),1).toString();
            gradientList.setProperty(i,"color",ccc)
        }
        updateGradientRect()
        notifyGradientListDataChanged()
    }


    function dumpGradientList(){
        for(var i=0;i<gradientList.count;++i){
            console.log("Index:"+i+" position:"+gradientList.get(i).position+" color:"+gradientList.get(i).color)
        }
    }

    function sortGradientList(){
        for(var i=0;i<gradientList.count;++i){
            for(var j=0;j<gradientList.count - i - 1;++j){
                if(gradientList.get(j).position > gradientList.get(j+1).position){
                    gradientList.move(j,j+1,1)
                }
            }
        }
    }

    Repeater{
        id:repeater
        model: gradientList
        width:parent.width
        delegate: Canvas {
            id: canvasId
            width: 20;
            height: 12
            property var hPadding: width/4
            property var vPadding: height/4

            x:parent.width * position - width/2
            anchors.top:gradientRect.bottom
            anchors.topMargin: 6
            contextType: "2d"

            onPaint: {
                context.lineWidth = 0
                context.strokeStyle = color
                context.fillStyle = color
                context.beginPath();
                context.moveTo(hPadding, height - vPadding)
                context.lineTo(width/2, vPadding);
                context.lineTo(width - hPadding, height - vPadding);
                context.closePath();
                context.fill()
                context.stroke();
            }

            onXChanged: {
                position = (canvasId.x + canvasId.width/2)/parent.width
            }

            MouseArea{
                anchors.fill: parent
                drag.target: canvasId
                drag.axis: Drag.XAxis
                drag.minimumX: -10
                drag.maximumX: root.width - 10
                preventStealing:true

                onReleased: {
                    sortGradientList();
//                    dumpGradientList();

                    notifyGradientListDataChanged();
                }
            }
        }

    }



    ColorDialog {
        id: colorDialog
        property var curEditIndex: -1

        title: qsTr("Choose Color")
        color: "#FF0000"
        onAccepted: {
            var pos = gradientList.get(curEditIndex).position
            var ccc = currentColor.toString();
            console.log("colorDialog onAccept:"+curEditIndex + " Color:"+currentColor + " currentHue:"+currentHue)

            gradientList.setProperty(curEditIndex,"color", ccc)
            updateGradientRect()
            notifyGradientListDataChanged();
        }
    }

    function rgbToHex(color) {
      return "#" + ((color.a << 24) + (color.r << 16) + (color.g << 8) + color.b).toString(16).slice(1);
    }




    function hexToRgb(hex) {
      var result = /^#?([a-fA-F\d]{2})([a-fA-F\d]{2})([a-fA-F\d]{2})$/i.exec(hex);
      return result ? {
        r: parseInt(result[1], 16),
        g: parseInt(result[2], 16),
        b: parseInt(result[3], 16)
      } : null;
    }


    Component {
        id:stopComponent
        GradientStop {}
    }


    Component.onCompleted: {


    }

}
