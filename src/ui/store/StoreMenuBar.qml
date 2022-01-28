import QtQuick 2.12
import QtQuick.Controls 2.12

ToolButton {
    property url imageSource
    property url hoveredImageSource
    property string txt
    property string cate
    width: 40
    height: 60
    property bool isSelected: false

    Rectangle {
        width: parent.width
        height: parent.height
        color: "#15171C"
    }
    Rectangle {
        id: bg
        radius: 2
        y: 4
        width: parent.width
        height: parent.height - 8
        color: "#19ffffff"
        visible: isSelected
    }
    Image {
        width: 20
        height: 20
        source: (hovered||isSelected)?hoveredImageSource:imageSource
        smooth: true
        mipmap:true
        fillMode: Image.PreserveAspectFit
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            topMargin: 10
        }
    }
    Text {
        text: txt
        color: (hovered||isSelected)?"#ffffff":"#BFBFBF"
        anchors {
            horizontalCenter: parent.horizontalCenter
            bottom: parent.bottom
            bottomMargin: 5
        }
    }

    MouseArea{
        width:parent.width
        height:parent.height
        onClicked: {
            datasource.selectTopCategory(cate)
        }
    }

    function updateSelectState() {
        isSelected = datasource.selectTopCategoryID() === cate
    }
}
