import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import ColumnLayoutComponent 1.0
Item {
    width: parent.width
    height: parent.height
    z:-1

    ColumnLayoutComponent{
        id:singleComponent

        onBindComponent: {
            loader.source = singleComponent.childQml(0)
        }
    }

    Loader{
        anchors.fill: parent
        id:loader
        source: ""
        onLoaded: {
            console.log("===Loader:onLoaded:Cur:"+singleComponent+" Source:" + source)
        }
        function onComponentLoaded(c){
            singleComponent.attachComponent(0,c)
            console.log("===Loader:onComponentLoaded:Cur:"+c+" Height:" + loader.height)
        }
    }

    Component.onCompleted: {
        onComponentLoaded(singleComponent)
    }
}
