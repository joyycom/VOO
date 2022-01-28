import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import "../widget/"
import SliderComponent 1.0

Item {
    width:parent.width
    height: 40


    SliderComponent{
        id:sliderComponent

        onBindComponent: {
            var siliderValue = sliderComponent.sliderValue
            sliderLayout.stepSize = sliderComponent.stepSize()
            sliderLayout.decimal = sliderComponent.decimals()
            sliderLayout.from = sliderComponent.sliderMinValue()
            sliderLayout.to = sliderComponent.sliderMaxValue()
            sliderLayout.keyframeEnable = sliderComponent.isKeyFrameEnable()
            sliderLayout.value = siliderValue
            sliderLayout.title = sliderComponent.sliderText()
            sliderLayout.keyFrameTweenName = sliderComponent.sliderKeyFrameTweenType()
            sliderLayout.isKeyframeChecked = sliderComponent.isCurSliderKeyFrame()
            sliderLayout.spinBoxItem.valueSuffix = sliderComponent.sliderDisplayTextSuffix()
            sliderLayout.spinBoxItem.valuePrefix = sliderComponent.sliderDisplayTextPrefix()
            sliderLayout.ofParamKey = sliderComponent.sliderKey()
            console.log("onBindComponent After:"+ siliderValue + " Text: "+sliderLayout.value)
        }
        onSliderValueChanged: {
            sliderLayout.value = sliderComponent.sliderValue
            console.log("onSliderValueChanged:key:"+sliderComponent.sliderKey()+ " Value:"+ sliderComponent.sliderValue  + " Text: "+sliderLayout.value)
        }

        onKeyFrameStateChanged: {
            sliderLayout.keyFrameTweenName = sliderComponent.sliderKeyFrameTweenType()

            sliderLayout.isKeyframeChecked = sliderComponent.isCurSliderKeyFrame()
        }
    }

    TitleSlider {
        id: sliderLayout
        width: parent.width
        Layout.fillWidth: true
        height: 30
        from:0
        to:100
        stepSize: 0.01
        value: 0
        decimal: 2
        comp: sliderComponent
        valueSuffix: ""
        spinBoxItem.textFromValueWrapper:function(v,locate){
            return sliderComponent.sliderDisplayText(v)
        }

        spinBoxItem.valueFromTextWrapper: function(t,locate){
            return sliderComponent.sliderValueFromText(t)
        }

        onValueChanged: {
            if(sliderComponent !== null && sliderLayout.isUserChanged()){
                console.log("Slider:scaleSeekbar Value:"+sliderLayout.value)
                sliderComponent.setSliderValue(sliderLayout.value)
            }
        }

        onSliderValueChanged: {
            if(sliderComponent !== null){
                sliderComponent.setSliderValue(beginValue,endValue)
                console.log("Slider:scaleSeekbar Press: Value:"+beginValue + " -> "+endValue)
            }
        }
        onKeyFrameCheckedChanged: {
            if(checked){
                 sliderComponent.enableSliderKeyFrames()
            }else{
                sliderComponent.removeSliderKeyFrames()
            }
        }
    }

    Component.onCompleted: {
        console.log("Component==Slider:"+index + " SliderComponent:"+sliderComponent)
        onComponentLoaded(sliderComponent)
    }
}
