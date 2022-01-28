import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import ColumnLayoutComponent 1.0
Item {
    property var source: ""
    property var isShowing: false
    signal componentLoaded(var component)

    width: parent.width
    height: parent.height


    Loader{
        anchors.fill: parent
        id:loader

        function onComponentLoaded(c){
            componentLoaded(c)
        }
    }

    onIsShowingChanged: {
        if(isShowing && loader.source !== source){
            loader.source = source;
        }
    }

}
