import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3

RowLayout {
    id: sliderRoot

    property var ofParamKey: ""
    property var comp
    property alias title: attrTitle.text
    property double from: 0
    property double to: 100
    property double stepSize: 1
    property int decimal:  0
    property var value
    property alias valueSuffix: spinBox.valueSuffix
    property alias valuePrefix: spinBox.valuePrefix
    signal sliderValueChanged(var beginValue, var endValue)
    property double beginTouchValue: 0
    property bool gradientEnable: false
    property color gradientStart: "white"
    property color gradientEnd: "black"
    signal keyFrameCheckedChanged(bool checked)
    property bool keyframeEnable: false
    property bool isKeyframeChecked: false
    width: parent.width
    height: parent.height

    onValueChanged: {
        if(Math.abs(slider.value - value) > 0.00001){
            slider.value = value
        }
        if(Math.abs(spinBox.realValue() - value) > 0.0001){
            spinBox.updateRealValue(value)
        }
    }



    onIsKeyframeCheckedChanged: {
        keyframeCheckBox.updateChecked(isKeyframeChecked)
    }
    function isUserChanged(){
        return slider.pressed || spinBox.isUserChanged()
    }

    onFromChanged: {
        spinBox.updateFrom(from)
    }

    onToChanged: {
        spinBox.updateTo(to)
    }

    onDecimalChanged: {
        spinBox.decimal = decimal
        spinBox.updateTo(to)
        spinBox.updateFrom(from)
    }

    BasicCheckBox{
        id:keyframeCheckBox
        height: 30
        width: 30
        textColor: "#FFFFFF"
        indicatorWidth:10
        implicitWidth:25
        backgroundColor:"#00FF0000"
        indicatorColor: "#666666"
        checked: false
        visible: keyframeEnable
        onCheckedUserChanged: {
            isKeyframeChecked = checked
            keyFrameCheckedChanged(checked)
        }
        indicator: Image {
            sourceSize.width: 20
            sourceSize.height: 20
            y: (parent.height - height) / 2
            source: parent.checkState === Qt.Checked ? "qrc:/image/assets/keyframe_enable.png" : "qrc:/image/assets/keyframe_disable.png"
        }
    }

    Text {
        id: attrTitle
        width: 52
        height: 30
        color: "#FFFFFF"
        font.pixelSize: 12
    }

    Slider {
        id: slider
        from: sliderRoot.from
        to: sliderRoot.to
        stepSize: sliderRoot.stepSize
        value: sliderRoot.value
        Layout.fillWidth: true
        Layout.minimumWidth: 100
        Layout.leftMargin: 20
        Layout.rightMargin: 18

        onPressedChanged: {
            if (pressed) {
                sliderRoot.beginTouchValue = slider.value
            } else {
                sliderRoot.sliderValueChanged(sliderRoot.beginTouchValue,
                                              slider.value)
            }
        }

        background: Rectangle {
            anchors.fill: parent
            color: "#00000000"

            Rectangle {
                x: 0
                y: (parent.height - height) / 2
                height: 2
                width: parent.width
                visible: !gradientEnable
                color: gradientEnable ? "#00000000" : "#3B3E47"
            }

            Rectangle {
                x: (parent.width - width)/2
                y: (parent.height - height) / 2
                height: parent.width
                width: 2
                rotation: 90
                visible: gradientEnable
                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: gradientEnd
                    }
                    GradientStop {
                        position: 1.0
                        color: gradientStart
                    }
                }
            }


            Rectangle {
                x: (slider.visualPosition
                    > 0.5) ? parent.width / 2 : slider.visualPosition * parent.width
                y: (parent.height - height) / 2
                height: 2
                width: (slider.visualPosition
                        > 0.5) ? (slider.visualPosition - 0.5)
                                 * parent.width : (0.5 - slider.visualPosition) * parent.width
                color: gradientEnable ? "#00000000" : "white"
            }

            Rectangle {
                width: 2
                height: 10
                x: (parent.width - width) / 2
                y: (parent.height - height) / 2
                color: "white"
            }
        }
        //这个拖动按钮可以找个图片替换
        handle: Item {
            id: slideHandler
            x: slider.visualPosition * (parent.width) - width / 2
            y: (slider.height - height) / 2
            width: 14
            height: 14
            Rectangle {
                id: rect_in
                anchors.centerIn: parent
                width: 10
                height: 10
                radius: width / 2
                color: "white"
            }
        }
        onValueChanged: {
            sliderRoot.value = slider.value
            console.log("Slider:ValueChanged:" + value)
        }
    }


    BasicSpinBox{
        id:spinBox
        from:sliderRoot.from
        to:sliderRoot.to
        width: 80
        implicitWidth:80
        height:30
        Layout.leftMargin: 4
        inputMethodHints: Qt.ImhDigitsOnly
        borderVisible:true
        borderColor:"#2D2F33"
        bgNormalColor:"#2D2F33"
        textColor: "#BFBFBF"
        btnNormalColor:"#2D2F33"
        indicatorNormalColor:"#BFBFBF"
        indicatorDisableColor:Qt.darker(indicatorNormalColor)
        font{
            pixelSize: 12
        }
        editable:true
        onValueChanged: {
            if(spinBox.isUserChanged()){
                sliderRoot.value = spinBox.realValue()
            } else if(Math.abs(sliderRoot.value - spinBox.realValue()) >= sliderRoot.stepSize*0.5){
                sliderRoot.value = spinBox.realValue()
            }
        }
        onSpinValueChanged: {
            sliderRoot.sliderValueChanged(spinBox.getRealValue(beginValue), spinBox.getRealValue(endValue))
        }
    }

    Item{
        width: 30
        height: 30
        visible: keyframeEnable
        Image{
            width: 20
            height: 20
            anchors.centerIn: parent
            sourceSize.height: 20
            sourceSize.width: 20
            source: "qrc:/image/assets/keyframe_curve.png"

        }

        MouseArea{
            id:keyframeTweenMouse
            anchors.fill: parent

            onClicked: {
                onKeyFrameTweenClick(ofParamKey)
            }

        }
    }

    function onKeyFrameTweenClick(key){
        var c = Qt.createComponent(
                    "qrc:/ui/dialog/TweenSettingDialog.qml")
        if (c.status === Component.Error) {
            console.error("Error:" + c.errorString())
            return
            // or maybe throw
        }
        var curTweenName = comp.keyFrameTweenType(key)
        if(curTweenName === ""){
            console.error("Error: curTweenType isEmpty")
            editToastMgr.show(qsTr("Add keyframes first"),3000)
            return
        }
        console.log("Mouse.init TweenName==="+key)
        var settingDialog = c.createObject(editorPage)
        settingDialog.selectedKey = curTweenName
        settingDialog.ofParamKey = key
        settingDialog.notifySelectedKeyChanged.connect(notifySelectedKeyChanged)
        settingDialog.show()
    }

    function notifySelectedKeyChanged(ofParamKey, tween){
        comp.updateKeyFrameTweenType(ofParamKey,tween)
    }
}
