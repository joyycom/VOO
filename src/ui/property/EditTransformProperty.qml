import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import "../widget"
import SimpleComponent 1.0

ColumnLayout {
    readonly property string of_key_scaleX : "0:ScaleX"
    readonly property string of_key_scaleY : "0:ScaleY"

    readonly property string of_key_rotate: "0:Rotate"
    readonly property string of_key_transx: "0:TransX"
    readonly property string of_key_transy: "0:TransY"

    property int scale_max_value: 1000
    property int scale_decimal: 2

    property var component:null

    property int paramIndex
    property int value
    clip: true
    height: 120
    width: parent.width
    Layout.fillWidth: true

    SimpleComponent{
        id:simpleComponent

    }

    Connections{
        target: component

        onOfParamChanged: {
            updateOfParamChanged(key)
        }

        onBindComponent: {
            updateOfParams()
        }

        onKeyFrameStateChanged: {
            updateKeyFrameStateChange(key)
        }
    }

    function updateKeyFrameStateChange(key){
        if(key === of_key_scaleX){
            keyframeScaleCheckBox.updateChecked(component.isCurKeyFrame(key))
        }else if(key === of_key_scaleY){
            keyframeScaleCheckBox.updateChecked(component.isCurKeyFrame(key))
        }else  if(key === of_key_rotate){
            keyframeRotateCheckBox.updateChecked(component.isCurKeyFrame(key))
        }else if(key === of_key_transx){
            keyframeTransCheckBox.updateChecked(component.isCurKeyFrame(key))
        }else if(key ===  of_key_transy){
            keyframeTransCheckBox.updateChecked(component.isCurKeyFrame(key))
        }
    }

    function updateOfParamChanged(key, defVal){
        var value = component.getOfParamVariantValue(key,defVal)
        if(key === of_key_scaleX){
            scale_X.updateRealValue(component.getOfParamVariantValue(of_key_scaleX,0))
        }else if(key === of_key_scaleY){
            scale_Y.updateRealValue(component.getOfParamVariantValue(of_key_scaleY,0))
        }else if(key === of_key_rotate){
            spin_rotate.updateRealValue(value)
        }else if(key === of_key_transx){
            spin_X.updateRealValue(component.getOfParamVariantValue(of_key_transx,0))
        }else if(key ===  of_key_transy){
            spin_Y.updateRealValue(component.getOfParamVariantValue(of_key_transy,0))

        }
    }

    function updateOfParams(){
        var keys = [of_key_scaleX,of_key_scaleY,of_key_rotate,of_key_transx,of_key_transy]
        var defVals = [1.0,1.0,0,0,0]
        for(var i=0;i<keys.length;++i){
            updateOfParamChanged(keys[i],defVals[i])
            updateKeyFrameStateChange(keys[i],defVals[i])
        }

    }


    RowLayout {
        id:scaleLayout
        height: 30
        width:parent.width

        BasicCheckBox{
            id:keyframeScaleCheckBox
            height: 30
            width: 30
            textColor: "#FFFFFF"
            indicatorWidth:10
            implicitWidth:25
            backgroundColor:"#00FF0000"
            indicatorColor: "#666666"
            checked: false
            visible: true

            onCheckedUserChanged: {
                var keys = [of_key_scaleX,of_key_scaleY]

                if(checked){
                    var values = [scale_X.realValue(),scale_Y.realValue()]
                    component.enableKeyFrames(keys,values)
                }else{
                    component.removeKeyFrames(keys)
                }
            }

            indicator: Image {
                sourceSize.width: 20
                sourceSize.height: 20
                y: (parent.height - height) / 2
                source: parent.checkState === Qt.Checked ? "qrc:/image/assets/keyframe_enable.png" : "qrc:/image/assets/keyframe_disable.png"
            }
        }

        Text{
            id:scaleTitle
            text:qsTr("scale","Scale")
            width: 16
            height: 20
            color:"#FFFFFF"
        }

        Text{
            id:title_scale_x
            text:"X"
            width: 16
            height: 20
            Layout.leftMargin: 20
            color:"#FFFFFF"
        }

        BasicSpinBox{
            id:scale_X
            from:-scale_max_value
            to:scale_max_value
            inputMethodHints: Qt.ImhDigitsOnly
            borderVisible:true
            width: 80
            implicitWidth:80
            decimal: scale_decimal
            height:30
            Layout.leftMargin: 4
            borderColor:"#2D2F33"
            bgNormalColor:"#2D2F33"
            textColor: "#BFBFBF"
            btnNormalColor:"#2D2F33"
            indicatorNormalColor:"#BFBFBF"
            indicatorDisableColor:Qt.darker(indicatorNormalColor)
            font{
                pixelSize: 12
            }
            value: 0
            editable:true
            onValueChanged: {
                var curValue = component.getOfParamVariantValue(of_key_scaleX,0)
                if(component !== null && isUserChanged() && Math.abs(curValue - scale_X.realValue()) > 0.5 * Math.pow(10,-decimal)){
                    component.updateOfParamVariantValueBySilent(of_key_scaleX,scale_X.realValue())
                }
            }
            onSpinValueChanged: {
                component.updateOfParamVariantValueByUser(of_key_scaleX, scale_X.getRealValue(endValue),scale_X.getRealValue(beginValue))
            }
        }

        Text{
            id:title_scale_y
            text:"Y"
            width: 16
            height: 20
            Layout.leftMargin: 20
            color:"#FFFFFF"
        }

        BasicSpinBox{
            id:scale_Y
            from:-scale_max_value
            to:scale_max_value
            width: 80
            implicitWidth:80
            height:30
            decimal: scale_decimal
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
            value: 0
            editable:true
            onValueChanged: {
                var curValue = component.getOfParamVariantValue(of_key_scaleY,0)
                if(component !== null && isUserChanged() && Math.abs(curValue - scale_Y.realValue()) > 0.5 * Math.pow(10,-decimal)){
                    component.updateOfParamVariantValueBySilent(of_key_scaleY,scale_Y.realValue())
                }
            }
            onSpinValueChanged: {
                component.updateOfParamVariantValueByUser(of_key_scaleY, scale_Y.getRealValue(endValue),scale_Y.getRealValue(beginValue))
            }
        }
        Item{
            width: 30
            height: 30
            Image{
                width: 20
                height: 20
                anchors.centerIn: parent
                sourceSize.height: 20
                sourceSize.width: 20
                source: "qrc:/image/assets/keyframe_curve.png"

            }
            MouseArea{
                id:keyframeScaleXYTweenMouse
                anchors.fill: parent

                onClicked: {
                    onKeyFrameTweenClick(of_key_scaleX)
                }
            }
        }

        Item{
            Layout.fillWidth: true
            height: 30
        }

    }




    RowLayout {
        id:rotateLayout
        height: 30
        width:parent.width

        BasicCheckBox{
            id:keyframeRotateCheckBox
            height: 30
            width: 30
            textColor: "#FFFFFF"
            indicatorWidth:10
            implicitWidth:25
            backgroundColor:"#00FF0000"
            indicatorColor: "#666666"
            checked: false
            visible: true

            onCheckedUserChanged: {
                if(checked){
                     component.enableKeyFrame(of_key_rotate,0)
                }else{
                    component.removeKeyFrame(of_key_rotate)
                }
            }

            indicator: Image {
                sourceSize.width: 20
                sourceSize.height: 20
                y: (parent.height - height) / 2
                source: parent.checkState === Qt.Checked ? "qrc:/image/assets/keyframe_enable.png" : "qrc:/image/assets/keyframe_disable.png"
            }
        }

        Text{
            id:rotateTitle
            text:qsTr("rotate","Rotate")
            width: 16
            height: 20
            color:"#FFFFFF"
        }


        BasicSpinBox{
            id:spin_rotate
            from:-99999
            to:99999
            inputMethodHints: Qt.ImhDigitsOnly
            borderVisible:true
            width: 120
            implicitWidth:120
            decimal: 1
            height:30
            Layout.leftMargin: 4
            valueSuffix: "°"
            borderColor:"#2D2F33"
            bgNormalColor:"#2D2F33"
            textColor: "#BFBFBF"
            btnNormalColor:"#2D2F33"
            indicatorNormalColor:"#BFBFBF"
            indicatorDisableColor:Qt.darker(indicatorNormalColor)
            font{
                pixelSize: 12
            }
            value: 0
            editable:true
            onValueChanged: {
                var curValue = component.getOfParamVariantValue(of_key_rotate,0)
                if(component !== null && isUserChanged() && Math.abs(curValue - spin_rotate.realValue()) > 0.5 * Math.pow(10,-decimal)){
                    component.updateOfParamVariantValueBySilent(of_key_rotate,spin_rotate.realValue())
                }
            }
            onSpinValueChanged: {
                component.updateOfParamVariantValueByUser(of_key_rotate, spin_rotate.getRealValue(endValue),spin_rotate.getRealValue(beginValue))
            }
        }

        Item{
            width: 30
            height: 30
            Image{
                width: 20
                height: 20
                anchors.centerIn: parent
                sourceSize.height: 20
                sourceSize.width: 20
                source: "qrc:/image/assets/keyframe_curve.png"

            }
            MouseArea{
                id:keyframeRotateTweenMouse
                anchors.fill: parent

                onClicked: {
                    onKeyFrameTweenClick(of_key_rotate)
                }
            }
        }
    }



    RowLayout {
        id:transLayout
        height: 30
        width:parent.width

        BasicCheckBox{
            id:keyframeTransCheckBox
            height: 30
            width: 30
            textColor: "#FFFFFF"
            indicatorWidth:10
            implicitWidth:25
            backgroundColor:"#00FF0000"
            indicatorColor: "#666666"
            checked: false
            visible: true

            onCheckedUserChanged: {
                var keys = [of_key_transx,of_key_transy]

                if(checked){
                    var values = [spin_X.realValue(),spin_Y.realValue()]
                    component.enableKeyFrames(keys,values)
                }else{
                    component.removeKeyFrames(keys)
                }
            }

            indicator: Image {
                sourceSize.width: 20
                sourceSize.height: 20
                y: (parent.height - height) / 2
                source: parent.checkState === Qt.Checked ? "qrc:/image/assets/keyframe_enable.png" : "qrc:/image/assets/keyframe_disable.png"
            }
        }

        Text{
            id:transTitle
            text:qsTr("locate","Locate")
            width: 16
            height: 20
            color:"#FFFFFF"
        }

        Text{
            id:title_x
            text:"X"
            width: 16
            height: 20
            Layout.leftMargin: 20
            color:"#FFFFFF"
        }

        BasicSpinBox{
            id:spin_X
            from:-99999
            to:99999
            inputMethodHints: Qt.ImhDigitsOnly
            borderVisible:true
            width: 80
            implicitWidth:80
            decimal: 1
            height:30
            Layout.leftMargin: 4
            borderColor:"#2D2F33"
            bgNormalColor:"#2D2F33"
            textColor: "#BFBFBF"
            btnNormalColor:"#2D2F33"
            indicatorNormalColor:"#BFBFBF"
            indicatorDisableColor:Qt.darker(indicatorNormalColor)
            font{
                pixelSize: 12
            }
            value: 0
            editable:true
            onValueChanged: {
                var curValue = component.getOfParamVariantValue(of_key_transx,0)
                if(component !== null && isUserChanged() && Math.abs(curValue - spin_X.realValue()) > 0.5 * Math.pow(10,-decimal)){
                    component.updateOfParamVariantValueBySilent(of_key_transx,spin_X.realValue())
                }
            }
            onSpinValueChanged: {
                component.updateOfParamVariantValueByUser(of_key_transx, spin_X.getRealValue(endValue),spin_X.getRealValue(beginValue))
            }
        }

        Text{
            id:title_y
            text:"Y"
            width: 16
            height: 20
            Layout.leftMargin: 20
            color:"#FFFFFF"
        }

        BasicSpinBox{
            id:spin_Y
            from:-99999
            to:99999
            width: 80
            implicitWidth:80
            height:30
            decimal: 1
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
            value: 0
            editable:true
            onValueChanged: {
                var curValue = component.getOfParamVariantValue(of_key_transy,0)
                if(component !== null && isUserChanged() && Math.abs(curValue - spin_Y.realValue()) > 0.5 * Math.pow(10,-decimal)){
                    component.updateOfParamVariantValueBySilent(of_key_transy,spin_Y.realValue())
                }
            }
            onSpinValueChanged: {
                component.updateOfParamVariantValueByUser(of_key_transy, spin_Y.getRealValue(endValue),spin_Y.getRealValue(beginValue))
            }
        }
        Item{
            width: 30
            height: 30
            Image{
                width: 20
                height: 20
                anchors.centerIn: parent
                sourceSize.height: 20
                sourceSize.width: 20
                source: "qrc:/image/assets/keyframe_curve.png"

            }
            MouseArea{
                id:keyframeXYTweenMouse
                anchors.fill: parent

                onClicked: {
                    onKeyFrameTweenClick(of_key_transx)
                }
            }
        }

        Item{
            Layout.fillWidth: true
            height: 30
        }

    }

    function onKeyFrameTweenClick(key){
        var c = Qt.createComponent(
                    "qrc:/ui/dialog/TweenSettingDialog.qml")
        if (c.status === Component.Error) {
            console.error("Error:" + component.errorString())
            return
            // or maybe throw
        }
        var curTweenName = component.keyFrameTweenType(key)
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
        if(ofParamKey === of_key_transx){
            var keys = [of_key_transx,of_key_transy]
            component.updateKeyFrameTweenTypes(keys,tween)
        }else if(ofParamKey === of_key_scaleX){
            var keys = [of_key_scaleX,of_key_scaleY]
            component.updateKeyFrameTweenTypes(keys,tween)
        } else{
            component.updateKeyFrameTweenType(ofParamKey,tween)
        }
    }

    Component.onCompleted: {
        console.log("SimpleTransformProperty onCompleted:"+component)
        if(component === null){
            component = propertyEditController.transformComponent()
            if(!component.hadBindComponent()){
                onComponentLoaded(component)
            }else{
                console.log("SimpleTransformProperty attachComponent1:"+simpleComponent + " transformComponent: "+component)
                onComponentLoaded(simpleComponent)
            }
//            component = simpleComponent
//            onComponentLoaded(simpleComponent)
        }else{
            console.log("SimpleTransformProperty Component.onCompleted transformComponent:"+component)
        }
        spin_X.updateFrom(component.minTransX())
        spin_X.updateTo(component.maxTransX())


        spin_Y.updateFrom(component.minTransY())
        spin_Y.updateTo(component.maxTransY())


//        scaleSlider.to = component.maxScaleX()
        updateOfParams()

    }
}
