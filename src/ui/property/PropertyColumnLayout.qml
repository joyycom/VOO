import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import ColumnLayoutComponent 1.0
Item {
    width: parent.width
    height: parent.height


    ColumnLayoutComponent{
        id:columnLayoutComponent

        onBindComponent: {
            repeater.model = columnLayoutComponent.count()
        }
    }

    ScrollView{
        z:-1
        id:rootScroll
        anchors.fill: parent
        contentHeight: layout.height
        contentWidth: parent.width


        Column {
            id: layout
            width: parent.width-16
            height: implicitHeight + 10
            x:8
            spacing: 0


            Repeater{
                id:repeater
                width: parent.width
                model:0
                Loader{
                    id:loader
                    width: parent.width
                    source:columnLayoutComponent.childQml(index)
                    onLoaded: {
                        layout.forceLayout()
                        console.log("===Loader:onLoaded2:Cur:"+source+" Height:" + loader.height+ " Y:"+y + " "+layout.height)
                    }

                    function onComponentLoaded(c){
                        columnLayoutComponent.attachComponent(index,c)

                    }
                }
            }


        }
    }
    Component.onCompleted: {
        onComponentLoaded(columnLayoutComponent)
    }
}
