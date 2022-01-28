import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import "../widget"
import TransitionComponent 1.0

Item {
    property alias title: title.text
    height: 40
    width: parent.width


    TransitionComponent {
        id: transitionComponent
        onBindComponent: {
            console.log("seekbar.slider.value:" + getDuring(
                            ) + " Max:" + getMaxDuring())
            seekbar.slider.value = getDuring() * 100
            seekbar.slider.from = getMinDuring() * 100
            seekbar.slider.to = getMaxDuring() * 100
        }
        onOfParamChanged: {
            console.log("onOfParamChanged :" + key)
            if (getDuring() * 100 != seekbar.slider.value) {
                seekbar.slider.value = getDuring() * 100
            }
        }
        onDuringChanged: {
            if (getDuring() * 100 != seekbar.slider.value) {
                seekbar.slider.value = getDuring() * 100
            }
        }
    }

    RowLayout {
        id: sliderLayout
        spacing: 15
        height: 30
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: editPropertyPanelHMargin
        anchors.rightMargin: editPropertyPanelHMargin

        Text {
            id: title
            text: qsTr("Transition time")
            Layout.minimumWidth: 60
            height: 20
            color: "#FFFFFF"
        }

        SeekBar {
            id: seekbar
            Layout.fillWidth: true
            Layout.minimumWidth: 100
            slider {
                orientation: Qt.Horizontal
                from: transitionComponent.getMinDuring() * 100
                to: transitionComponent.getMaxDuring() * 100
                stepSize: 1
                value: transitionComponent.getDuring() * 100

                onValueChanged: {
                    if (seekbar.slider.pressed) {
                        transitionComponent.setDuring(
                                    seekbar.slider.value / 100.0)
                    }
                }
            }
            onSliderValueChanged: {
                transitionComponent.setDuring(beginValue / 100.0,
                                              endValue / 100.0)
            }
            height: 20
        }

        Rectangle {
            color: "#2D2F33"
            radius: 2
            width: 48
            height: 20

            Text {
                anchors.fill: parent
                text: (seekbar.slider.value / 100.0).toFixed(2) + "s"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                color: "#BFBFBF"
            }
        }
    }
    Component.onCompleted: {
        onComponentLoaded(transitionComponent)
    }
}
