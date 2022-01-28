import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import FramelessWindow 1.0
import "../widget"

FramelessWindow {

    id: window
    modality: Qt.ApplicationModal
    width: 540
    height: 360
    resizable: false
    onlyAllowListMovable: true
    allowTouchWidget: [titleRect]
    onClosing: {
        destroy()
    }

    function setLog(logs){
        console.log("1111:"+logs)

        for(var i in logs){
            var log = logs[i];
            var color = "#AAAAAA"
            if(log.startsWith("Warning:")){
                color = "#F0F028";
            }else if(log.startsWith("Fatal")){
                color = "#FF2828";
            }

            msgList.append({c:color,log:log});
            console.log("1111:"+log)
        }
    }

    Rectangle {
        color: "#15171C"
        radius: 4
        border.color: "#000000"
        border.width: 2
        anchors.fill: parent
        TitleBar {
            id: titleRect
            width: parent.width - 4
            height: 34
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 2
            anchors.leftMargin: 2
            color: "#000000"

            onCloseClicked: {
                window.close()
            }
        }
        ListView {
            id: contentColumn
            width: parent.width
            height: 300
            anchors.top: titleRect.bottom
            model: ListModel {id: msgList}
            spacing: 2
            delegate:Rectangle{
                color:index%2 === 0?"#00000000":"#80000000"
                width: parent.width
                height: 30

                TextEdit {
                    id: contentLabel
                    text: log
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 12
                    selectByMouse: true
                    color: c
                    enabled: false
                    wrapMode: "WrapAtWordBoundaryOrAnywhere"
                }

            }
        }
        Item {
            id: buttons
            width: parent.width
            height:  26
            anchors.right: parent.right
            anchors.top: contentColumn.bottom
            anchors.rightMargin: 20
            anchors.topMargin: 25
            anchors.bottomMargin: 20
            clip: true

            Button {
                id: button1
                width: 70
                height: 26
                anchors.right: parent.left
                anchors.rightMargin: 10
                text: "OK"
                onClicked: {
                    close()
                }
            }
        }
    }
}
