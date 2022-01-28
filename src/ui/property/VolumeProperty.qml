import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import "../widget"
import VolumeComponent 1.0

Item {
    property alias title: title.text
    property int paramIndex
    property int value
    height:40
    width: parent.width


    VolumeComponent{
        id:volumeComponent
        onBindComponent: {
            seekbar.slider.value = getVolume() * 100
        }
        onOfParamChanged: {
            console.log("onOfParamChanged :"+key)
            if(key == "Clip:Volume"){
                 seekbar.slider.value = getVolume() * 100
            }
        }
    }

    RowLayout {
        id:sliderLayout
        spacing: 10
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: editPropertyPanelHMargin
        anchors.rightMargin: editPropertyPanelHMargin

        Text{
            id:title
            text: qsTr("Volume")
            Layout.minimumWidth: 60
            height: 20
            color:"#FFFFFF"
        }

        SeekBar{
            id: seekbar
            Layout.fillWidth: true
            Layout.minimumWidth: 100
            slider{
                orientation:Qt.Horizontal
                from:0
                to:200
                stepSize: 1
                value: volumeComponent.getVolume() * 100

                onValueChanged: {
                    volumeComponent.setVolume(seekbar.slider.value/100.0)
                }
            }
            onSliderValueChanged: {
                volumeComponent.setVolumeByUser(beginValue/100.0,endValue/100.0)
            }
            height: 20
        }

        Rectangle{
            color: "#2D2F33"
            radius:2
            width:48
            height:20

            Text{
                anchors.fill: parent
                text:seekbar.slider.value.toFixed(0) + "%"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color:"#BFBFBF"
            }
        }
    }
    Component.onCompleted: {
        onComponentLoaded(volumeComponent)

    }
}
