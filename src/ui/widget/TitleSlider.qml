import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3

RowLayout {
    id: sliderRoot
    property alias spinBoxItem: spinBox
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
    signal keyFrameCheckedChanged(bool checked)

    property double beginTouchValue: 0
    property bool keyframeEnable: false
    property bool isKeyframeChecked: false
    property var keyFrameTweenName: "linear"


    width: parent.width
    height: parent.height

    onValueChanged: {
        if(Math.abs(seekbar.slider.value - value) > 0.00001){
            seekbar.slider.value = value
        }
        if(Math.abs(spinBox.realValue() - value) > 0.0001){
            spinBox.updateRealValue(value)
        }

    }

    onIsKeyframeCheckedChanged: {
        keyframeCheckBox.updateChecked(isKeyframeChecked)
    }

    function isUserChanged(){
        return seekbar.slider.pressed || spinBox.isUserChanged()
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

    SeekBar {
        id: seekbar
        Layout.fillWidth: true
        Layout.minimumWidth: 100
        Layout.leftMargin: 20
        Layout.rightMargin: 18
        slider {
            orientation: Qt.Horizontal
            from: sliderRoot.from
            to: sliderRoot.to
            stepSize: sliderRoot.stepSize
            value: sliderRoot.value

            onValueChanged: {
                sliderRoot.value = (seekbar.slider.value)
            }
        }
        onSliderValueChanged: {
            sliderRoot.sliderValueChanged(beginValue, endValue)

            console.log("Slider:onSliderPressedChanged Press: Value:"
                        + beginValue + " -> " + endValue)
        }

        height: 30
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
