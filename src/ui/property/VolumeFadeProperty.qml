import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import "../widget"
import VolumeFadeComponent 1.0

Item {
    property alias title: title.text
    height:40
    width: parent.width


    VolumeFadeComponent{
        id:volumeComponent
        onBindComponent: {
            title.text = volumeComponent.paramTitle()
            seekbar.slider.to = volumeComponent.getMaxDuring()
            seekbar.slider.value = volumeComponent.getDuring()
        }
        onFadeDuringChanged: {
            seekbar.slider.value = volumeComponent.getDuring()
        }

        onMaxDurationChanged: {
            seekbar.slider.to = volumeComponent.getMaxDuring()
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
            text: ""
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
                from:volumeComponent.getMinDuring()
                to:volumeComponent.getMaxDuring()
                stepSize: 0.1
                value: 0

                onValueChanged: {
                    volumeComponent.setDuring(seekbar.slider.value)
                }
            }
            onSliderValueChanged: {
                volumeComponent.setDuringByUser(beginValue,endValue)
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
                text:seekbar.slider.value.toFixed(1) + "s"
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
