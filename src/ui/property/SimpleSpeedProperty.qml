import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import SimpleSpeedComponent 1.0
import "../widget"

Item {
    width: parent.width
    height: 40

    SimpleSpeedComponent{
        id:simpleSpeedComponent
        onSimpleSpeedChanged: {
            seekbar.setStepValue(speed)
            console.log("onSimpleSpeedChanged:"+simpleSpeedComponent.getSpeed() + " Speed:"+speed)
        }
    }

    RowLayout {
        id:sliderLayout
        spacing: 10
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: editPropertyPanelHMargin
        anchors.rightMargin: editPropertyPanelHMargin
        anchors.leftMargin: editPropertyPanelHMargin

        Text{
            id:title
            text:qsTr("Times")
            Layout.minimumWidth: 60
            color:"#FFFFFF"
        }

        StepSeekBar{
            id: seekbar
            Layout.fillWidth: true
            Layout.minimumWidth: 100
            points: [0.1,1,2,5,10,100]

            onSliderValueChanged:{
                simpleSpeedComponent.setSpeedByUser(beginValue,endValue)
            }
            onStepValueChanged: {
                simpleSpeedComponent.setSpeed(stepValue)
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
                text:"x"+seekbar.stepValue.toFixed(1)
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color:"#BFBFBF"
            }
        }
    }

    Component.onCompleted: {
        onComponentLoaded(simpleSpeedComponent)
    }

}
