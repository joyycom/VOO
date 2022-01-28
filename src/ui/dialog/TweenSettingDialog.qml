import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtQml 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import FramelessWindow 1.0
import "../widget"

FramelessWindow {
    property var selectedKey: "none"
    property var ofParamKey: ""
    signal notifySelectedKeyChanged(var paramKey, var tween)
    modality: Qt.ApplicationModal
    width: 560
    height: 600
    resizable: false
    onlyAllowListMovable: true
    allowTouchWidget: [titleRect, dialogTitle]
    ListModel {
        id: tweenListModel
    }

    Rectangle {
        color: "#15171C"
        radius: 4
        border.color: "#000000"
        border.width: 2
        anchors.fill: parent
        TitleBar {
            id: titleRect
            width: parent.width - 4
            height: 42
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 2
            anchors.leftMargin: 2
            color: "#000000"
            onCloseClicked: {
                close()
            }
            Label {
                id: dialogTitle
                text: qsTr("Curve")
                font.pixelSize: 12
                color: "#BFBFBF"
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        GridView {
            id: gridView
            anchors.margins: 4
            height: parent.height - anchors.topMargin - 30
            anchors.top: titleRect.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 5
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            model: tweenListModel
            cellWidth: 90
            cellHeight: 90
            clip: true
            delegate: gridDelegate
        }

        Component {
            id: gridDelegate
            Item {
                id: wrapper
                width: 90
                height: 90
                Rectangle {
                    width: 80
                    height: 60
                    id: imgWrapper
                    anchors.horizontalCenter: parent.horizontalCenter
                    border.color: selectedKey === key ? "#02E4D6" : "#00000000"
                    border.width: 1
                    color: "#00000000"

                    Image {
                        id: img
                        anchors.centerIn: parent
                        height: parent.height - 2
                        width: parent.width - 2
                        source: icon
                    }
                }

                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: imgWrapper.bottom
                    anchors.topMargin: 2
                    width: parent.width
                    horizontalAlignment: Text.AlignHCenter
                    height: 10
                    elide: Text.ElideMiddle
                    id: title
                    text: name
                    color: "#BFBFBF"
                    font.pixelSize: 10
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectedKey = key
                        notifySelectedKeyChanged(ofParamKey, key)
                        close()
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        var tweenList = [{
                             "name": "Linear",
                             "icon": "qrc:/image/assets/curve/linear.png",
                             "key": "linear"
                         }, {
                             "name": "Bezier",
                             "icon": "qrc:/image/assets/curve/bezier.png",
                             "key": "bezier"
                         }, {
                             "name": "None",
                             "icon": "qrc:/image/assets/curve/none.png",
                             "key": "none"
                         }, {
                             "name": "Full",
                             "icon": "qrc:/image/assets/curve/full.png",
                             "key": "full"
                         }, {
                             "name": "NoneLinear",
                             "icon": "qrc:/image/assets/curve/noneLinear.png",
                             "key": "noneLinear"
                         }, {
                             "name": "LinearFul",
                             "icon": "qrc:/image/assets/curve/linearFul.png",
                             "key": "linearFul"
                         }, {
                             "name": "EaseInSine",
                             "icon": "qrc:/image/assets/curve/easeInSine.png",
                             "key": "sine.easeIn"
                         }, {
                             "name": "EaseOutSine",
                             "icon": "qrc:/image/assets/curve/easeOutSine.png",
                             "key": "sine.easeOut"
                         }, {
                             "name": "EaseInOutSine",
                             "icon": "qrc:/image/assets/curve/easeInOutSine.png",
                             "key": "sine.easeInOut"
                         }, {
                             "name": "EaseInQuad",
                             "icon": "qrc:/image/assets/curve/easeInQuad.png",
                             "key": "quad.easeIn"
                         }, {
                             "name": "EaseOutQuad",
                             "icon": "qrc:/image/assets/curve/easeOutQuad.png",
                             "key": "quad.easeOut"
                         }, {
                             "name": "EaseInOutQuad",
                             "icon": "qrc:/image/assets/curve/easeInOutQuad.png",
                             "key": "quad.easeInOut"
                         }, {
                             "name": "EaseInCubic",
                             "icon": "qrc:/image/assets/curve/easeInCubic.png",
                             "key": "cubic.easeIn"
                         }, {
                             "name": "EaseOutCubic",
                             "icon": "qrc:/image/assets/curve/easeOutCubic.png",
                             "key": "cubic.easeOut"
                         }, {
                             "name": "EaseInOutCubic",
                             "icon": "qrc:/image/assets/curve/easeInOutCubic.png",
                             "key": "cubic.easeInOut"
                         }, {
                             "name": "EaseInQuart",
                             "icon": "qrc:/image/assets/curve/easeInQuart.png",
                             "key": "quart.easeIn"
                         }, {
                             "name": "EaseOutQuart",
                             "icon": "qrc:/image/assets/curve/easeOutQuart.png",
                             "key": "quart.easeOut"
                         }, {
                             "name": "EaseInOutQuart",
                             "icon": "qrc:/image/assets/curve/easeInOutQuart.png",
                             "key": "quart.easeInOut"
                         }, {
                             "name": "EaseInQuint",
                             "icon": "qrc:/image/assets/curve/easeInQuint.png",
                             "key": "quint.easeIn"
                         }, {
                             "name": "EaseOutQuint",
                             "icon": "qrc:/image/assets/curve/easeOutQuint.png",
                             "key": "quint.easeOut"
                         }, {
                             "name": "EaseInOutQuint",
                             "icon": "qrc:/image/assets/curve/easeInOutQuint.png",
                             "key": "quint.easeInOut"
                         }, {
                             "name": "EaseInExpo",
                             "icon": "qrc:/image/assets/curve/easeInExpo.png",
                             "key": "expo.easeIn"
                         }, {
                             "name": "EaseOutExpo",
                             "icon": "qrc:/image/assets/curve/easeOutExpo.png",
                             "key": "expo.easeOut"
                         }, {
                             "name": "EaseInOutExpo",
                             "icon": "qrc:/image/assets/curve/easeInOutExpo.png",
                             "key": "expo.easeInOut"
                         }, {
                             "name": "EaseInCirc",
                             "icon": "qrc:/image/assets/curve/easeInCirc.png",
                             "key": "circ.easeIn"
                         }, {
                             "name": "EaseOutCirc",
                             "icon": "qrc:/image/assets/curve/easeOutCirc.png",
                             "key": "circ.easeOut"
                         }, {
                             "name": "EaseInOutCirc",
                             "icon": "qrc:/image/assets/curve/easeInOutCirc.png",
                             "key": "circ.easeInOut"
                         }, {
                             "name": "EaseInBack",
                             "icon": "qrc:/image/assets/curve/easeInBack.png",
                             "key": "back.easeIn"
                         }, {
                             "name": "EaseOutBack",
                             "icon": "qrc:/image/assets/curve/easeOutBack.png",
                             "key": "back.easeOut"
                         }, {
                             "name": "EaseInOutBack",
                             "icon": "qrc:/image/assets/curve/easeInOutBack.png",
                             "key": "back.easeInOut"
                         }, {
                             "name": "EaseInElastic",
                             "icon": "qrc:/image/assets/curve/easeInElastic.png",
                             "key": "elastic.easeIn"
                         }, {
                             "name": "EaseOutElastic",
                             "icon": "qrc:/image/assets/curve/easeOutElastic.png",
                             "key": "elastic.easeOut"
                         }, {
                             "name": "EaseInOutElastic",
                             "icon": "qrc:/image/assets/curve/easeInOutElastic.png",
                             "key": "elastic.easeInOut"
                         }, {
                             "name": "EaseInBounce",
                             "icon": "qrc:/image/assets/curve/easeInBounce.png",
                             "key": "bounce.easeIn"
                         }, {
                             "name": "EaseOutBounce",
                             "icon": "qrc:/image/assets/curve/easeOutBounce.png",
                             "key": "bounce.easeOut"
                         }, {
                             "name": "EaseInOutBounce",
                             "icon": "qrc:/image/assets/curve/easeInOutBounce.png",
                             "key": "bounce.easeInOut"
                         }]
        tweenListModel.append(tweenList)
    }
}
