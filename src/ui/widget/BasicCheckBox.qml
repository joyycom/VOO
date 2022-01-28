import QtQuick 2.12
import QtQuick.Templates 2.12 as T
import QtQuick.Controls 2.12
import QtQuick.Controls.impl 2.12

//qtquickcontrols2\src\imports\controls\CheckBox.qml
//from Customizing CheckBox
//2020-12-26 移除Shape，改用impl中的ColorImage加载按钮图标
T.CheckBox {
    id: control

    //可以像源码一样，定义一个全局的样式，然后取全局样式中对应的颜色
    //checked选中状态，down按下状态，hovered悬停状态
    property color textColor: "white"
    property color backgroundTheme: "darkCyan"
    property color backgroundColor: control.down ? Qt.darker(
                                                       backgroundTheme) : (control.hovered
                                                                           || control.highlighted) ? Qt.lighter(backgroundTheme) : control.checked ? backgroundTheme : backgroundTheme
    property int indicatorWidth: 24 //勾选框
    property color indicatorColor: control.textColor
    property int radius: 0

    property bool bak_checked: false


    signal checkedUserChanged(bool checked)

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            implicitContentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(
                        implicitBackgroundHeight + topInset + bottomInset,
                        implicitContentHeight + topPadding + bottomPadding,
                        implicitIndicatorHeight + topPadding + bottomPadding)

    padding: 5
    spacing: 5
    font {
        pixelSize: 12
    }

    function updateChecked(c){
        bak_checked = c;
        checked = c;
    }

    onCheckedChanged: {
        if(bak_checked !== checked){
            bak_checked = checked
            checkedUserChanged(checked)
        }
        if(!checked && focus){
            focus = false
        }
    }

    //勾选框，用贴图更方便
    indicator: Image {
        sourceSize.width: control.indicatorWidth
        sourceSize.height: control.indicatorWidth
        x: control.text ? control.leftPadding : control.leftPadding
                          + (control.availableWidth - width) / 2
        y: (parent.height - height) / 2
        source: control.checkState === Qt.Checked ? "qrc:/image/assets/ic_checkbox_checked.png" : "qrc:/image/assets/ic_checkbox_default.png"
    }

    //勾选框文本
    contentItem: CheckLabel {
        text: control.text
        font: control.font
        color: textColor
        visible: control.text.length > 0
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        renderType: Text.NativeRendering
        elide: Text.ElideRight
        leftPadding: control.text.length > 0?control.indicator.width + control.spacing:0
    }

    background: Rectangle {
        anchors.fill: parent
        radius: control.radius
        color: backgroundColor
    }
}
