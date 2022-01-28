import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import VideoComponent 1.0
import StackLayoutComponent 1.0
import "../widget"

Item {
    width: parent.width
    height: parent.height
    z:0

    StackLayoutComponent{
        id:stackLayoutComponent

        onBindComponent: {
            var preIndex = stackLayoutComponent.getCurIndex()
            repeater.model = stackLayoutComponent.count()
            stackTabbar.currentIndex = preIndex
            stackLayoutRepeater.model = stackLayoutComponent.count()
            stackLayout.currentIndex = Qt.binding(function(){return stackTabbar.currentIndex})
        }
    }

    TabBar {
        id: stackTabbar
        width: parent.width
        height: 30
        background: Rectangle {
            color: "#15171C"
        }
        z:100
        Repeater {
            id:repeater
            model: 0
            TabButton {
                text: modelData
                height: 30
                contentItem: Text {
                    text: stackLayoutComponent.subTitle(index)
                    font: parent.font
                    color: stackTabbar.currentIndex == index ? "#FFFFFF" : "#747474"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                background: Rectangle {
                    color: "#00000000"
                }
            }
        }

        onCurrentIndexChanged: {
            stackLayoutComponent.setCurIndex(currentIndex)
        }
    }

    StackLayout {
        id: stackLayout
        width: parent.width
        height: parent.height - stackTabbar.height
        z:-1
        anchors.top: stackTabbar.bottom
        anchors.bottom: parent.bottom



        Repeater {
            id:stackLayoutRepeater
            width: parent.width
            height: parent.height
            z:-1

            model: 0

            LazyPropertyLayout{
                width: parent.width
                height: parent.height
                z:-1

                isShowing: index === stackTabbar.currentIndex
                source: stackLayoutComponent.childQml(index)

                onComponentLoaded: {
                    stackLayoutComponent.attachComponent(index, component)
                    console.log("===Loader:onComponentLoaded:Cur:"+component)
                }
            }

        }
    }

    Component.onCompleted: {
        onComponentLoaded(stackLayoutComponent)
    }
}
