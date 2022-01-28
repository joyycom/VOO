import QtQuick 2.12
import QtQuick.Window 2.12
import QtQml 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.10
import ExportDialogDataSource 1.0
import QmlUtils 1.0
import FramelessWindow 1.0
import "../widget"
import "../widget/Mouse.js" as Logic

FramelessWindow {
    id: exportDialog
    modality: Qt.ApplicationModal
    width: 600
    height: titleRect.height + (exportingDialog.visible ? 316 : scrollView.height)
    resizable: false
    onlyAllowListMovable: true
    allowTouchWidget: [titleRect, dialogTitle]
    Component.onCompleted: {
        datasource.updatePackSize()
        datasource.onQmlLoaded()
    }
    Component.onDestruction: {
        datasource.setOutput(baseOutputAddress)
        datasource.setVideoName(nameEdit.text)
        datasource.setCoverType(coverType)
        datasource.setCoverPath(coverPath)
        datasource.setOutputPackCheck(outputPackCheckBox.checked)
        datasource.setOutputVMSCheck(outputVmsCheckBox.checked)
        datasource.setTemplateOverwriteCheck(overwriteCheckBox.checked)
        datasource.setTemplateCnName(templateCnNameEdit.text)
        datasource.setTemplateEnName(templateEnNameEdit.text)
        datasource.setTemplateType(templateTypeCombo.currentIndex)
        datasource.setE_CommerceCfgPath(e_commerceCfgPath)
    }
    property var baseOutputAddress: datasource.getOutput()
    property var e_commerceCfgPath: datasource.getE_CommerceCfgPath()
    property var outputFolder: datasource.optimizeOutputFolder(
                                   baseOutputAddress, datasource.getTitle())
    property var outputAddress: baseOutputAddress + "/" + outputFolder
    property real videoDuration: datasource.getDuration().toFixed(2)
    property int coverType: datasource.getCoverType()
    property var coverPath: datasource.getCoverPath()
    property var previewCoverPath: null
    function showMessage(content) {
        var component = Qt.createComponent("qrc:/ui/dialog/AlertDialog.qml")
        var dialog = component.createObject(editorPage)
        dialog.content = content
        dialog.show()
        return dialog
    }
    function exportPeram(overwritten) {
        if (!datasource.hasClip()) {
            showMessage(qsTr("Currently no video material"))
            return
        }
        var addressText = addressEdit.text
        if (overwritten === 0 && (coverType >= 0 || outputPackCheckBox.checked)
                && fileUtils.exists(addressText)) {
            var dialog = showMessage(
                        qsTr("The export path already exists, do you want to overwrite it?"))
            dialog.button1Text = qsTr("Yea")
            dialog.button2Text = qsTr("Nay")
            dialog.button3Text = qsTr("Cancel")
            dialog.button1Event.connect(function () {
                dialog.close()
                exportPeram(1)
            })
            dialog.button2Event.connect(function () {
                dialog.close()
                outputFolder = datasource.optimizeOutputFolder(addressText,
                                                               outputFolder)
                exportPeram(0)
            })
            dialog.button3Event.connect(function () {
                dialog.close()
            })
            return
        }
        if (!fileUtils.makeDir(addressText)) {
            showMessage(qsTr("The export path is illegal"))
            return
        }
        exportingDialog.savePath = addressText
        exportingDialog.videoName = nameEdit.text
        exportingDialog.coverType = coverType
        exportingDialog.exportTemplate = outputPackCheckBox.checked
        exportingDialog.doContribute = outputVmsCheckBox.checked
        exportingDialog.templateCnName = templateCnNameEdit.text
        exportingDialog.templateEnName = templateEnNameEdit.text
        exportingDialog.templateType = templateTypeCombo.currentIndex
        exportingDialog.templateTeam = templateTeamCombo.currentIndex
        exportingDialog.templateOverwrite = overwriteCheckBox.checked
        exportingDialog.packSize = size.text
        exportingDialog.e_commerceCfgPath = e_commerceCfgPath

        if (exportingDialog.exportTemplate
                && exportingDialog.templateCnName === ""
                && exportingDialog.templateEnName === "") {
            templateTipLabel.visible = true
            templateCnNameEdit.focus = true
            return
        }
        var inputDeviceList = []
        if (exportingDialog.exportTemplate) {
            for (var i = 0, j = 0; i < inputSettingRepeater.count; i++) {
                var item = inputSettingRepeater.itemAt(i)
                if (item.checked()) {
                    inputDeviceList[j++] = item.getJson()
                }
            }
        }
        exportingDialog.inputDeviceList = inputDeviceList
        exportingDialog.visible = true
        exportingDialog.startExport()
    }
    function close() {
        exportDialog.destroy()
    }
    FileUtils {
        id: fileUtils
    }
    ExportDialogDataSource {
        id: datasource
        onPackSizeChanged: {
            size.text = packSize
        }
        onExportProgressChanged: {
            exportingDialog.mode = 0
            exportingDialog.progress = Math.min(100, 100 * progress)
        }
        onContributeProgressChanged: {
            exportingDialog.mode = 1
            exportingDialog.progress = Math.min(100, 100 * progress)
        }
        onExportCancel: {
            close()
        }
        onExportError: {
            exportFailedDialog.visible = true
        }
        onExportFinishChanged: {
            exportSuccessDialog.outputDir = outputDir
            exportSuccessDialog.visible = true

            var exportLogs = getExportLogs()
            if (exportLogs.length > 0) {
                var component = Qt.createComponent(
                            "qrc:/ui/dialog/MessageListDialog.qml")
                if (component.status === Component.Error) {
                    console.error("Error:" + component.errorString())
                    return
                    // or maybe throw
                }
                var dialog = component.createObject(editorPage)

                dialog.setLog(exportLogs)
                dialog.show()
            }
        }
        onUpdateCover: {
            if (coverType === 0) {
                exportDialog.coverPath = coverPath
                crb1.checked = coverPath !== null
            }
            exportDialog.previewCoverPath = coverPath
            exportDialog.coverType = coverType
        }
        onTemplateTypeChanged: {
            templateTypeCombo.model = list
            templateTypeCombo.currentIndex = datasource.getTemplateType()
        }
        onTemplateTeamChanged: {
            templateTeamCombo.model = list
            templateTeamCombo.currentIndex = datasource.getTemplateTeam()
        }
    }
    onCoverTypeChanged: {
        if (coverType > 0) {
            datasource.uploadTimelineCover(coverType)
        }
    }
    Rectangle {
        color: "#15171C"
        radius: 4
        border.color: "#000000"
        border.width: 2
        anchors.fill: parent
        TitleBar {
            id: titleRect
            width: parent.width
            height: 42
            anchors.top: parent.top
            anchors.left: parent.left
            border.color: "#000000"
            border.width: 2
            color: "#000000"
            onCloseClicked: {
                datasource.cancel()
                exportDialog.close()
            }
            Label {
                id: dialogTitle
                text: qsTr("Export")
                font.pixelSize: 12
                color: "#BFBFBF"
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
            }
        }
        Flickable {
            id: scrollView
            width: parent.width
            height: Math.min(exportRect.height, 508)
            anchors.top: titleRect.bottom
            interactive: false
            clip: true
            contentHeight: exportRect.height
            visible: !exportingDialog.visible
            ScrollBar.vertical: ScrollBar {
                width: 8
                minimumSize: 0.1
                policy: ScrollBar.AlwaysOn
                visible: parent.contentHeight > parent.height
            }
            MouseArea {
                anchors.fill: parent
                onWheel: Logic.onMouseWheel(wheel, scrollView)
            }
            Column {
                id: exportRect
                width: parent.width
                bottomPadding: bottomRect.height + 20
                Item {
                    id: basicItem
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 68
                    anchors.rightMargin: 68
                    height: 186
                    Label {
                        id: outputLabel
                        height: 24
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("Export path")
                        font.pixelSize: 12
                        color: "#FFFFFF"
                        anchors.top: parent.top
                        anchors.topMargin: 20
                    }
                    TextField {
                        id: addressEdit
                        implicitWidth: 324
                        implicitHeight: 24
                        anchors.left: outputLabel.right
                        anchors.leftMargin: 20
                        anchors.verticalCenter: outputLabel.verticalCenter
                        color: "#BFBFBF"
                        selectionColor: "#5465FF"
                        selectedTextColor: "#FFFFFF"
                        selectByMouse: true
                        placeholderText: qsTr("Default path")
                        text: fileUtils.toNativeSeparators(outputAddress)
                        leftPadding: 6
                        topPadding: 0
                        bottomPadding: 0
                        background: Rectangle {
                            color: "#2D2F33"
                            radius: 2
                        }
                    }
                    Button {
                        id: addressBtn
                        width: 60
                        height: 24
                        anchors.left: addressEdit.right
                        anchors.leftMargin: 12
                        anchors.verticalCenter: addressEdit.verticalCenter
                        text: qsTr("Browse")
                        onClicked: {
                            fileOutputVideoDialog.open()
                        }
                        FileDialog {
                            id: fileOutputVideoDialog
                            title: qsTr("Select a Saving Path")
                            selectFolder: true
                            folder: shortcuts.desktop
                            onAccepted: {
                                baseOutputAddress = fileUtils.urlToFile(fileUrl)
                            }
                        }
                    }
                    Label {
                        id: nameLabel
                        height: 24
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("Video name")
                        font.pixelSize: 12
                        color: "#FFFFFF"
                        anchors.top: outputLabel.bottom
                        anchors.left: outputLabel.left
                        anchors.topMargin: 20
                    }
                    TextField {
                        id: nameEdit
                        implicitWidth: 396
                        implicitHeight: 24
                        anchors.left: nameLabel.right
                        anchors.leftMargin: 20
                        anchors.verticalCenter: nameLabel.verticalCenter
                        color: "#BFBFBF"
                        selectionColor: "#5465FF"
                        selectedTextColor: "#FFFFFF"
                        selectByMouse: true
                        placeholderText: qsTr("Default name")
                        text: datasource.getVideoName()
                        leftPadding: 6
                        topPadding: 0
                        bottomPadding: 0
                        background: Rectangle {
                            color: "#2D2F33"
                            radius: 2
                        }
                    }
                    Label {
                        id: coverLabel
                        height: 42
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("Cover setting")
                        font.pixelSize: 12
                        color: "#FFFFFF"
                        anchors.top: nameLabel.bottom
                        anchors.left: nameLabel.left
                        anchors.topMargin: 20
                    }
                    RowLayout {
                        id: coverRowLayout
                        anchors.left: coverLabel.right
                        anchors.leftMargin: 20
                        anchors.verticalCenter: coverLabel.verticalCenter
                        spacing: 20
                        BasicRadioButton {
                            id: crb0
                            text: qsTr("Do not export cover")
                            checked: coverType === -1
                            onCheckedChanged: {
                                if (checked) {
                                    coverType = -1
                                }
                            }
                        }
                        BasicRadioButton {
                            id: crb1
                            checked: coverType === 0
                            onCheckedChanged: {
                                if (checked) {
                                    if (coverPath === null || !fileUtils.exists(
                                                coverPath)) {
                                        if (coverType === -1) {
                                            crb0.checked = true
                                        } else if (coverType === 1) {
                                            crb2.checked = true
                                        } else if (coverType === 2) {
                                            crb3.checked = true
                                        } else if (coverType === 3) {
                                            crb4.checked = true
                                        }
                                        coverDialog.open()
                                    } else {
                                        coverType = 0
                                    }
                                }
                            }
                            implicitWidth: 60
                            implicitHeight: 42
                            customContentItem: Item {
                                id: coverShow
                                anchors.left: parent.customIndicator.right
                                anchors.leftMargin: 6
                                Rectangle {
                                    width: 42
                                    height: 42
                                    radius: 4
                                    color: "#3B3E47"
                                    Image {
                                        width: 16
                                        height: 16
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        anchors.top: parent.top
                                        anchors.topMargin: 6
                                        fillMode: Image.PreserveAspectCrop
                                        source: "qrc:/image/assets/export_local_pic_default.png"
                                    }
                                }
                                Image {
                                    id: coverShowImage
                                    width: 42
                                    height: 42
                                    sourceSize.width: width
                                    sourceSize.height: height
                                    fillMode: Image.PreserveAspectCrop
                                    cache: false
                                    source: "file:/" + coverPath
                                }
                                Rectangle {
                                    width: coverShowImage.width
                                    height: 16
                                    anchors.bottom: parent.bottom
                                    color: "#7F000000"
                                    Label {
                                        id: uploadLabel
                                        text: fileUtils.exists(
                                                  coverPath) ? qsTr("Re Upload") : qsTr(
                                                                   "Local Upload")
                                        font.pixelSize: 9
                                        color: "#FFFFFF"
                                        anchors.centerIn: parent
                                    }
                                }
                                Image {
                                    id: uploadHover
                                    width: 12
                                    height: 12
                                    sourceSize.width: width
                                    sourceSize.height: height
                                    source: "qrc:/image/assets/export_upload_hover.png"
                                    visible: false
                                }
                                FileDialog {
                                    id: coverDialog
                                    title: qsTr("Select an Image file")
                                    folder: shortcuts.pictures
                                    nameFilters: ["Image Files (*.bmp *.png *.jpg *.jepg)"]
                                    onAccepted: {
                                        datasource.uploadLocalCover(fileUrl)
                                    }
                                }
                                MouseArea {
                                    id: coverMouse
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onEntered: {
                                        uploadHover.visible = true
                                    }
                                    onExited: {
                                        uploadHover.visible = false
                                    }
                                    onClicked: {
                                        coverDialog.open()
                                    }
                                }
                            }
                        }
                        BasicRadioButton {
                            id: crb2
                            text: qsTr("First Frame")
                            checked: coverType === 1
                            onCheckedChanged: {
                                if (checked) {
                                    coverType = 1
                                }
                            }
                        }
                        BasicRadioButton {
                            id: crb3
                            text: qsTr("Current Frame")
                            checked: coverType === 2
                            onCheckedChanged: {
                                if (checked) {
                                    coverType = 2
                                }
                            }
                        }
                        BasicRadioButton {
                            id: crb4
                            text: qsTr("Last Frame")
                            checked: coverType === 3
                            onCheckedChanged: {
                                if (checked) {
                                    coverType = 3
                                }
                            }
                        }
                    }
                    Label {
                        id: outputSettingLabel
                        text: qsTr("Output settings")
                        font.pixelSize: 12
                        color: "#FFFFFF"
                        anchors.top: coverLabel.bottom
                        anchors.left: coverLabel.left
                        anchors.topMargin: 20
                    }
                    BasicCheckBox {
                        id: outputPackCheckBox
                        height: 16
                        text: qsTr("Export as a template at the same time")
                        indicatorWidth: 10
                        leftPadding: 0
                        anchors.left: outputSettingLabel.right
                        anchors.verticalCenter: outputSettingLabel.verticalCenter
                        anchors.leftMargin: 19
                        textColor: "#FFFFFF"
                        backgroundColor: "transparent"
                        font.pixelSize: 12
                        radius: 1
                        checked: datasource.getOutputPackCheck()
                    }
                    BasicCheckBox {
                        id: outputVmsCheckBox
                        height: 16
                        text: qsTr("Contribute to VMS at the same time")
                        indicatorWidth: 10
                        leftPadding: 0
                        anchors.left: outputPackCheckBox.right
                        anchors.verticalCenter: outputPackCheckBox.verticalCenter
                        anchors.leftMargin: 24
                        textColor: "#FFFFFF"
                        backgroundColor: "transparent"
                        font.pixelSize: 12
                        radius: 1
                        opacity: outputPackCheckBox.checked ? 1 : 0.3
                        enabled: outputPackCheckBox.checked
                        checked: datasource.getOutputVMSCheck()
                        onCheckedChanged: {
                            if (outputVmsCheckBox.checked
                                    && !loginController.isLogin()) {
                                outputVmsCheckBox.checked = false
                                var component = Qt.createComponent(
                                            "qrc:/ui/dialog/LoginDialog.qml")
                                var page = component.createObject(this)
                                page.show()
                            }
                        }
                    }
                }
                Item {
                    height: 20 + basicLine.height
                    anchors.left: basicItem.left
                    anchors.right: basicItem.right
                    visible: outputPackCheckBox.checked
                    Rectangle {
                        id: basicLine
                        width: parent.width
                        height: 1
                        color: "#1AFFFFFF"
                        anchors.bottom: parent.bottom
                    }
                }
                Item {
                    id: inputDeviceItem
                    height: 36
                    anchors.left: basicItem.left
                    anchors.right: basicItem.right
                    visible: outputPackCheckBox.checked
                             && inputSettingRepeater.count > 0
                    Image {
                        id: inputDeviceIcon
                        width: 16
                        height: 16
                        anchors.bottom: parent.bottom
                        source: "qrc:/image/assets/timeline_input_add.png"
                    }
                    Label {
                        id: inputDeviceLabel
                        height: 16
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("Input settings")
                        font.pixelSize: 12
                        color: "#FFFFFF"
                        anchors.bottom: parent.bottom
                        anchors.left: inputDeviceIcon.right
                        anchors.leftMargin: 6
                    }
                }
                Repeater {
                    id: inputSettingRepeater
                    visible: outputPackCheckBox.checked
                    model: datasource.inputDeviceList()
                    Item {
                        height: 24 + 15 //15是间距
                        anchors.left: basicItem.left
                        anchors.right: basicItem.right
                        visible: outputPackCheckBox.checked
                        function getJson() {
                            return inputDeviceEdit.text
                        }
                        function checked() {
                            return inputDeviceCheckBox.checked
                        }
                        TextField {
                            id: inputDeviceEdit
                            height: 24
                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            anchors.leftMargin: 68
                            color: "#FFFFFF"
                            selectionColor: "#5465FF"
                            selectedTextColor: "#FFFFFF"
                            placeholderTextColor: "#747474"
                            placeholderText: qsTr("Extended field")
                            text: datasource.getJsonFromId(modelData)
                            selectByMouse: true
                            leftPadding: 6
                            topPadding: 0
                            bottomPadding: 0
                            background: Rectangle {
                                border.width: 1
                                border.color: inputDeviceEdit.focus ? "#5465FF" : "#1AFFFFFF"
                                color: "transparent"
                                radius: 2
                            }
                        }
                        BasicCheckBox {
                            id: inputDeviceCheckBox
                            height: 16
                            text: datasource.getFileNameFromId(modelData)
                            indicatorWidth: 10
                            leftPadding: 2
                            anchors.left: parent.left
                            anchors.right: inputDeviceEdit.left
                            anchors.verticalCenter: inputDeviceEdit.verticalCenter
                            anchors.rightMargin: 10
                            textColor: "#FFFFFF"
                            backgroundColor: "transparent"
                            font.pixelSize: 12
                            radius: 1
                            checked: true
                        }
                    }
                }
                Item {
                    height: 20 + basicLine2.height
                    anchors.left: basicItem.left
                    anchors.right: basicItem.right
                    visible: outputPackCheckBox.checked
                             && inputDeviceItem.visible
                    Rectangle {
                        id: basicLine2
                        width: parent.width
                        height: 1
                        color: "#1AFFFFFF"
                        anchors.bottom: parent.bottom
                    }
                }
                Item {
                    id: templateItem
                    height: 106
                    anchors.left: basicItem.left
                    anchors.right: basicItem.right
                    visible: outputPackCheckBox.checked
                    Label {
                        id: templateNameLabel
                        height: 24
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("Template name")
                        font.pixelSize: 12
                        color: "#FFFFFF"
                        anchors.top: parent.top
                        anchors.topMargin: 20
                    }
                    TextField {
                        id: templateCnNameEdit
                        implicitWidth: 190
                        implicitHeight: 24
                        anchors.left: templateNameLabel.right
                        anchors.leftMargin: 20
                        anchors.verticalCenter: templateNameLabel.verticalCenter
                        color: "#BFBFBF"
                        selectionColor: "#5465FF"
                        selectedTextColor: "#FFFFFF"
                        selectByMouse: true
                        placeholderText: qsTr("Chinese name")
                        text: datasource.getTemplateCnName()
                        leftPadding: 6
                        topPadding: 0
                        bottomPadding: 0
                        background: Rectangle {
                            color: "#2D2F33"
                            radius: 2
                        }
                        onTextEdited: {
                            templateTipLabel.visible = false
                        }
                    }
                    TextField {
                        id: templateEnNameEdit
                        implicitWidth: 190
                        implicitHeight: 24
                        anchors.left: templateCnNameEdit.right
                        anchors.leftMargin: 20
                        anchors.verticalCenter: templateCnNameEdit.verticalCenter
                        color: "#BFBFBF"
                        selectionColor: "#5465FF"
                        selectedTextColor: "#FFFFFF"
                        selectByMouse: true
                        placeholderText: qsTr("English name")
                        text: datasource.getTemplateEnName()
                        leftPadding: 6
                        topPadding: 0
                        bottomPadding: 0
                        background: Rectangle {
                            color: "#2D2F33"
                            radius: 2
                        }
                        onTextEdited: {
                            templateTipLabel.visible = false
                        }
                    }
                    Label {
                        id: templateTipLabel
                        height: 14
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("*The template name is empty")
                        font.pixelSize: 12
                        color: "#EB291C"
                        anchors.top: templateNameLabel.bottom
                        anchors.topMargin: 3
                        visible: false
                    }
                    Label {
                        id: templateTypeLabel
                        height: 24
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("Template classification")
                        font.pixelSize: 12
                        color: "#FFFFFF"
                        anchors.top: templateNameLabel.bottom
                        anchors.topMargin: 20
                    }
                    BasicComboBox {
                        id: templateTypeCombo
                        height: 24
                        anchors.left: templateTypeLabel.right
                        anchors.right: parent.right
                        anchors.verticalCenter: templateTypeLabel.verticalCenter
                        anchors.leftMargin: 20
                        editable: false
                        backgroundColor: "#2D2F33"
                        itemNormalColor: "#2D2F33"
                        borderColor: "transparent"
                        textColor: "#BFBFBF"
                        font.pixelSize: 12
                        itemHighlightColor: "#2D2F33"
                        itemHeight: 26
                        model: datasource.getTemplateTypeData()
                        currentIndex: datasource.getTemplateType()
                    }
                }
                Item {
                    height: 40
                    anchors.left: basicItem.left
                    anchors.right: basicItem.right
                    visible: outputPackCheckBox.checked
                             && outputVmsCheckBox.checked
                    Label {
                        id: templateTeamLabel
                        height: 24
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("Template Location")
                        font.pixelSize: 12
                        color: "#FFFFFF"
                    }
                    BasicComboBox {
                        id: templateTeamCombo
                        height: 24
                        anchors.left: templateTeamLabel.right
                        anchors.right: parent.right
                        anchors.verticalCenter: templateTeamLabel.verticalCenter
                        anchors.leftMargin: 20
                        editable: false
                        backgroundColor: "#2D2F33"
                        itemNormalColor: "#2D2F33"
                        borderColor: "transparent"
                        textColor: "#BFBFBF"
                        font.pixelSize: 12
                        itemHighlightColor: "#2D2F33"
                        itemHeight: 26
                        model: datasource.getTemplateTeamData()
                        currentIndex: datasource.getTemplateTeam()
                    }
                }
                Item {
                    height: 40
                    anchors.left: basicItem.left
                    anchors.right: basicItem.right
                    visible: outputPackCheckBox.checked
                             && templateTypeCombo.currentIndex === 0
                    Label {
                        id: e_commerceCfgLabel
                        height: 24
                        verticalAlignment: Text.AlignVCenter
                        text: qsTr("E-commerce Config")
                        font.pixelSize: 12
                        color: "#FFFFFF"
                    }
                    TextField {
                        id: e_commerceCfgEdit
                        implicitHeight: 24
                        anchors.left: e_commerceCfgLabel.right
                        anchors.right: e_commerceCfgBtn.left
                        anchors.leftMargin: 20
                        anchors.rightMargin: 12
                        anchors.verticalCenter: e_commerceCfgLabel.verticalCenter
                        color: "#BFBFBF"
                        selectionColor: "#5465FF"
                        selectedTextColor: "#FFFFFF"
                        selectByMouse: true
                        placeholderText: qsTr("Default path")
                        text: fileUtils.toNativeSeparators(e_commerceCfgPath)
                        leftPadding: 6
                        topPadding: 0
                        bottomPadding: 0
                        background: Rectangle {
                            color: "#2D2F33"
                            radius: 2
                        }
                        onTextChanged: {
                            e_commerceCfgPath = text
                        }
                    }
                    Button {
                        id: e_commerceCfgBtn
                        width: 60
                        height: 24
                        anchors.right: parent.right
                        anchors.verticalCenter: e_commerceCfgEdit.verticalCenter
                        text: qsTr("Browse")
                        onClicked: {
                            commerceCfgDialog.open()
                        }
                        FileDialog {
                            id: commerceCfgDialog
                            title: qsTr("Select an e-commerce Config Dir")
                            selectFolder: true
                            folder: shortcuts.desktop
                            onAccepted: {
                                var filePath = fileUtils.urlToFile(fileUrl)
                                if (fileUtils.exists(filePath + "/preview.csv")
                                        && fileUtils.exists(
                                            filePath + "/pod.xlsx")
                                        && fileUtils.exists(
                                            filePath + "/printing.csv")) {
                                    e_commerceCfgPath = filePath
                                } else {
                                    showMessage(qsTr("Dir contains preview.csv and pod.xlsx files"))
                                }
                            }
                        }
                    }
                }
                BasicCheckBox {
                    id: overwriteCheckBox
                    height: 16
                    anchors.left: basicItem.left
                    visible: outputPackCheckBox.checked
                             && outputVmsCheckBox.checked
                    text: qsTr("Synchronization Update")
                    indicatorWidth: 10
                    leftPadding: 0
                    textColor: "#FFFFFF"
                    backgroundColor: "transparent"
                    font.pixelSize: 12
                    radius: 1
                    checked: datasource.getTemplateOverwriteCheck()
                }
            }
        }
        Rectangle {
            id: bottomRect
            width: parent.width - 4
            height: 50
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 2
            color: "#212328"

            Rectangle {
                id: exportRec
                width: 124
                height: 16
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 16
                anchors.leftMargin: 22
                color: "transparent"

                Label {
                    id: timeLbl
                    text: qsTr("Duration: ")
                    font.pixelSize: 12
                    color: "#BFBFBF"
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                }
                Label {
                    id: time
                    text: videoDuration.toString() + "s"
                    font.pixelSize: 12
                    color: "#BFBFBF"
                    anchors.left: timeLbl.right
                    anchors.verticalCenter: parent.verticalCenter
                }

                Rectangle {
                    id: line
                    width: 2
                    height: parent.height - 2
                    y: 2
                    color: "#33ffffff"
                    anchors {
                        left: time.right
                        leftMargin: 5
                    }
                }

                Label {
                    id: sizeLbl
                    text: qsTr("Size: ")
                    font.pixelSize: 12
                    color: "#BFBFBF"
                    anchors.left: line.right
                    anchors.leftMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                }
                Label {
                    id: size
                    font.pixelSize: 12
                    color: "#BFBFBF"
                    anchors.left: sizeLbl.right
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

            Button {
                id: okBtn
                width: 70
                height: 26
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.rightMargin: 22
                anchors.bottomMargin: 11
                text: qsTr("Start")
                style: 1
                onClicked: {
                    exportPeram(0)
                }
            }

            Button {
                id: cancelBtn
                width: 70
                height: 26
                anchors.right: okBtn.left
                anchors.bottom: parent.bottom
                anchors.rightMargin: 10
                anchors.bottomMargin: 11
                text: qsTr("Cancel")
                onClicked: {
                    close()
                }
            }
            Rectangle {
                width: parent.width
                height: 2
                anchors.bottom: parent.bottom
                color: "#000000"
            }
        }
        ExportingDialog {
            id: exportingDialog
            width: parent.width
            anchors.top: titleRect.bottom
            anchors.bottom: parent.bottom
            visible: false
        }

        ExportSuccessDialog {
            id: exportSuccessDialog
            width: parent.width
            anchors.top: titleRect.bottom
            anchors.bottom: parent.bottom
            visible: false
        }
        ExportFailedDialog {
            id: exportFailedDialog
            width: parent.width
            anchors.top: titleRect.bottom
            anchors.bottom: parent.bottom
            visible: false
        }
    }
}
