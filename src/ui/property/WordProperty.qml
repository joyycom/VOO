import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import WordComponent 1.0
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Material 2.12

import "../widget"

Item {
    clip: true

    height:parent.height
    width: parent.width
    Layout.fillHeight: true
    Layout.fillWidth: true
    property int itemHeight: 25
    property int itemLabelWidth: 60
    property bool onFontDownload: false
    property int downloadIndex: -1

    WordComponent{
        id:wordComponent

        onBackgroundColorChanged: {
            backgroundColorSelector.selectedColor = color
            console.log("backgroundColorSelector:"+color)
        }

        onShadowColorChanged: {
            shadowColorSelector.selectedColor = color
        }
        onTextColorChanged: {
            fontColorColorSelector.selectedColor = color
            console.log("onTextColorChanged:"+color)
        }
        onStokeColorChanged: {
            selectedColorRectangle.color = color
        }
        onFontFamilyIndexChanged: {
            wordFontComboBox.currentIndex = index
            wordFontComboBox.myText = name
            wordFontComboBox.curDownloadStatus = isExist
        }
        onFontDownloading: {
            onFontDownload = downloading
            downloadIndex = index
        }

        onOfParamChanged: {
            if(key == "0:Opacity"){
                opacityLayout.value =wordComponent.getOfParamVariantValue(key)
            }else if(key === "0:Spacing"){
                spacingLayout.value = wordComponent.getOfParamVariantValue(key)
            }else if(key === "0:Leading"){
                leadingLayout.value = wordComponent.getOfParamVariantValue(key)
            }else if(key === "0:Alignment"){
                var alignBtns = [radioAlignLeft,radioAlignRight,radioAlignCenter]
                alignBtns[wordComponent.getOfParamIntValue("0:Alignment",0)].checked = true
            }
        }
        onKeyFrameStateChanged: {
            if(key == "0:Opacity"){
                opacityLayout.isKeyframeChecked = wordComponent.isCurKeyFrame(key)
            }
        }

        onBindComponent: {
            opacityLayout.value =wordComponent.getOfParamVariantValue("0:Opacity",100)
            opacityLayout.isKeyframeChecked = wordComponent.isCurKeyFrame("0:Opacity")
            spacingLayout.value =wordComponent.getOfParamVariantValue("0:Spacing",0)
            leadingLayout.value =wordComponent.getOfParamVariantValue("0:Leading",0)

            var alignBtns = [radioAlignLeft,radioAlignRight,radioAlignCenter]
            alignBtns[wordComponent.getOfParamIntValue("0:Alignment",0)].checked = true


            console.log(" WordProperty onBindComponent")

        }
    }

    ScrollView{
        anchors.fill: parent
        contentHeight: 10000
        id:wordRootScrollView
        contentWidth: parent.width

        ColumnLayout{
            x:editPropertyPanelHMargin
            width:parent.width - 2 * editPropertyPanelHMargin
            height:parent.contentHeight
            spacing: 10

            onImplicitHeightChanged: {
                wordRootScrollView.contentHeight = implicitHeight + 50
                console.log("onImplicitHeightChanged:"+implicitHeight)
            }


            Text{
                id:wordTitle
                text: qsTr("Text edit")
                width: parent.width
                Layout.fillWidth: true
                height:itemHeight
                Layout.topMargin: 10
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
                minimumPixelSize: 24
                minimumPointSize: 36
                color:"#999999"
            }


            Rectangle{
                width: parent.width
                Layout.fillWidth: true
                height: 80
                color:"#2C2E32"
                radius: 2

                Flickable {
                    id: textFlick

                    width: parent.width - 4
                    height: parent.height - 4
                    x:2
                    y:2
                    contentWidth: wordTextField.paintedWidth
                    contentHeight: wordTextField.paintedHeight
                    clip: true

                    function ensureVisible(r) {
                        if (contentX >= r.x)
                            contentX = r.x;
                        else if (contentX+width <= r.x+r.width)
                            contentX = r.x+r.width-width;
                        if (contentY >= r.y)
                            contentY = r.y;
                        else if (contentY+height <= r.y+r.height)
                            contentY = r.y+r.height-height;
                    }

                    TextEdit {
                        id: wordTextField
                        width: textFlick.width
                        height: textFlick.height
                        color: "white"
                        text: wordComponent.wordContent
                        selectByMouse: true
                        wrapMode:"WrapAtWordBoundaryOrAnywhere"
                        onCursorRectangleChanged: textFlick.ensureVisible(cursorRectangle)
                        onTextChanged: {
                            wordComponent.wordContent = text
                        }
                    }
                }
            }

            RowLayout {
                id:fontFamilyLayout
                spacing: 15
                height:itemHeight
                Layout.topMargin: 10
                width: parent.width
                Layout.fillWidth: true

                Text{
                    id:fontFamilyTitle
                    Layout.minimumWidth: 60
                    height: 20
                    text: qsTr("Font")
                    color:"#FFFFFF"
                }

                BasicComboBox {
                    id:wordFontComboBox
                    editable: false
                    height:itemHeight
                    width:parent.width
                    backgroundColor: "#2D2F33"
                    itemNormalColor: "#2D2F33"
                    borderColor: "transparent"
                    itemHighlightColor: "#2D2F33"
                    itemHeight:26
                    showCount: 15
                    currentIndex: wordComponent.fontFamilyIndex
                    model: wordComponent.fontResource
                    //                    textRole: model.name
                    property var myText: ""
                    property bool curDownloadStatus
                    onCurrentIndexChanged: {
                        wordComponent.setFontFamily(wordFontComboBox.currentIndex)
                    }

                    delegate: ItemDelegate {
                        id: box_item
                        height: wordFontComboBox.itemHeight
                        width: wordFontComboBox.width
                        padding: 0
                        contentItem: Item {
                            Image {
                                width: 16
                                height: 16
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                source: "qrc:/image/assets/res_checked.png"
                                visible: wordFontComboBox.currentIndex === index
                            }
                            Text {
                                id: content
                                text: index == 0 ? qsTr("System font") : name
                                color: wordFontComboBox.currentIndex === index ? "white" : "#BFBFBF"
                                leftPadding: wordFontComboBox.itemPadding
                                font.pixelSize: 12
                                elide: Text.ElideRight
                                renderType: Text.NativeRendering
                                verticalAlignment: Text.AlignVCenter
                                anchors.verticalCenter: parent.verticalCenter
                            }
                            Image {
                                id: downloadIcon
                                width: 16
                                height: 16
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                anchors.rightMargin: 10
                                visible: !isDownload && index != 0 && !(onFontDownload && downloadIndex === index)
                                source: "qrc:/image/assets/res_download.png"
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        if(!onFontDownload){
                                            wordComponent.clickFontDownload(index)
                                        }
                                    }
                                }
                            }
                            Image {
                                id: downloading_icon
                                anchors.fill: downloadIcon
                                visible: onFontDownload && downloadIndex === index
                                source: "qrc:/image/assets/res_downloading.png"
                            }
                            RotationAnimation {
                                id: rotationAnimation
                                target: downloading_icon
                                running: onFontDownload && downloadIndex === index
                                from: 0
                                to: 360
                                direction: RotationAnimation.Clockwise
                                duration: 500
                                loops: Animation.Infinite
                            }
                        }
                        hoverEnabled: wordFontComboBox.hoverEnabled
                        background: Rectangle {
                            color: (wordFontComboBox.highlightedIndex
                                    === index) ? wordFontComboBox.itemHighlightColor : wordFontComboBox.itemNormalColor
                        }
                    }

                    contentItem: TextField {
                        leftPadding: 8
                        text: wordFontComboBox.editable ? wordFontComboBox.editText :
                                                          wordFontComboBox.currentIndex != 0 ? wordFontComboBox.myText:
                                                                                               qsTr("System font")
                        font.pixelSize: 12
                        color: "#BFBFBF"
                        verticalAlignment: Text.AlignVCenter
                        //默认鼠标选取文本设置为false
                        selectByMouse: true
                        //选中文本的颜色
                        selectedTextColor: "green"
                        //选中文本背景色
                        selectionColor: "white"
                        clip: true
                        //renderType: Text.NativeRendering
                        enabled: wordFontComboBox.editable
                        autoScroll: wordFontComboBox.editable
                        readOnly: wordFontComboBox.down
                        inputMethodHints: wordFontComboBox.inputMethodHints
                        validator: wordFontComboBox.validator
                        renderType: Text.NativeRendering
                        background: Rectangle {
                            visible: wordFontComboBox.enabled && wordFontComboBox.editable
                            border.width: parent && parent.activeFocus ? 1 : 0
                            border.color: wordFontComboBox.itemHighlightColor
                            color: "transparent"
                        }

                        Image {
                            id: notExist_icon
                            source: "qrc:/image/assets/font_notExist.png"
                            width: 14
                            height: 14
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: 5
                            visible: !wordFontComboBox.curDownloadStatus && wordFontComboBox.currentIndex != 0
                        }
                    }
                }
            }

            RowLayout {
                id:fontStyleLayout
                spacing: 15
                height:itemHeight
                width: parent.width
                Layout.fillWidth: true

                Text{
                    id:fontStyleTitle
                    width: itemLabelWidth
                    height: 20
                    text: qsTr("Font style")
                    color:"#FFFFFF"
                }

                Text{
                    id:fontStyleBold
                    width: 30
                    height: 24
                    font.bold:true
                    text:"B"
                    color:wordComponent.isTextBold?"#02E4D6":"#FFFFFF"

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            wordComponent.setTextBold(!wordComponent.isTextBold)
                        }
                    }
                }

