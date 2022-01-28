import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import CurvedSpeedComponent 1.0
import "../widget"

Item {
    height:parent.height
    width: parent.width
    CurvedSpeedComponent {
        id: curvedSpeedComponent

        onBindComponent: {
            console.log("onBindComponent " + curvedSpeedComponent.listModel())
        }

        onBzizerPointsChanged: {
            if (points.length === 0) {
                curvedSpeedViewContainer.visible = false
            } else {
                curvedSpeedViewContainer.visible = true
                curvedSpeedView.setPoints(points)
            }
        }

        onResourceProgressChanged: {
            curvedSpeedView.updateProgress(progress)
        }
    }


    Flickable{
        anchors.fill: parent
        contentHeight: layoutRoot.height
        contentWidth: width
        interactive:true
        anchors.top: parent.top
        anchors.topMargin: 10
        id:rootScrollView

        Column {
            id: layoutRoot
            height: implicitHeight
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: editPropertyPanelHMargin
            anchors.rightMargin: editPropertyPanelHMargin
            spacing: 12

            GridView {
                id: curvedSpeedGridView
                height: 180
                width: parent.width
                cellWidth: 70
                cellHeight: 80
                clip: true
                model: curvedSpeedComponent.listModel()
                delegate: gridDelegate

                onContentHeightChanged: {
                    height = contentHeight + 10

                    console.log("ContentHeight:" + height+" layoutRoot:"+layoutRoot.height)
                }
            }
            Rectangle {
                width: parent.width
                height: 200
                id: curvedSpeedViewContainer
                visible: false
                color: "#212121"

                CurvedSpeedView {
                    id: curvedSpeedView
                    width: parent.width
                    height: 160
                    verticalMargin:20
                    anchors.top: parent.top

                    onDataChanged: {
                        console.log("DataChanged:" + curvedSpeedView.getPoints())
                        curvedSpeedComponent.updatePoints(curvedSpeedView.getPoints())
                    }
                    onSelectedChanged: {
                        btnRemove.enabled = index > 0
                                && index < curvedSpeedView.points.length - 1

                        btnAdd.enabled = index < 0

                        console.log("onSelectedChanged:" + index)
                    }

                    onWidthChanged: {
                        if(curvedSpeedView.points.length > 0){
                            curvedSpeedView.setPoints(curvedSpeedComponent.bzizerPoints())
                        }
                    }

                    onIsPressedChanged: {
                        rootScrollView.interactive = !isPressed
                    }

                    onProgressChangedByUser: {
                        console.log("onProgressChangedByUser:" + progress)
                        curvedSpeedComponent.updateProgress(progress)
                    }
                }
                Item{
                    width: parent.width
                    height: 30
                    anchors.bottom: parent.bottom


                    Image {
                        id: btnReset
                        width: 20
                        height: 20
                        anchors.left: parent.left
                        anchors.leftMargin: 15
                        anchors.verticalCenter: parent.verticalCenter
                        sourceSize.width: 32
                        sourceSize.height: 32
                        source: "qrc:/image/assets/curved_speed_reset.png"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                curvedSpeedComponent.resetPoints()
                            }
                        }
                    }



                    Text {
                        id: btnResetBtn
                        width: 80
                        height: 20
                        color:"#FFFFFF"
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: btnReset.right
                        leftPadding: 4

                        text: qsTr("Reset")
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                curvedSpeedComponent.resetPoints()
                            }
                        }
                    }


                    Image {
                        id: btnAdd
                        anchors.right: parent.right
                        anchors.rightMargin: 15
                        anchors.verticalCenter: parent.verticalCenter
                        width: 20
                        height: 20
                        sourceSize.width: 32
                        sourceSize.height: 32
                        source: "qrc:/image/assets/curved_speed_add_point.png"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                curvedSpeedView.addCurPoint()
                            }
                        }
                    }

                    Image {
                        id: btnRemove
                        width: 20
                        height: 20
                        anchors.right: btnAdd.left
                        anchors.rightMargin: 15
                        sourceSize.width: 32
                        anchors.verticalCenter: parent.verticalCenter
                        sourceSize.height: 32
                        source: "qrc:/image/assets/curved_speed_remove_point.png"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                curvedSpeedView.deleteCurPoint()
                            }
                        }
                    }

                }

            }
            Item{
                width:parent.width
                Layout.fillHeight: true
            }
        }

        Component {
            id: gridDelegate
            Item {
                id: wrapper
                height: curvedSpeedGridView.cellHeight
                width: curvedSpeedGridView.cellWidth

                Rectangle {
                    id:imageContainer
                    width: 60
                    height: 60
                    anchors.top: parent.top
                    anchors.margins: 2
                    anchors.horizontalCenter: parent.horizontalCenter
                    border.color: isSelected ? "#02E4D6" : "#00000000"
                    border.width: 1
                    color: "#00000000"

                    Image {
                        id: itemImage
                        width: parent.width - 4
                        height: parent.height - 4
                        mipmap: true
                        anchors.centerIn: parent
                        source: thumb
                    }


                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: {
                            curvedSpeedComponent.selectCurvedSpeed(index)
                        }
                    }

                }


                Text {
                    id: title
                    text: name
                    height: 20
                    width: parent.width
                    anchors.top: imageContainer.bottom
                    color: "#BFBFBF"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }



            }
        }
        Component.onCompleted: {
            onComponentLoaded(curvedSpeedComponent)
        }
    }
}
