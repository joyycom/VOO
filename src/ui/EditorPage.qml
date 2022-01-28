import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Dialogs 1.3
import EditorPageDataSource 1.0
import PropertyEditController 1.0
import StoreResourcePreviewController 1.0
import LoginController 1.0
import FramelessWindow 1.0
import QtQuick.Controls 1.4 as Controls1_4
import QtQuick.Layouts 1.3

import "./timeline"
import "./store"
import "./basicmenubar"
import "./property"
import "./widget"

ApplicationWindow {
    property bool isShowGuide: true
    property bool aeClose: false
    property var preFocusItem: null
    property var shortcutContext: videoPreviewItem.state
                                  === "window" ? Qt.ApplicationShortcut : Qt.WindowShortcut
    property var proTitle: editorDatasource.currentProjectTitle()
    property var miniPlayerWindow
    id: editorPage
    objectName: "EditorPage"
    width: 1280
    height: 720
    minimumWidth: 1280
    minimumHeight: 720
    color: "#15171C"

    onActiveFocusItemChanged: {
        preFocusItem = activeFocusItem
        if (activeFocusItem === null
                || (activeFocusItem !== timeLineRec
                    && !activeFocusItem.objectName.startsWith("Focus"))) {

        } else {

        }
    }
    EditorPageDataSource {
        id: editorDatasource
        onShowProgressDialog: {
            console.log("onShowProgressDialog111:")
            var component = Qt.createComponent(
                        "qrc:/ui/dialog/ProgressDialog.qml")
            if (component.status === Component.Error) {
                console.error("Error:" + component.errorString())
                return
                // or maybe throw
            }

            var progressDialog = component.createObject(editorPage)
            progressDialog.show()
            console.log("onShowProgressDialog:")
        }
        onVideoPreviewWindowCreated: {
            videoPreviewItem.windowParent = window
            videoPreviewItem.state = "window"
        }
        onVideoPreviewWindowDestroyed: {
            videoPreviewItem.state = "editor"
        }
        onMiniPlayWindowCreated: {
            miniPlayerWindow = window
            miniPlayerWindow.frameScale = videoPreviewItem.previewFrame.frameScale
        }
        onMiniPlayWindowDestroyed: {
            miniPlayerWindow = null
        }
        onCloseEditorPage: {
            editorPage.close()
        }
    }

    Connections {
        target: videoPreviewItem.previewFrame
        onFrameScaleChanged: {
            if (miniPlayerWindow) {
                miniPlayerWindow.frameScale = videoPreviewItem.previewFrame.frameScale
            }
        }
    }
    PropertyEditController {
        id: propertyEditController
    }
    StoreResourcePreviewController {
        id: resourcePreviewController
    }
    LoginController {
        id: loginController
        onLoginSuccess: {
            nicknameLabel.visible = true
        }
        onUpdateNickname: {
            nicknameLabel.text = nickname
        }
        onLogout: {
            nicknameLabel.visible = false
        }
    }
    FileDialog {
        id: projOpenDialog
        title: "Select an Proj file"
        folder: shortcuts.documents
        nameFilters: ["Project files (*.proj)"]
        onAccepted: {
            isShowGuide = false
            editorPage.close()
            openProjFile(fileUrl)
            release(editorPage)
        }
    }
    FileDialog {
        id: saveAsDialog
        title: qsTr("Select a Saving Path")
        folder: shortcuts.desktop
        selectFolder: true
        onAccepted: {
            editorDatasource.saveAs(fileUrl)
        }
    }
    Rectangle {
        id: titleBar
        width: parent.width
        height: 40
        color: "#000000"
        TextField {
            id: titleEdit
            anchors.centerIn: parent
            color: hovered ? "#FFFFFF" : "#BFBFBF"
            selectionColor: "#5465FF"
            selectedTextColor: "#FFFFFF"
            verticalAlignment: TextEdit.AlignVCenter
            text: proTitle
            selectByMouse: true
            maximumLength: 100
            topPadding: 0
            bottomPadding: 0
            background: Rectangle {
                border.width: 1
                border.color: titleEdit.focus ? "#5465FF" : "transparent"
                color: "transparent"
                radius: 2
            }
            onEditingFinished: {
                if (!focus) {
                    proTitle = text
                    editorDatasource.setTitle(proTitle)
                }
            }
            Keys.onPressed: {
                switch (event.key) {
                    // @disable-check M20
                case Qt.Key_Escape:
                    titleEdit.text = proTitle
                case Qt.Key_Enter:
                case Qt.Key_Return:
                    focus = false
                    break
                default:
                    return
                }
            }
        }
        Button {
            id: outputButton
            width: 70
            height: 26
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            anchors.rightMargin: 10
            text: qsTr("Export")
            style: 1
            onClicked: {
                vooExport()
            }
        }
        Image {
            id: nicknameIcon
            width: 16
            height: 16
            source: "qrc:/image/assets/login_nickname_icon.png"
            anchors.right: outputButton.left
            anchors.verticalCenter: outputButton.verticalCenter
            anchors.rightMargin: 10
            visible: nicknameLabel.visible
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var component = Qt.createComponent(
                                "qrc:/ui/dialog/UserInfoDialog.qml")
                    var page = component.createObject(this)
                    var xy = mapFromItem(nicknameIcon, 0, 0)
                    page.x = xy.x + nicknameIcon.width - page.width
                    page.y = xy.y + nicknameIcon.height + 5
                    page.open()
                }
            }
        }
        Label {
            id: nicknameLabel
            anchors.right: nicknameIcon.left
            anchors.verticalCenter: outputButton.verticalCenter
            anchors.rightMargin: 2
            font.pixelSize: 12
            color: "#BFBFBF"
            text: loginController.getNickname()
            visible: loginController.isLogin()
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var component = Qt.createComponent(
                                "qrc:/ui/dialog/UserInfoDialog.qml")
                    var page = component.createObject(this)
                    var xy = mapFromItem(nicknameIcon, 0, 0)
                    page.x = xy.x + nicknameIcon.width - page.width
                    page.y = xy.y + nicknameIcon.height + 5
                    page.open()
                }
            }
        }
        Image {
            id: qrCodeIcon
            width: 15
            height: 15
            source: "qrc:/image/assets/qrcode_icon.png"
            anchors.right: nicknameLabel.visible ? nicknameLabel.left : loginBtn.left
            anchors.verticalCenter: outputButton.verticalCenter
            anchors.rightMargin: 10
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var component = Qt.createComponent(
                                "qrc:/ui/dialog/QRCode.qml")
                    var page = component.createObject(this)
                    var xy = mapFromItem(qrCodeIcon, 0, 0)
                    page.x = xy.x + qrCodeIcon.width - page.width
                    page.y = xy.y + qrCodeIcon.height + 5
                    page.open()
                }
            }
        }
        Button {
            id: loginBtn
            width: 70
            height: 26
            anchors.right: outputButton.left
            anchors.verticalCenter: outputButton.verticalCenter
            anchors.rightMargin: 10
            visible: !nicknameLabel.visible
            text: qsTr("Login")
            style: 2
            onClicked: {
                var component = Qt.createComponent(
                            "qrc:/ui/dialog/LoginDialog.qml")
                var page = component.createObject(this)
                page.show()
            }
        }
    }

    BasicMenuBar {
        id: menuBar
        backgroundColor: "#000000"
        anchors.top: titleBar.top
        BasicMenu {
            id: fileMenu
            title: qsTr("File")
            BasicMenuItem {
                text: qsTr("New")
                shortcutText: Qt.platform.os === "osx" ? "⌘+N" : "Ctrl+N"
                Action {
                    shortcut: StandardKey.New
                    onTriggered: {
                        isShowGuide = false
                        editorPage.close()
                        newProject()
                        release(editorPage)
                    }
                }
                onTriggered: {
                    isShowGuide = false
                    editorPage.close()
                    newProject()
                    release(editorPage)
                }
            }
            BasicMenuItem {
                text: qsTr("Open...")
                shortcutText: Qt.platform.os === "osx" ? "⌘+O" : "Ctrl+O"
                Action {
                    shortcut: StandardKey.Open
                    onTriggered: {
                        projOpenDialog.open()
                    }
                }
                onTriggered: {
                    projOpenDialog.open()
                }
            }
            BasicMenuItem {
                text: qsTr("Import From AE")
                shortcutText: "Ctrl+I"
                Component.onCompleted: {
                    if (Qt.platform.os !== "windows") {
                        height = 0
                        visible = false
                    }
                }
                Action {
                    shortcut: "Ctrl+I"
                    enabled: Qt.platform.os === "windows"
                    onTriggered: {
                        aeClose = true
                        editorDatasource.importFromAE()
                    }
                }
                onTriggered: {
                    aeClose = true
                    editorDatasource.importFromAE()
                }
            }
            BasicMenuSeparator {}
            BasicMenuItem {
                text: qsTr("Save")
                shortcutText: Qt.platform.os === "osx" ? "⌘+S" : "Ctrl+S"
                Action {
                    shortcut: StandardKey.Save
                    onTriggered: {
                        editorDatasource.save()
                    }
                }
                onTriggered: {
                    editorDatasource.save()
                }
            }
            BasicMenuItem {
                text: qsTr("Save as...")
                shortcutText: Qt.platform.os === "osx" ? "⌘+⇧+S" : "Ctrl+Alt+S"
                Action {
                    shortcut: StandardKey.SaveAs
                    onTriggered: {
                        saveAsDialog.open()
                    }
                }
                onTriggered: {
                    saveAsDialog.open()
                }
            }
            BasicMenuItem {
                text: qsTr("Export")
                shortcutText: Qt.platform.os === "osx" ? "⌘+E" : "Ctrl+E"
                Action {
                    shortcut: "Ctrl+E"
                    onTriggered: {
                        vooExport()
                    }
                }
                onTriggered: {
                    vooExport()
                }
            }
            BasicMenuSeparator {}
            BasicMenuItem {
                text: qsTr("Quit")
                shortcutText: Qt.platform.os === "osx" ? "⌘+Q" : "Ctrl+Q"
                Action {
                    shortcut: "Ctrl+Q"
                    onTriggered: {
                        editorPage.close()
                    }
                }
                onTriggered: {
                    editorPage.close()
                }
            }
        }
        BasicMenu {
            id: editMenu
            title: qsTr("Edit")
            BasicMenuItem {
                text: qsTr("Undo")
                shortcutText: Qt.platform.os === "osx" ? "⌘+Z" : "Ctrl+Z"
                Action {
                    shortcut: StandardKey.Undo
                    onTriggered: {
                        editorDatasource.undo()
                    }
                }
                onTriggered: {
                    editorDatasource.undo()
                }
            }
            BasicMenuItem {
                text: qsTr("Redo")
                shortcutText: Qt.platform.os === "osx" ? "⌘+⇧+Z" : "Ctrl+Shift+Z"
                Action {
                    shortcut: StandardKey.Redo
                    onTriggered: {
                        editorDatasource.redo()
                    }
                }
                onTriggered: {
                    editorDatasource.redo()
                }
            }
        }
        BasicMenu {
            id: helpMenu
            title: qsTr("Help")
            Action {
                text: qsTr("About...")
                onTriggered: {
                    var component = Qt.createComponent(
                                "qrc:/ui/dialog/AboutDialog.qml")
                    var page = component.createObject(editorPage, {
                                                          "appVersion": editorDatasource.appVersion(
                                                                            )
                                                      })
                    page.show()
                }
            }
            Action {
                text: qsTr("Feedback...")
                onTriggered: {
                    var component = Qt.createComponent("qrc:/ui/Feedback.qml")
                    var feedback = component.createObject(this)
                    feedback.show()
                }
            }
            Action {
                text: qsTr("License")
                onTriggered: {
                    editorDatasource.openThirdLicense()
                }
            }
            Action {
                text: qsTr("Check for Updates...")
                onTriggered: {
                    var component = Qt.createComponent(
                                "qrc:/ui/dialog/CheckUpdate.qml")
                    var page = component.createObject(this)
                    page.show()
                }
            }
        }
        BasicMenuBarItem {
            id: settingMenu
            text: qsTr("Set Up")
            onTriggered: {
                var component = Qt.createComponent(
                            "qrc:/ui/dialog/SettingDialog.qml")
                var page = component.createObject(editorPage)
                page.show()
            }
        }
    }

    Controls1_4.SplitView {
        width: parent.width
        height: parent.height - menuBar.y - menuBar.height
        orientation: Qt.Vertical
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: titleBar.bottom
        anchors.bottom: parent.bottom
        anchors.leftMargin: 0
        anchors.topMargin: 0
        handleDelegate: Rectangle {
            width: 2
            height: 2
            color: "black"
        }
        Controls1_4.SplitView {
            width: parent.width
            height: parent.height / 2
            Layout.maximumHeight: 0.8 * parent.height
            Layout.minimumHeight: 250
            Layout.fillHeight: true
            orientation: Qt.Horizontal
            handleDelegate: Rectangle {
                width: 2
                height: 2
                color: "black"
            }

            Store {
                id: storeRect
                width: 540
                height: parent.height
                Layout.minimumWidth: 470
            }
            Item {
                id: previewRec
                Layout.minimumWidth: 400
                Layout.fillWidth: true
                clip: true
                property alias window: editorPage
                Item {
                    height: recoverText.height + recoverText.height + recoverText.anchors.topMargin
                    anchors.centerIn: parent
                    visible: videoPreviewItem.state === "window"
                    Text {
                        id: recoverText
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Click to collapse to the original window")
                        color: "#747474"
                        font.pixelSize: 18
                    }
                    Button {
                        id: recoverButton
                        width: 70
                        height: 26
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.top: recoverText.bottom
                        anchors.topMargin: 15
                        text: qsTr("Put away")
                        onClicked: {
                            if (videoPreviewItem.windowParent !== null) {
                                videoPreviewItem.windowParent.close()
                            }
                        }
                    }
                }
                VideoPreviewItem {
                    id: videoPreviewItem
                    editorParent: previewRec
                    windowPreviewParent: previewRec
                }
            }
            EditProperty {
                id: editProperty
                Layout.minimumWidth: 350
            }
        }
        Timeline {
            id: timeLineRec
            height: parent.height / 2
            Layout.minimumHeight: 200
        }
    }
    Item {
        property var item
        property alias image: image
        id: storeDragWindow
        opacity: 0
        x: 100000000
        y: 100000000
        onItemChanged: {
            if (item) {
                item.saving = true
                width = item.width
                height = item.height
                item.grabToImage(function (result) {
                    item.saving = false
                    image.source = result.url
                    image.opacity = 1
                })
            }
        }
        onOpacityChanged: {
            if (opacity == 0) {
                x = 100000000
                y = 100000000
            }
        }
        Image {
            id: image
            width: parent.width
            height: parent.height
        }
    }
    Rectangle {
        id: windowBorder
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin: -2
        color: "transparent"
        border.color: "#000000"
        border.width: 2
    }
    MouseArea {
        anchors.fill: windowBorder
        z: -999
        onClicked: {
            timeLineRec.focus = true
        }
    }
    Timer {
        id: timer
        interval: 0
        running: false
        repeat: false
        onTriggered: {
            showMaximized()
            splashWindow.close()
            editorPage.requestActivate()
            opacity = 1
        }
    }

    Component.onCompleted: {
        opacity = 0
        visible = true
        timer.start()
        editorDatasource.onLoadCompleted()
    }
    function vooExport() {
        videoPreviewItem.previewController.stop()
        var component = Qt.createComponent("qrc:/ui/dialog/ExportDialog.qml")
        if (component.status === Component.Error) {
            console.error("Error:" + component.errorString())
            return
            // or maybe throw
        }
        var dialog = component.createObject(editorPage)
        dialog.show()
    }
    function closeEditor(hasClip) {
        if (miniPlayerWindow) {
            miniPlayerWindow.close()
        }
        //焦点放到时间轴
        timeLineRec.focus = true
        //关闭独立预览窗口
        if (videoPreviewItem.windowParent !== null) {
            videoPreviewItem.windowParent.close()
        }
        videoPreviewItem.preview.stop()
        editorDatasource.save(true)
        updateConfig(editorDatasource.localPath(), hasClip, proTitle)
        if (isShowGuide) {
            showGuide()
            release(editorPage)
        }
    }
    onClosing: {
        var hasClip = editorDatasource.hasClip()
        var isTempProject = editorDatasource.isTempProject()
        if (!aeClose && !hasClip && !isTempProject) {
            close.accepted = false
            var component = Qt.createComponent("qrc:/ui/dialog/AlertDialog.qml")
            var dialog = component.createObject(editorPage)
            dialog.content = qsTr(
                        "Currently no video material, do you want to save it?")
            dialog.show()
            dialog.button1Text = qsTr("Yea")
            dialog.button2Text = qsTr("Nay")
            dialog.button3Text = qsTr("Cancel")
            dialog.button1Event.connect(function () {
                dialog.close()
                closeEditor(true)
            })
            dialog.button2Event.connect(function () {
                dialog.close()
                closeEditor(false)
            })
            dialog.button3Event.connect(function () {
                dialog.close()
            })
            return
        }
        closeEditor(hasClip)
    }
    //有争议的热键，统一放到这里来分发，没争议的自由使用
    Shortcut {
        context: shortcutContext
        sequences: ["Backspace", StandardKey.Delete]
        onActivated: {
            //时间轴删除选中片段
            timeLineRec.hotkeyDelete()
            //素材区删除选中素材
            storeRect.hotkeyDelete()
        }
    }
    Shortcut {
        context: shortcutContext
        sequence: StandardKey.SelectAll
        onActivated: {
            //素材区全选
            storeRect.hotkeySelectAll()
        }
    }
}
