import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import SubEffectListComponent 1.0
import "../widget"

Item {
    height:parent.height
    width: parent.width
    property real preY: 0
    property var itemHeights: Array()



    SubEffectListComponent{
        id: effectComponent
        onBindComponent:{


        }
        onOfParamChanged:{

        }

        onEffectListChanged: {
            effectList.model = effectComponent.effectModel
        }

        onKeyFrameStateChanged: {

        }

        onContentYChanged: {
            rootScrollView.contentY = contentY
        }
    }


    Text {
        id: effectListEmptyText
        width: parent.width
        height: 40
        anchors.centerIn: parent
        text: qsTr("Effect List is empty!")
        font.pointSize: 16
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: "#999999"
        visible: effectList.height == 0
    }


    Flickable{
        anchors.fill: parent
        contentHeight: effectList.height
        contentWidth: width
        interactive:true
        id:rootScrollView

        onContentYChanged: {
            effectComponent.setContentY(contentY)
        }

        ListView {
            id: effectList
            width: parent.width
            height: contentHeight
            implicitHeight: contentHeight
            orientation: ListView.Vertical
            model:effectComponent.effectModel
            delegate: effectDelegate
            onHeightChanged: {
                console.log("EffectList Height:"+effectList.height)
            }
        }
    }


    Component {
        id: effectDelegate

        Rectangle{
            id: wrapper
            z:mouseArea.pressed?2:0
            height:column.height
            width: parent.width
            color:"#15171C"

            onHeightChanged: {
                itemHeights[index] = height
            }

            Column {
                id: column
                width: parent.width
                height: implicitHeight

                Rectangle{
                    width: parent.width
                    height:index==0?2:1
                    color: "#000000"
                    anchors.bottomMargin: 5
                }
                Item {
                    width: parent.width - 30
                    x:15
                    height: 50

                    Item{
                        width:30
                        height:30
                        id:dropdownContiner
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        Image{
                            width:6
                            height:10
                            anchors.centerIn: parent
                            source:"qrc:/image/assets/icon_dropdown_off.png"
                            visible: qml !== ""
                            rotation:isSelected?90:0
                            sourceSize.width: 10
                            sourceSize.height: 14
                        }
                    }


                    Image{
                        id:effectThumb
                        width:28
                        height:28
                        source:icon
                        sourceSize.width: 28
                        sourceSize.height: 28
                        anchors.left:dropdownContiner.right
                        anchors.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        fillMode: Image.PreserveAspectCrop
                        onStatusChanged: {
                            if(status === Image.Error){
                                effectThumb.source = "qrc:/image/assets/icon_default_effect.png"
                            }
                            console.log("onStatusChanged:"+status)
                        }
                        onSourceChanged: {
                            console.log("effectThumb source:"+source)
                        }
                    }

                    Rectangle{
                        id:thumbShadow
                        color:"#00000000"
                        radius: 2
                        border.color:"#15171C"
                        border.width: 4
                    }

                    Image{
                        id:effectThumbCorner
                        width:12
                        height:12
                        source:effectType=="Filter"?"qrc:/image/assets/timeline_filter_tiny_icon.png":"qrc:/image/assets/timeline_effect_tiny_icon.png"
                        sourceSize.width: 12
                        sourceSize.height: 12
                        anchors.right:effectThumb.right
                        anchors.rightMargin: 2
                        anchors.bottomMargin: 2
                        anchors.bottom:effectThumb.bottom
                        fillMode: Image.PreserveAspectCrop
                    }

                    Item{
                        id:deleteContainer
                        width:30
                        height:30
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        anchors.verticalCenter: parent.verticalCenter


                        Image{
                            id:effectDelete
                            width:12
                            height:12
                            anchors.centerIn: parent
                            fillMode: Image.PreserveAspectFit
                            source:"qrc:/image/assets/trash.png"
                        }

                        MouseArea{
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: {
                                effectDelete.source = "qrc:/image/assets/trash_hovered.png";
                            }

                            onExited: {
                                effectDelete.source = "qrc:/image/assets/trash.png";
                            }

                            onClicked: {
                                effectComponent.removeSubEffect(index)
                                console.log("delete "+index)
                            }
                        }
                    }


                    Text {
                        id:effectName
                        text: name
                        height: parent.height
                        clip: true
                        color: "#FFFFFF"
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        anchors.left:effectThumb.right
                        anchors.right: deleteContainer.left
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        wrapMode: Text.WordWrap
                    }

                    MouseArea{
                        id:mouseArea
                        anchors.left: parent.left
                        anchors.right: deleteContainer.left
                        height: parent.height
                        pressAndHoldInterval: 100
                        preventStealing: true
                        cursorShape: Qt.ArrowCursor
                        onClicked: {
                        }

                        onPressed: {
                            preY = wrapper.y
                        }

                        onReleased: {
                            var targetIndex = calcTargetIndex()
                            wrapper.y = preY

                            if( targetIndex !== index){
                                effectComponent.moveSubEffect(index,targetIndex)
                            }else{
                                effectComponent.setSelected(index)
                            }
                        }

                        onPressAndHold: {
                        }

                        onPositionChanged: {
                            wrapper.y = wrapper.y + mouse.y
                        }

                        function calcTargetIndex(){
                            var pos = wrapper.y + wrapper.height/2
                            var curHeight = 0;
                            for(var i=0;i<effectList.count;++i){
                                curHeight += itemHeights[i];
                                if(pos<=curHeight){
                                    return i;
                                }
                            }
                            return effectList.count-1
                        }

                    }
                }

                LazyPropertyLayout{
                    id:componentLoader
                    width: parent.width - 30
                    x:15
                    visible: isSelected
                    height: qmlHeight
                    source: qml
                    isShowing: isSelected


                    onComponentLoaded: {
                        console.log("attachComponent Index:"+index+" Component:"+component)
                        effectComponent.attachComponent(index,component)
                    }
                }

                Rectangle{
                    width: parent.width
                    height:effectList.count == index+1?2:1
                    anchors.topMargin: 5
                    color: "#000000"
                }
            }
        }
    }

    Component.onCompleted: {
        onComponentLoaded(effectComponent)
    }
}
