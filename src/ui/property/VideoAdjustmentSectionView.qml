import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import VideoComponent 1.0
import "../widget"

Item {
    clip: true


    VideoComponent{
        id: videoComponent
        onBindComponent:{
            var keys = ["0:Brightness","0:Contrast","0:Saturation","0:Sharpen","0:Highlight","0:Shadow","0:Fade","0:Temperature","0:Tone"]
            for(const key of keys){
                updateOfParamValue(key)
                updateKeyframeChecked(key)
            }
        }
        onOfParamChanged:{
            updateOfParamValue(key)
        }

        onKeyFrameStateChanged: {
            updateKeyframeChecked(key)
        }
    }

    function updateOfParamValue(key){
        var value = videoComponent.getOfParamVariantValue(key,0)
        if(key === "0:Brightness"){
            brightSlider.value = value
        }else if(key === "0:Contrast"){
            contrastSlider.value = value
        }else if(key === "0:Saturation"){
            saturationSlider.value = value
        }else if(key ===  "0:Sharpen"){
            sharpenSlider.value = value
        }else if(key === "0:Highlight"){
            hightlightSlider.value = value
        }else if(key === "0:Fade"){
            fadeSlider.value = value
        }else if(key ===  "0:Shadow"){
            shadowSlider.value = value
        }else if(key ===  "0:Temperature"){
            temperatureSlider.value = value
        }else if(key === "0:Tone"){
            toneSlider.value = value
        }
        console.log("onOfParamChanged:"+key + " Value:"+value)
    }

    function updateKeyframeChecked(key){
        if(key === "0:Brightness"){
            brightSlider.isKeyframeChecked = videoComponent.isCurKeyFrame(key)
        }else if(key === "0:Contrast"){
            contrastSlider.isKeyframeChecked = videoComponent.isCurKeyFrame(key)
        }else if(key === "0:Saturation"){
            saturationSlider.isKeyframeChecked = videoComponent.isCurKeyFrame(key)
        }else if(key ===  "0:Sharpen"){
            sharpenSlider.isKeyframeChecked = videoComponent.isCurKeyFrame(key)
        }else if(key === "0:Highlight"){
            hightlightSlider.isKeyframeChecked = videoComponent.isCurKeyFrame(key)
        }else if(key === "0:Fade"){
            fadeSlider.isKeyframeChecked = videoComponent.isCurKeyFrame(key)
        }else if(key ===  "0:Shadow"){
            shadowSlider.isKeyframeChecked = videoComponent.isCurKeyFrame(key)
        }else if(key ===  "0:Temperature"){
            temperatureSlider.isKeyframeChecked = videoComponent.isCurKeyFrame(key)
        }else if(key === "0:Tone"){
            toneSlider.isKeyframeChecked = videoComponent.isCurKeyFrame(key)
        }
    }

    ScrollView{
        anchors.fill: parent
        contentHeight: layoutRoot.height
        contentWidth: width
        id:wordRootScrollView

        ColumnLayout {
            id: layoutRoot
            x:editPropertyPanelHMargin
            width:parent.width - 2 * editPropertyPanelHMargin
            spacing: 12
            height: implicitHeight

            TitleMidSlider {
                id: brightSlider
                Layout.fillWidth: true
                width: parent.width
                height: 30
                title: qsTr("Brightness")
                from: -50
                to: 50
                value: 0
                comp: videoComponent
                ofParamKey: "0:Brightness"
                stepSize: 1
                decimal: 0
                valueSuffix: ""
                onValueChanged: {
                    if(isUserChanged()){
                        videoComponent.updateOfParamVariantValueBySilent("0:Brightness", value)
                    }
                }
                onSliderValueChanged: {
                    videoComponent.updateOfParamVariantValueByUser("0:Brightness",endValue,beginValue)
                }
                keyframeEnable:true
                onKeyFrameCheckedChanged: {
                    if(checked){
                         videoComponent.enableKeyFrame("0:Brightness",value)
                    }else{
                        videoComponent.removeKeyFrame("0:Brightness")
                    }
                }
            }

            TitleMidSlider {
                id: contrastSlider
                width: parent.width
                Layout.fillWidth: true
                height: 30
                title: qsTr("Contrast")
                from: -50
                to: 50
                value: 0
                comp: videoComponent
                ofParamKey: "0:Contrast"
                stepSize: 1
                decimal: 0
                valueSuffix: ""
                onValueChanged: {
                    if(isUserChanged()){
                        videoComponent.updateOfParamVariantValueBySilent("0:Contrast", value)
                    }

                }
                onSliderValueChanged: {
                    videoComponent.updateOfParamVariantValueByUser("0:Contrast",endValue,beginValue)
                }
                keyframeEnable:true
                onKeyFrameCheckedChanged: {
                    if(checked){
                         videoComponent.enableKeyFrame("0:Contrast",value)
                    }else{
                        videoComponent.removeKeyFrame("0:Contrast")
                    }
                }
            }

            TitleMidSlider {
                id: saturationSlider
                width: parent.width
                Layout.fillWidth: true
                height: 30
                title: qsTr("Saturation")
                from: -50
                to: 50
                value: 0
                stepSize: 1
                decimal: 0
                comp: videoComponent
                ofParamKey: "0:Saturation"
                valueSuffix: ""
                onValueChanged: {
                    if(isUserChanged()){
                        videoComponent.updateOfParamVariantValueBySilent("0:Saturation", value)
                    }

                }
                onSliderValueChanged: {
                    videoComponent.updateOfParamVariantValueByUser("0:Saturation",endValue,beginValue)
                }
                keyframeEnable:true
                onKeyFrameCheckedChanged: {
                    if(checked){
                         videoComponent.enableKeyFrame("0:Saturation",value)
                    }else{
                        videoComponent.removeKeyFrame("0:Saturation")
                    }
                }
            }

            TitleSlider {
                id: sharpenSlider
                width: parent.width
                Layout.fillWidth: true
                height: 30
                title: qsTr("Sharpen")
                from: 0
                to: 100
                value: 0
                stepSize: 1
                decimal: 0
                comp: videoComponent
                ofParamKey: "0:Sharpen"
                valueSuffix: ""
                onValueChanged: {
                    if(isUserChanged()){
                        videoComponent.updateOfParamVariantValueBySilent("0:Sharpen", value)
                    }

                }
                onSliderValueChanged: {
                    videoComponent.updateOfParamVariantValueByUser("0:Sharpen",endValue,beginValue)
                }
                keyframeEnable:true
                onKeyFrameCheckedChanged: {
                    if(checked){
                         videoComponent.enableKeyFrame("0:Sharpen",value)
                    }else{
                        videoComponent.removeKeyFrame("0:Sharpen")
                    }
                }
            }

            TitleSlider {
                id: hightlightSlider
                width: parent.width
                Layout.fillWidth: true
                height: 30
                title: qsTr("Highlight")
                from: 0
                to: 100
                value: 0
                comp: videoComponent
                ofParamKey: "0:Highlight"
                stepSize: 1
                decimal: 0
                valueSuffix: ""
                onValueChanged: {
                    if(isUserChanged()){
                        videoComponent.updateOfParamVariantValueBySilent("0:Highlight", value)
                    }

                }
                onSliderValueChanged: {
                    videoComponent.updateOfParamVariantValueByUser("0:Highlight",endValue,beginValue)
                }
                keyframeEnable:true
                onKeyFrameCheckedChanged: {
                    if(checked){
                         videoComponent.enableKeyFrame("0:Highlight",value)
                    }else{
                        videoComponent.removeKeyFrame("0:Highlight")
                    }
                }
            }

            TitleSlider {
                id: shadowSlider
                width: parent.width
                Layout.fillWidth: true
                height: 30
                title: qsTr("Shadow")
                from: 0
                to: 100
                value: 0
                stepSize: 1
                comp: videoComponent
                ofParamKey: "0:Sharpen"
                decimal: 0
                valueSuffix: ""
                onValueChanged: {
                    if(isUserChanged()){
                        videoComponent.updateOfParamVariantValueBySilent("0:Shadow", value)
                    }

                }
                onSliderValueChanged: {
                    videoComponent.updateOfParamVariantValueByUser("0:Shadow",endValue,beginValue)
                }
                keyframeEnable:true
                onKeyFrameCheckedChanged: {
                    if(checked){
                         videoComponent.enableKeyFrame("0:Shadow",value)
                    }else{
                        videoComponent.removeKeyFrame("0:Shadow")
                    }
                }
            }

            TitleSlider {
                id: fadeSlider
                width: parent.width
                Layout.fillWidth: true
                height: 30
                title: qsTr("Fade")
                from: 0
                to: 100
                comp: videoComponent
                ofParamKey: "0:Fade"
                value: 0
                stepSize: 1
                decimal: 0
                onValueChanged: {
                    if(isUserChanged()){
                        videoComponent.updateOfParamVariantValueBySilent("0:Fade", value)
                    }

                }
                onSliderValueChanged: {
                    videoComponent.updateOfParamVariantValueByUser("0:Fade",endValue,beginValue)
                }
                keyframeEnable:true
                onKeyFrameCheckedChanged: {
                    if(checked){
                         videoComponent.enableKeyFrame("0:Fade",value)
                    }else{
                        videoComponent.removeKeyFrame("0:Fade")
                    }
                }
            }

            TitleMidSlider {
                id: temperatureSlider
                width: parent.width
                Layout.fillWidth: true
                height: 30
                title: qsTr("Temperature")
                from: -50
                to: 50
                value: 0
                comp: videoComponent
                ofParamKey: "0:Temperature"
                stepSize: 1
                decimal: 0
                valueSuffix: ""
                onValueChanged: {
                    if(isUserChanged()){
                        videoComponent.updateOfParamVariantValueBySilent("0:Temperature", value)
                    }
                }
                onSliderValueChanged: {
                    videoComponent.updateOfParamVariantValueByUser("0:Temperature",endValue,beginValue)
                }
                gradientEnable: true
                gradientEnd: "#FF7A00"
                gradientStart: "#0049FF"
                keyframeEnable:true
                onKeyFrameCheckedChanged: {
                    if(checked){
                         videoComponent.enableKeyFrame("0:Temperature",value)
                    }else{
                        videoComponent.removeKeyFrame("0:Temperature")
                    }
                }
            }

            TitleMidSlider {
                id: toneSlider
                width: parent.width
                Layout.fillWidth: true
                height: 30
                title: qsTr("Tone")
                from: -50
                to: 50
                value: 0
                comp: videoComponent
                ofParamKey: "0:Tone"
                stepSize: 1
                decimal: 0
                valueSuffix: ""
                onValueChanged: {
                    if(isUserChanged()){
                        videoComponent.updateOfParamVariantValueBySilent("0:Tone", value)
                    }
                }
                onSliderValueChanged: {
                    videoComponent.updateOfParamVariantValueByUser("0:Tone",endValue,beginValue)
                }
                gradientEnable: true
                gradientEnd: "#FF00F5"
                gradientStart: "#00CC83"
                keyframeEnable:true
                onKeyFrameCheckedChanged: {
                    if(checked){
                         videoComponent.enableKeyFrame("0:Tone",value)
                    }else{
                        videoComponent.removeKeyFrame("0:Tone")
                    }
                }
            }

            Button {
                width: 70
                height: 25
                Layout.alignment: Qt.AlignRight
                text: qsTr("Reset")
                onClicked: {
                    console.log("调节重置")
                    videoComponent.resetVideoAdjustmentParams()
                }
            }
            //占位
            Rectangle{
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.topMargin: 1
                width:parent.width
                color:"#00000000"
            }
        }
    }

    Component.onCompleted: {
        onComponentLoaded(videoComponent)
    }
}
