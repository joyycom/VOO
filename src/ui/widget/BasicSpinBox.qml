import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12
import QtQuick.Templates 2.12 as T

//qtquickcontrols2\src\imports\controls\SpinBox.qml
//from Customizing SpinBox
T.SpinBox {
    id: control

    property bool borderVisible: true          //显示边框
    property color borderColor: "darkCyan"     //边框颜色
    property color textColor: "black"          //文本颜色
    property color bgNormalColor: "white"      //文本背景色
    property color bgFocusColor: bgNormalColor //文本背景焦点色
    property color indicatorNormalColor: "black"   //按钮图标颜色
    property color indicatorDisableColor: "gray"
    property color btnNormalColor: "white"     //按钮背景颜色
    property color btnHoverColor: Qt.lighter(btnNormalColor)
    property color btnPressColor: Qt.lighter(btnNormalColor)
    property var valueSuffix: ""
    property var valuePrefix: ""
    property int  decimal: 0

    property double beginPressedValue : 0

    signal spinValueChanged(var beginValue,var endValue);

    property var inputRegex: ""

    property var textFromValueWrapper: function(v,locale){
        return valuePrefix+v.toFixed(decimal)+valueSuffix
    }

    property var valueFromTextWrapper: function(t,locale){
        var start = 0
        if(valuePrefix.length > 0 && t.startsWith(valuePrefix)){
            start = valuePrefix.length
        }
        var end = t.length
        if(valueSuffix.length > 0 && t.endsWith(valueSuffix)){
            end = t.length - valueSuffix.length
        }

        var text = t.substr(start,end-start);
        console.log("valueFromText: Text:"+t+" Start:"+start+ " end:"+end+" prefix:"+valuePrefix+" sufix:"+valueSuffix + " text:"+text)

        return Number.parseFloat(text);
    }

    textFromValue: function(v,locale){
        return textFromValueWrapper(v*Math.pow(10,-decimal),locale)
    }

    valueFromText: function(t,locale){
        return (valueFromTextWrapper(t,locale)*Math.pow(10,decimal)).toFixed(0);
    }

    stepSize: 1
    implicitWidth: 120
    implicitHeight: 30
    /*implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentItem.implicitWidth + 2 * padding +
                            up.implicitIndicatorWidth +
                            down.implicitIndicatorWidth)
    implicitHeight: Math.max(implicitContentHeight + topPadding + bottomPadding,
                             implicitBackgroundHeight,
                             up.implicitIndicatorHeight,
                             down.implicitIndicatorHeight)*/

    padding: 0
    leftPadding: padding
    rightPadding: padding + (up.indicator ? up.indicator.width : 0)
    editable: true
    enabled: true
    onFocusChanged: {
        console.log("SpinBox:"+focus + " Value:"+value)
    }

    font{
        family: "SimSun"
        pixelSize: 16
    }

    onStepSizeChanged: {
    }

    RegExpValidator {
        id:regexValider
        regExp: inputRegex
    }

    onValuePrefixChanged: {
        regexValider.regExp = valuePrefix+"\d*(\\.)?\d{,"+decimal+"}"+valueSuffix
        validator = regexValider
    }

    onValueSuffixChanged: {
        regexValider.regExp = valuePrefix+"\d*(\\.)?\d{,"+decimal+"}"+valueSuffix
        validator = regexValider
    }

    onDecimalChanged: {
        if(decimal >= 1){
            stepSize = 1
        }
        regexValider.regExp = valuePrefix+"\d*(\\.)?\d{,"+decimal+"}"+valueSuffix
        validator = regexValider
    }

    function isUserChanged(){
        return up.pressed || down.pressed || focus
    }

    function updateFrom(f){
        from = f * Math.pow(10,decimal);
    }

    function updateTo(t){
        to = t * Math.pow(10,decimal);
    }

    function realValue(){
        return getRealValue(value);
    }

    function getRealValue(v){
        return v * Math.pow(10,-decimal);
    }

    function updateRealValue(realValue){
        value =  Math.round(realValue * Math.pow(10,decimal));
    }


    contentItem: Rectangle{
        z: 2
        color: control.activeFocus
               ?bgFocusColor
               :bgNormalColor;

        TextInput {
            property bool editByUser: false
            id:spinText
            anchors.fill: parent
            text: control.displayText
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
//            readOnly: !control.editable
            validator: control.validator
            inputMethodHints: control.inputMethodHints
            color: control.textColor
            selectByMouse: true
            selectionColor: "#5465FF"
            selectedTextColor: "#FFFFFF"
            enabled: true
            focus: false
            activeFocusOnPress: true
            overwriteMode :false
            font: control.font
            readOnly:false
            onAcceptableInputChanged: {
                updateInputChanged()
            }

            onTextEdited: {
                editByUser = true;
            }
            function updateInputChanged(){
                var beginValue = control.value
                console.log(" onAccepted:"+text + " control.value:"+control.value + " from:"+from+" to:"+to)
                var result = control.valueFromText(text, control.locale)
                if(result > Math.max(from,to)){
                    result = Math.max(from,to)
                }else if(result < Math.min(from,to)){
                    result = Math.min(from,to)
                }
                control.value = result
                text = Qt.binding(function() { return control.textFromValue(control.value, control.locale) })
                if(editByUser){
                    spinValueChanged(beginValue,control.value)
                }
                editByUser = false
            }

            onAccepted: {
               updateInputChanged()
            }
        }

        Rectangle{
            width: parent.width
            height: 1
            color: borderVisible?borderColor:"transparent"
        }
        Rectangle{
            width: parent.width
            height: 1
            anchors.bottom: parent.bottom
            color: borderVisible?borderColor:"transparent"
        }


    }


    up.indicator: Rectangle {
        x: parent.width - width
        height: parent.height/2
        implicitWidth: control.height
        implicitHeight: control.height/2
        color: up.pressed
               ?btnPressColor
               :up.hovered
                 ?btnHoverColor
                 :btnNormalColor
        border.width: borderVisible?1:0
        border.color: borderColor

        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: parent.width / 3
            height: 2
            color: enabled ? indicatorNormalColor : indicatorDisableColor
        }
        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: 2
            height: parent.width / 3
            color: enabled ? indicatorNormalColor : indicatorDisableColor
        }
    }

    down.indicator: Rectangle {
        x: parent.width - width
        height: parent.height/2
        y:control.height/2
        implicitWidth: control.height
        implicitHeight: control.height/2
        color: down.pressed
               ?btnPressColor
               :down.hovered
                 ?btnHoverColor
                 :btnNormalColor
        border.width: borderVisible?1:0
        border.color: borderColor


        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: parent.width / 3
            height: 2
            color: enabled ? indicatorNormalColor : indicatorDisableColor
        }

    }

    up.onPressedChanged: {
        if(up.pressed){
            beginPressedValue = control.value
        }else{
            spinValueChanged(beginPressedValue,control.value)
        }
        console.log("up.onPressedChanged:"+up.pressed)
    }
    down.onPressedChanged: {
     console.log("down.onPressedChanged:"+down.pressed + " value:"+value+" Text:"+displayText)
        if(down.pressed){
            beginPressedValue = control.value
        }else{
            spinValueChanged(beginPressedValue,control.value)
        }
    }

//    MouseArea{
//        anchors.fill: parent
//        //避免和handle冲突
//        acceptedButtons: Qt.NoButton
//        onWheel: {
//            if(wheel.angleDelta.y<0){
//                control.decrease();
//            }else{
//                control.increase();
//            }
//        }
//        onClicked: {
//            console.log("Mouse-----------------------")
//        }
//    }

    Keys.onPressed: {
        if(event.key === Qt.Key_Control){
            if(decimal >= 1){
                stepSize = 10
            }
            console.log("Key Control onPressed stepSize:"+stepSize)
        }
    }

    Keys.onReleased: {
        if(event.key === Qt.Key_Control){
            if(decimal >= 1){
                stepSize = 1
            }
            console.log("Key Control onReleased stepSize:"+stepSize)
        }
    }


    //不需要background
    background: Item{
        implicitWidth: 120
        implicitHeight: 30
    }
}
