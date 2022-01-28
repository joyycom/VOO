import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import ColumnLayoutComponent 1.0
Item {
    width: parent.width
    height:layout.height

    ColumnLayoutComponent{
        id:columnLayoutComponent

        onBindComponent: {
            repeater.model = columnLayoutComponent.count()
        }
    }


    Column {
        id: layout
        width: parent.width
        height: implicitHeight

        spacing: 10

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
                    console.log("===Loader:onLoaded2:Cur:"+source+" Height:" + loader.height+ " Y:"+y)
                }

                function onComponentLoaded(c){
                    columnLayoutComponent.attachComponent(index,c)

                }
            }
        }


    }

    Component.onCompleted: {
        onComponentLoaded(columnLayoutComponent)
    }
}
