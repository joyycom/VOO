import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import AnimationListComponent 1.0
import "../widget"

Item {
    property bool isDownloading: false
    property int downloadIndex: -1
    height:parent.height
    width: parent.width

    AnimationListComponent {
        id: animationListComponent
        onBindComponent: {
            var during = animationListComponent.durationForSection()
            gridView.model = animationListComponent.listModel
            duringSlider.from = animationListComponent.minDuration()
            duringSlider.to = animationListComponent.maxDuration()
            duringSlider.value = during
        }
        onOnDownloading: {
            isDownloading = downloading
            downloadIndex = index
        }
        onListModelChanged: {
            gridView.model = animationListComponent.listModel
        }

        onMaxDurationChanged: {
            duringSlider.to = animationListComponent.maxDuration()
        }
    }

    GridView {
        id: gridView
        anchors.margins: 4
        height: parent.height - anchors.topMargin - 30
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: editPropertyPanelHMargin
        anchors.rightMargin: editPropertyPanelHMargin
        //model: animationListComponent.listModel
        cellWidth: 80
        cellHeight: 100
        clip: true
        delegate: gridDelegate
    }

    AnimationSlider {
        id: duringSlider
        anchors.top: gridView.bottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 15
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: editPropertyPanelHMargin
        anchors.rightMargin: editPropertyPanelHMargin
        visible: true
        title: qsTr("Animation speed")
        valueSuffix: "s"
        stepSize: 0.1
        onValueChanged: {
            if(duringSlider.slider.pressed){
                console.log("onvaluechange, value:" + value)
                animationListComponent.onAnimationDurationChange(value)
            }
        }
        onSliderValueChanged: {
            animationListComponent.onAnimationDurationChangeByUser(beginValue,
                                                                   endValue)
        }
        Component.onCompleted: {
            duringSlider.value = animationListComponent.durationForSection()
        }
    }

    Connections {
        target: animationListComponent
        onDurationForSectionChanged: {
            duringSlider.value = duration
            console.log("onDurationForSectionChanged, Duration: " + duration)
        }
    }

    Component {
        id: gridDelegate
        ColumnLayout {
            id: wrapper
            width: 60
            height: 80
            Rectangle {
                width: 60
                height: 60
                border.color: isSelected ? "#02E4D6" : "#00000000"
                border.width: 1
                color: "#00000000"

                BasicImage{
                    id: img
                    anchors.centerIn: parent
                    height: parent.height - 4
                    width: parent.width - 4
                    imgSrc: icon == "" ? "qrc:/image/assets/animation_default.png" : icon
                    animatedImgSrc: dynamicIcon

                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        animationListComponent.onAnimationTypeChange(index)
                    }
                    onEntered: {
                        console.log("dynamicIcon Src: ",
                                    img.animatedImgSrc)
                        if (img.animatedImgSrc != "") {
                            img.isEntered = true
                        }
                    }
                    onExited: {
                        img.isEntered = false
                    }
                }

                Image {
                    id: downloadIcon
                    width: 16
                    height: 16
                    anchors.right: parent.right
                    anchors.rightMargin: 4
                    anchors.top: parent.top
                    anchors.topMargin: 4
                    visible: !isDownload && index != 0
                             && !(isDownloading && downloadIndex === index)
                    source: "qrc:/image/assets/res_download.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            if (!isDownloading) {
                                animationListComponent.clickDownload(index)
                            }
                        }
                    }
                }
                Image {
                    id: downloading_icon
                    anchors.fill: downloadIcon
                    visible: isDownloading && downloadIndex === index
                             && index != 0
                    source: "qrc:/image/assets/res_downloading.png"
                }
                RotationAnimation {
                    id: rotationAnimation
                    target: downloading_icon
                    running: isDownloading && downloadIndex === index
                             && index != 0
                    from: 0
                    to: 360
                    direction: RotationAnimation.Clockwise
                    duration: 500
                    loops: Animation.Infinite
                }
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                id: title
                text: name
                color: "#BFBFBF"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        console.log("chengj:  onMouseClicked:" + curSelectedIndex)
                        animationListComponent.onAnimationTypeChange(index)
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        onComponentLoaded(animationListComponent)
    }
}
