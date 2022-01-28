import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12
import ParticleColorComponent 1.0
import "../widget"

Item {
    height:60
    width: parent.width

    ParticleColorComponent {
        id: particleColorComponent

        onBindComponent: {
            title.text = getLabel()

        }

        onPartileColorDataChanged: {
            var colors = getPartileColors()
            console.log("onBindComponent " + colors)
            particleColorView.setGradientListData(colors)
        }
    }

    Text {
        id: title
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: editPropertyPanelHMargin
        anchors.rightMargin: editPropertyPanelHMargin
        anchors.leftMargin: editPropertyPanelHMargin
        color: "#FFFFFF"
        text: particleColorComponent.labelText
    }

    RowLayout {
        height: 40
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: editPropertyPanelHMargin
        anchors.leftMargin: editPropertyPanelHMargin
        anchors.topMargin: 4
        anchors.top:title.bottom
        spacing: 10

        ParticleColorView{
            focus: true
            id:particleColorView
            Layout.fillWidth: true
            height: 30

            onGradientListDataChanged: {
                particleColorComponent.setPartileColors(list)
                console.log("onGradientListDataChanged:"+list)
            }
        }


        Item{
            width:120
            height: parent.height

            BasicComboBox {
                id:colorComboBox
                editable: false
                height:20
                width:parent.width
                anchors.top: parent.top
                backgroundColor: "#2D2F33"
                itemNormalColor: "#2D2F33"
                borderColor: "transparent"
                itemHighlightColor: "#2D2F33"
                itemHeight:26
                showCount: 15
                model: colorTitleList
                onCurrentIndexChanged: {
                    var colors = []
                    if(currentIndex == 1){
                        colors = [
                                    {position:0.0,color:"#FF0000"},
                                    {position:0.25,color:"#FFFF00"},
                                    {position:0.5,color:"#00FF00"},
                                    {position:0.75,color:"#00FFFF"},
                                    {position:1.0,color:"#0000FF"}
                                ]
                        particleColorView.setGradientListData(colors)
                        particleColorComponent.setPartileColors(colors)

                    } else if(currentIndex === 2){
                        colors = [
                                    {position:0.0,color:"#FFFFFF"},
                                    {position:1.0,color:"#000000"}
                                ]
                        particleColorView.setGradientListData(colors)
                        particleColorComponent.setPartileColors(colors)
                    }else if(currentIndex === 3){
                        colors = [
                                    {position:0.0,color:"#FFFFFF"},
                                    {position:0.25,color:"#00FFFF"},
                                    {position:1,color:"#0000FF"}
                                ]
                        particleColorView.setGradientListData(colors)
                        particleColorComponent.setPartileColors(colors)
                    } else if(currentIndex == 4){
                        colors = [
                                    {position:0,color:"#FFFF00"},
                                    {position:0.25,color:"#FF0000"},

                                    {position:0.5,color:"#fd7ebe"},
                                    {position:0.75,color:"#ff7eff"},
                                    {position:1.0,color:"#9f00fd"}
                                ]
                        particleColorView.setGradientListData(colors)
                        particleColorComponent.setPartileColors(colors)

                    }
                }
            }

            Button{
                id:random
                text:"Random"
                height: 20
                width: parent.width/2
                anchors.left: parent.left
                anchors.top: colorComboBox.bottom
                anchors.topMargin: 2
                onClicked: {
                    particleColorView.random()
                }
            }

            Button{
                id:revert
                text:"Revert"
                height: 20
                width: parent.width/2
                anchors.left: random.right
                anchors.top: colorComboBox.bottom
                anchors.topMargin: 2
                anchors.leftMargin: 2
                onClicked: {
                    particleColorView.revert()
                }
            }
        }

    }


    ListModel{
        id:colorTitleList
        ListElement{
            text:"Custom"
        }
        ListElement{
            text:"Colorful"
        }
        ListElement{
            text:"BlackAndWhite"
        }
        ListElement{
            text:"Blue"
        }
        ListElement{
            text:"Colorful2"
        }
    }


    Component.onCompleted: {
        onComponentLoaded(particleColorComponent)
    }

}
