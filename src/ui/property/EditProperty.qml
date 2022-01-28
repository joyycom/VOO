import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import "../widget/"

Rectangle {
    color: "#15171C"
    id: editPropertyPanelRoot
    property int editPropertyPanelHMargin: 12

    Component.onCompleted: {
        propertyEditController.onLoadCompleted()
    }

    Connections {
        target: propertyEditController
        onEditPanelDataChanged: {
            rootLoader.source = propertyEditController.rootQml()
        }

        onToastContentChanged: {
            editToastMgr.show(msg,during)
        }
    }


    Loader {
        id:rootLoader
        anchors.fill: parent
        source: "EmptyProperty.qml"
        onLoaded: {
            console.log("editPropertyPanelContent Loader.onLoaded:" + source)
        }

        function onComponentLoaded(c){
            propertyEditController.attachComponent(0,c)
            console.log("===Loader:onComponentLoaded:Cur:"+c+" Height:" + height)
        }
    }


    ToastManager{
        id:editToastMgr
        anchors.fill: editPropertyPanelRoot
    }
}