//                Text{
//                    id:fontStyleItalic
//                    width: 30
//                    height: 24
//                    font.italic:true
//                    text:"I"
//                    color:wordComponent.isTextItalic?"#02E4D6":"#FFFFFF"
//                    MouseArea{
//                        anchors.fill: parent
//                        onClicked: {
//                            wordComponent.setTextItalic(!wordComponent.isTextItalic)
//                        }
//                    }
//                }

//                Text{
//                    id:fontStyleUnderLine
//                    width: 30
//                    height: 24
//                    font.underline:true
//                    text:"U"
//                    color:wordComponent.isUnderline?"#02E4D6":"#FFFFFF"
//                    MouseArea{
//                        anchors.fill: parent
//                        onClicked: {
//                            wordComponent.setUnderline(!wordComponent.isUnderline)
//                        }
//                    }
//                }

            }



            RowLayout {
                id:alignStyleLayout
                spacing: 10
                height:itemHeight
                width: parent.width
                Layout.fillWidth: true

                Text{
                    id:alignStyleTitle
                    width: 80
                    height: 20
                    text: qsTr("Align")
                    color:"#FFFFFF"
                }

                ButtonGroup {
                    id:alignBtnGroup
                }
                BasicRadioButton {
                    id:radioAlignLeft
                    text: ""
                    height: 20
                    width: 20
                    ButtonGroup.group: alignBtnGroup
                    indicator:
                        Image{
                        anchors.centerIn: parent
                        source:radioAlignLeft.checked?"qrc:/image/assets/text_align_left_checked.png":"qrc:/image/assets/text_align_left_unchecked.png"
                        width:20
                        height: 20
                    }



                    onCheckedChanged: {
                        var preValue = wordComponent.getOfParamIntValue("0:Alignment",0)
                        if(checked && preValue !== 0){
                            wordComponent.updateOfParamIntValueByUser("0:Alignment",0,preValue)
                        }
                    }
                }

                BasicRadioButton {
                    text: ""
                    id:radioAlignCenter
                    height: 20
                    width: 20
                    ButtonGroup.group: alignBtnGroup
                    checkable: true

                    checked:true

                    indicator:
                        Image{
                        anchors.centerIn: parent
                        source:radioAlignCenter.checked?"qrc:/image/assets/text_align_center_checked.png":"qrc:/image/assets/text_align_center_unchecked.png"
                        width:20
                        height: 20
                    }

                    onCheckedChanged: {
                        var preValue = wordComponent.getOfParamIntValue("0:Alignment",0)
                        if(checked && preValue !== 2){
                            wordComponent.updateOfParamIntValueByUser("0:Alignment",2,preValue)
                        }
                    }
                }
                BasicRadioButton {
                    text: ""
                    id:radioAlignRight
                    height: 20
                    width: 20
                    ButtonGroup.group: alignBtnGroup
                    indicator:Image{
                        anchors.centerIn: parent
                        source:radioAlignRight.checked?"qrc:/image/assets/text_align_right_checked.png":"qrc:/image/assets/text_align_right_unchecked.png"
                        width:20
                        height: 20
                    }

                    onCheckedChanged: {
                        var preValue = wordComponent.getOfParamIntValue("0:Alignment",0)
                        if(checked && preValue !== 1){
                            wordComponent.updateOfParamIntValueByUser("0:Alignment",1,preValue)
                        }
                    }
                }
            }

            TitleSlider {
                id: spacingLayout
                width: parent.width
                Layout.fillWidth: true
                height: 30
                from:-100
                to:100
                stepSize: 1
                value: 100
                decimal: 0
                comp: wordComponent
                ofParamKey: "0:Spacing"
                title: qsTr("Line spacing")
                valueSuffix: ""
                keyframeEnable:false
                onValueChanged: {
                    if(wordComponent !== null && spacingLayout.isUserChanged()){
                        console.log("Slider:spacingLayout Value:"+spacingLayout.value)
                        wordComponent.updateOfParamVariantValueBySilent("0:Spacing",spacingLayout.value)
                    }
                }
                onSliderValueChanged: {
                    if(wordComponent !== null){
                        wordComponent.updateOfParamIntValueByUser("0:Spacing",endValue,beginValue)
                        console.log("Slider:spacingLayout Press: Value:"+beginValue + " -> "+endValue)
                    }
                }
                onKeyFrameCheckedChanged: {
                    if(checked){
                        wordComponent.enableKeyFrame("0:Spacing",0)
                    }else{
                        wordComponent.removeKeyFrame("0:Spacing")
                    }
                }
            }

            TitleSlider {
                id: leadingLayout
                width: parent.width
                Layout.fillWidth: true
                height: 30
                from:-100
                to:100
                stepSize: 1
                value: 100
                decimal: 0
                comp: wordComponent
                ofParamKey: "0:Leading"
                title: qsTr("Column spacing")
                valueSuffix: ""
                keyframeEnable:false
                onValueChanged: {
                    if(wordComponent !== null && leadingLayout.isUserChanged()){
                        console.log("Slider:scaleSeekbar Value:"+leadingLayout.value)
                        wordComponent.updateOfParamVariantValueBySilent("0:Leading",leadingLayout.value)
                    }
                }
                onSliderValueChanged: {
                    if(wordComponent !== null){
                        wordComponent.updateOfParamIntValueByUser("0:Leading",endValue,beginValue)
                        console.log("Slider:leadingLayout Press: Value:"+beginValue + " -> "+endValue)
                    }
                }
                onKeyFrameCheckedChanged: {
                    if(checked){
                        wordComponent.enableKeyFrame("0:Leading",0)
                    }else{
                        wordComponent.removeKeyFrame("0:Leading")
                    }
                }
            }


            ColorSelector {
                id:fontColorColorSelector
                height:30
                width: parent.width
                Layout.fillWidth: true
                transparentLayoutVisible:false
                selectedColor:wordComponent.textColor
                onSelectedColorChanged: {
                    wordComponent.textColor = selectedColor
                    var c = Qt.rgba(wordComponent.backgroundColor.r,wordComponent.backgroundColor.g,wordComponent.backgroundColor.b)
                    console.log("Background Color:"+wordComponent.backgroundColor + " alpha:"+wordComponent.backgroundColor.a + " c:"+c)
                }
                onSelectedColorValueChanged: {
                    wordComponent.setTextColorByUser(beginColor,endColor)
                }
            }

            TitleSlider {
                id: opacityLayout
                width: parent.width
                Layout.fillWidth: true
                height: 30
                from:0
                to:100
                stepSize: 1
                value: 100
                decimal: 0
                comp: wordComponent
                ofParamKey: "0:Opacity"
                title: "不透明度"
                valueSuffix: "%"
                keyframeEnable:true
                onValueChanged: {
                    if(wordComponent !== null && opacityLayout.isUserChanged()){
                        console.log("Slider:scaleSeekbar Value:"+opacityLayout.value)
                        wordComponent.updateOfParamVariantValueBySilent("0:Opacity",opacityLayout.value)
                    }
                }
                onSliderValueChanged: {
                    if(wordComponent !== null){
                        wordComponent.updateOfParamIntValueByUser("0:Opacity",endValue,beginValue)
                        console.log("Slider:opacityLayout Press: Value:"+beginValue + " -> "+endValue)
                    }
                }
                onKeyFrameCheckedChanged: {
                    if(checked){
                        wordComponent.enableKeyFrame("0:Opacity",100)
                    }else{
                        wordComponent.removeKeyFrame("0:Opacity")
                    }
                }
            }

            Rectangle{
                width: parent.width
                Layout.fillWidth: true
                height: 1
                Layout.topMargin: 12
                Layout.bottomMargin: 12
                color:"#2C2C2C"
            }

            EditTransformProperty{
                component: propertyEditController.transformComponent()
                id:simpleTransform
                width: parent.width
                height: 120
                scale_max_value:1000
                scale_decimal: 1
                Layout.fillWidth: true
            }


            Rectangle{
                width: parent.width
                Layout.fillWidth: true
                height: 1
                visible: inputLayout.visible
                Layout.topMargin: 12
                Layout.bottomMargin: 12
                color:"#2C2C2C"
            }

            BasicCheckBox{
                id:stokeCheckBox
                height: 25
                width: 80
                Layout.fillWidth: true
                text: qsTr("Draw edge")
                indicatorWidth:10
                textColor: "#FFFFFF"
                backgroundColor:"#00000000"
                indicatorColor: "#666666"
                checked: wordComponent.isStokeEnable
                onCheckedChanged: {
                    wordComponent.isStokeEnable = checked
                    stokeColorLayout.visible = checked
                    stokeThicknessLayout.visible = checked
                }
            }

            Item{
                id:stokeColorLayout
                width: parent.width
                Layout.fillWidth: true
                height: itemHeight
                visible: false

                Text{
                    id:colorTitle
                    width:itemLabelWidth
                    height: itemHeight
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                    text: qsTr("Color")
                    color:"#FFFFFF"

                }

                Rectangle {
                    width:80
                    height: itemHeight
                    color:"#2C2E32"
                    radius: 2
                    x:itemLabelWidth
                    anchors.top: parent.top
                    anchors.left: colorTitle.right

                    Rectangle{
                        id:selectedColorRectangle
                        width: 60
                        height: 20
                        x:4
                        y:2.5
                        color:wordComponent.stokeColor
                    }



                    ColorDialog {
                        id: colorDialog
                        title: qsTr("Choose Color")
                        color: "#000000"
                        onColorChanged: {
                            wordComponent.stokeColor = Qt.rgba(color.r,color.g,color.b,1)
                            selectedColorRectangle.color = wordComponent.stokeColor
                        }
                    }

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            colorDialog.open();
                        }
                    }

                    Image{
                        source:"qrc:/image/assets/dropdown.png"
                        anchors.right: parent.right
                        width:16
                        height: 16
                        y:4
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.rightMargin: 2
                    }

                }

            }


            Item{
                id:stokeThicknessLayout
                width: parent.width
                Layout.fillWidth: true
                height: itemHeight
                visible: false

                Text{
                    id:shadowThickness
                    text: qsTr("Thickness")
                    color:"#FFFFFF"
                    width:itemLabelWidth
                    height: 25
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                }

                SeekBar{
                    id: stokeThicknessSeekBar
                    width: parent.width - itemLabelWidth
                    x:itemLabelWidth
                    height: 25

                    slider{
                        orientation:Qt.Horizontal
                        from:0
                        to:100
                        stepSize: 1
                        value: wordComponent.stokeThickness

                        onValueChanged: {
                            wordComponent.stokeThickness = stokeThicknessSeekBar.slider.value
                        }
                    }
                    onSliderValueChanged: {
                        wordComponent.setStokeThicknessByUser(beginValue,endValue)
                    }
                }
            }


            Rectangle{
                width: parent.width
                Layout.fillWidth: true
                height: 1
                Layout.topMargin: 12
                Layout.bottomMargin: 12
                color:"#2C2C2C"
            }

            BasicCheckBox{
                id:backgroundCheckBox
                height: 25
                width: 80
                Layout.fillWidth: true
                text: qsTr("Frame")
                indicatorWidth:10
                textColor: "#FFFFFF"
                backgroundColor:"#00000000"
                indicatorColor: "#666666"
                checked: wordComponent.isBackgroundEnabled
                onCheckedChanged: {
                    wordComponent.isBackgroundEnabled = checked
                    backgroundColorSelector.visible = checked

                }
            }

            ColorSelector {
                id:backgroundColorSelector
                height:60
                width: parent.width
                Layout.fillWidth: true
                selectedColor:wordComponent.backgroundColor
                visible: wordComponent.isBackgroundEnabled
                onSelectedColorChanged: {
                    wordComponent.backgroundColor = selectedColor
                }
                onSelectedColorValueChanged: {
                    wordComponent.setBackgroundColorByUser(beginColor,endColor)
                }
            }


            Rectangle{
                width: parent.width
                Layout.fillWidth: true
                height: 1
                Layout.topMargin: 12
                Layout.bottomMargin: 12
                color:"#2C2C2C"
            }

            BasicCheckBox{
                id:shadowCheckBox
                height: 25
                width: 80
                Layout.fillWidth: true
                text: qsTr("Shadow")
                textColor: "#FFFFFF"
                indicatorWidth:10
                backgroundColor:"#00000000"
                indicatorColor: "#666666"
                checked: wordComponent.isShadowEnable
                onCheckedChanged: {
                    wordComponent.isShadowEnable = checked
                    shadowColorSelector.visible = checked
                    shadowDistanceLayout.visible = checked
                    shadowBlurLayout.visible = checked
                    shadowAngleLayout.visible = checked
                }
            }

            ColorSelector {
                id:shadowColorSelector
                height:60
                width: parent.width
                Layout.fillWidth: true
                selectedColor:wordComponent.shadowColor
                visible: false
                onSelectedColorChanged: {
                    wordComponent.shadowColor = selectedColor
                }
                onSelectedColorValueChanged: {
                    wordComponent.setShadowColorByUser(beginColor,endColor)
                }
            }


            Item{
                id:shadowBlurLayout
                width: parent.width
                Layout.fillWidth: true
                height: itemHeight
                visible: false

                Text{
                    id:shadowBlur
                    text: qsTr("Blur")
                    color:"#FFFFFF"
                    width:itemLabelWidth
                    height: 25
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                }

                SeekBar{
                    id: shadowBlurSeekBar
                    width: parent.width - itemLabelWidth
                    x:itemLabelWidth
                    height: 25

                    slider{
                        orientation:Qt.Horizontal
                        from:0
                        to:100
                        stepSize: 1
                        value: wordComponent.shadowBlur

                        onValueChanged: {
                            wordComponent.shadowBlur = shadowBlurSeekBar.slider.value
                        }
                    }
                    onSliderValueChanged: {
                        wordComponent.setShadowBlurByUser(beginValue,endValue)
                    }
                }
            }

            Item{
                id:shadowDistanceLayout
                width: parent.width
                Layout.fillWidth: true
                height: itemHeight
                visible: false

                Text{
                    id:shadowDistance
                    text: qsTr("Distance")
                    color:"#FFFFFF"
                    width:itemLabelWidth
                    height: 25
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                }

                SeekBar{
                    id: shadowDistanceSeekBar
                    height: 25
                    x:itemLabelWidth
                    width: parent.width - itemLabelWidth

                    slider{
                        orientation:Qt.Horizontal
                        from:0
                        to:100
                        stepSize: 1
                        value: wordComponent.shadowDistance

                        onValueChanged: {
                            wordComponent.shadowDistance = shadowDistanceSeekBar.slider.value
                        }

                    }
                    onSliderValueChanged: {
                        wordComponent.setShadowDistanceByUser(beginValue,endValue)
                    }
                }
            }

            Item{
                id:shadowAngleLayout
                width: parent.width
                Layout.fillWidth: true
                height: itemHeight
                visible: false

                Text{
                    id:shadowAngle
                    text: qsTr("Angle")
                    color:"#FFFFFF"
                    width:itemLabelWidth
                    height: 25
                    verticalAlignment: Qt.AlignVCenter
                    horizontalAlignment: Qt.AlignLeft
                }

                SeekBar{
                    id: shadowAngleSeekBar
                    x:itemLabelWidth
                    width: parent.width - itemLabelWidth
                    height: 25

                    slider{
                        orientation:Qt.Horizontal
                        from:-180
                        to:180
                        stepSize: 1
                        value: wordComponent.shadowAngle

                        onValueChanged: {
                            wordComponent.shadowAngle = shadowAngleSeekBar.slider.value
                        }
                    }
                    onSliderValueChanged: {
                        wordComponent.setShadowAngleByUser(beginValue,endValue)
                    }
                }
            }

            Rectangle{
                width: parent.width
                Layout.fillWidth: true
                height: 1
                visible: inputLayout.visible
                Layout.topMargin: 10
                Layout.bottomMargin: 10
                color:"#2C2C2C"
            }


            EditInputProperty{
                component: wordComponent
                id:inputLayout
                width: parent.width
                Layout.fillWidth: true
            }

            //占位
            Rectangle{
                Layout.fillHeight: true
                width: parent.width
                Layout.fillWidth: true
                Layout.topMargin: 10
                color:"#FFFF00"
            }
        }
    }



    Component.onCompleted: {
        console.log(" WordProperty attachComponent :"+wordComponent)
        onComponentLoaded(wordComponent)

    }
}
