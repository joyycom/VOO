import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import VideoComponent 1.0
import "../widget"

Item {

    height:parent.height
    width: parent.width
    property int defOpacityValue: 100
    property int defBeautyValue: 0
    property int defThinFaceValue: 0
    property var blendModes: ["Normal","Darken","Multiply","CorlorBurn","LinearBurn","DarkerColor","Lighten","Screen","ColorDodge","LinearDodge","LighterColor","Overlay","SoftLight","HardLight","VividLight","LinearLight","PinLight","HardMix","Difference","Exclusion","Substract","Divide","Add" ]
    property var blendModeValues: [0,2,10,8,7,17,3,1,9,14,16,4,6,5,18,13,19,20,15,21,11,22,12]

    VideoComponent{
        id: videoComponent
        onBindComponent:{
            updateOfParamValue("0:Opacity")
            updateOfParamValue("0:BlendMode")

        }
        onOfParamChanged:{

            updateOfParamValue(key)
        }

        onKeyFrameStateChanged: {
            if(key === "0:Opacity"){
                opacitySlider.isKeyframeChecked = videoComponent.isCurKeyFrame(key)
            }else{

            }
        }
    }

    function updateOfParamValue(key){
        var value = videoComponent.getOfParamVariantValue(key,0)
        if(key === "0:Opacity"){
            value = videoComponent.getOfParamVariantValue(key,100)
            opacitySlider.value = value
            opacitySlider.isKeyframeChecked = videoComponent.isCurKeyFrame(key)
        }else if(key === "0:Beauty"){
            beautySlider.value = value
        }else if(key === "0:ThinFace"){
            thinFaceSlider.value = value
        }else if(key ===  "0:BlendMode"){
            var index = videoComponent.getOfParamVariantValue("0:BlendMode",0)
            for(var i=0;i<blendModeValues.length;++i){
                if(index === blendModeValues[i]){
                    blendComboBox.currentIndex = i;
                    break;
                }
            }
        }
        console.log("onOfParamChanged:"+key + " Value:"+value)
    }

    ColumnLayout {
        x:editPropertyPanelHMargin
        width:parent.width - 2 * editPropertyPanelHMargin

        TitleSlider {
            id: opacitySlider
            width: parent.width
            Layout.fillWidth: true
            height: 30
            title: qsTr("Opacity")
            from: 0
            to: 100
            value: 100
            comp: videoComponent
            ofParamKey: "0:Opacity"
            stepSize: 0.1
            decimal: 1
            valueSuffix: "%"
            keyframeEnable:true
            onKeyFrameCheckedChanged: {
                if(checked){
                     videoComponent.enableKeyFrame("0:Opacity",value)
                }else{
                    videoComponent.removeKeyFrame("0:Opacity")
                }

            }
            onValueChanged: {
                if(isUserChanged()){
                    videoComponent.updateOfParamVariantValueBySilent("0:Opacity", value)
                }
            }
            onSliderValueChanged: {
                videoComponent.updateOfParamVariantValueByUser("0:Opacity",endValue,beginValue)
            }
        }

        EditTransformProperty{
            component: propertyEditController.transformComponent()
            id:simpleTransform
            width: parent.width
            height: 120
            Layout.fillWidth: true
        }



//        TitleSlider {
//            id: beautySlider
//            width: parent.width
//            Layout.fillWidth: true
//            height: 30
//            title: qsTr("Beauty")
//            from: 0
//            to: 100
//            value: 0
//            stepSize: 1
//            valueSuffix: "%"
//            onValueChanged: {
//                console.log("磨皮 value changed: " + value)
//                videoComponent.updateOfParamIntValueBySilent("0:Beauty", value)
//            }
//        }

//        TitleSlider {
//            id: thinFaceSlider
//            width: parent.width
//            Layout.fillWidth: true
//            height: 30
//            title: qsTr("ThinFace")
//            from: 0
//            to: 100
//            value: 0
//            stepSize: 1
//            valueSuffix: "%"
//            onValueChanged: {
//                console.log("瘦脸 value changed: " + value)
//                videoComponent.updateOfParamIntValueBySilent("0:ThinFace", value)
//            }
//        }

        Item{
            width: parent.width
            Layout.fillWidth: true
            height: 30
            Text {
                id: blendComboBoxTitle
                width: 52
                height: parent.height
                text: qsTr("Mixed mode")
                color: "#FFFFFF"
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: 12
                anchors.verticalCenter: parent.verticalCenter
            }

            BasicComboBox {
                id:blendComboBox
                editable: false
                height:parent.height
                width:160
                anchors.leftMargin: 20
                anchors.left: blendComboBoxTitle.right
                anchors.top:blendComboBoxTitle.top
                backgroundColor: "#2D2F33"
                itemNormalColor: "#2D2F33"
                borderColor: "transparent"
                itemHighlightColor: "#2D2F33"
                itemHeight:26
                showCount: 15
                currentIndex: 0
                model: blendModes
                onCurrentIndexChanged: {
                    var preIndex = videoComponent.getOfParamIntValue("0:BlendMode",0,false);
                    var curIndex = blendModeValues[currentIndex];
                    videoComponent.updateOfParamIntValueByUser("0:BlendMode",curIndex,preIndex)
                }
            }
        }

        Button {
            width: 70
            height: 25
            Layout.alignment: Qt.AlignRight
            text: qsTr("Reset")
            onClicked: {
                console.log("画面重置")
                videoComponent.resetVideoPictureParams()
            }
        }

        Rectangle{
            width: parent.width
            height: 1
            Layout.topMargin: 10
            Layout.bottomMargin: 10
            color:"#2C2C2C"
            visible: inputLayout.visible
        }

        EditInputProperty{
            component: videoComponent
            id:inputLayout
            width: parent.width
            Layout.fillWidth: true
        }
    }

    Component.onCompleted: {
        onComponentLoaded(videoComponent)
    }
}
