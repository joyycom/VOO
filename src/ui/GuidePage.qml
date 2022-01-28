import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
import GuideDataSource 1.0
import FramelessWindow 1.0
import "widget/"
import "widget/Mouse.js" as Logic

FramelessWindow {
    id: guidePage
    objectName: "GuidePage"
    width: 820
    height: 584
    resizable: false
    allowTouchWidget: [mainRect, titleRect, guideLogoImage]
    property bool editHistoryState: false
    property var choosedHistory: []
    GuideDataSource {
        id: guidePageDatasource
        onHistoryListChanged: {
            historyListview.model = data
        }
        onShowGuide: {
            show()
        }
        onHideGuide: {
            hide()
        }
        onShowEdit: {

        }
        onExitApp: {
            close()
        }
        onShowToast: {
            toastMgr.show(toast)
        }
    }

    Shortcut {
        sequence: "Ctrl+I"
        enabled: Qt.platform.os === "windows"

        onActivated: {
            console.log("import Proj From AE")
            guidePageDatasource.importProjFromAE()
        }
    }
    Rectangle {
        id: mainRect
        width: parent.width
        height: parent.height
        color: "#15171C"
        border.width: 2
        border.color: "#000000"
        radius: 4
        anchors.centerIn: parent
        TitleBar {
            id: titleRect
            width: parent.width
            height: 40
            minEnabled: true
            onCloseClicked: {
                guidePage.close()
            }
            onMinClicked: {
                showMinimized()
            }
        }
        Image {
            id: guideLogoImage
            width: 56
            height: 22
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 31
            source: "qrc:/image/assets/guide_logo.png"
        }
        Item {
            id: operateRect
            width: newProjectRect.width + openProjectRect.width + openProjectRect.anchors.leftMargin
            height: 120
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 90
            Rectangle {
                id: newProjectRect
                width: 336
                height: parent.height
                color: "#25252B"
                radius: 4
                Image {
                    id: newLogoImage
                    width: 78
                    height: 78
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 30
                    source: "qrc:/image/assets/guide_new_logo.png"
                }
                Item {
                    height: newProjectText.height + newProjectText2.height
                            + newProjectText2.anchors.topMargin
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: newLogoImage.right
                    anchors.leftMargin: 40
                    Text {
                        id: newProjectText
                        text: qsTr("New video")
                        color: "#FFFFFF"
                        font.pixelSize: 18
                    }
                    Text {
                        id: newProjectText2
                        anchors.top: newProjectText.bottom
                        anchors.topMargin: 5
                        text: qsTr("Click to start creating your video")
                        color: "#66FFFFFF"
                        font.pixelSize: 12
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        parent.y = -5
                    }
                    onExited: {
                        parent.y = 0
                    }
                    onClicked: {
                        guidePageDatasource.newProject()
                    }
                }
            }
            Rectangle {
                id: openProjectRect
                width: 336
                height: parent.height
                anchors.left: newProjectRect.right
                anchors.leftMargin: 16
                color: "#25252B"
                radius: 4
                Image {
                    id: openLogoImage
                    width: 78
                    height: 78
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 30
                    source: "qrc:/image/assets/guide_open_logo.png"
                }
                Item {
                    height: openProjectText.height + openProjectText2.height
                            + openProjectText2.anchors.topMargin
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: openLogoImage.right
                    anchors.leftMargin: 40
                    Text {
                        id: openProjectText
                        text: qsTr("Local files")
                        color: "#FFFFFF"
                        font.pixelSize: 18
                    }
                    Text {
                        id: openProjectText2
                        anchors.top: openProjectText.bottom
                        anchors.topMargin: 5
                        text: qsTr("Open the local project file to continue creating")
                        color: "#66FFFFFF"
                        font.pixelSize: 12
                    }
                }
                FileDialog {
                    id: projOpenDialog
                    title: "Select an Proj file"
                    folder: shortcuts.documents
                    nameFilters: ["Project files (*.proj)"]
                    onAccepted: {
                        guidePageDatasource.openProjFile(fileUrl)
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        parent.y = -5
                    }
                    onExited: {
                        parent.y = 0
                    }
                    onClicked: {
                        projOpenDialog.open()
                    }
                }
            }
        }
        Text {
            id: draftBoxText
            anchors.left: operateRect.left
            anchors.top: operateRect.bottom
            anchors.topMargin: 20
            text: qsTr("Draft Box")
            color: "#FFFFFF"
            font.pixelSize: 14
        }
        Text {
            anchors.verticalCenter: draftBoxText.verticalCenter
            anchors.left: draftBoxText.right
            anchors.leftMargin: 6
            text: "(" + historyListview.count + ")"
            color: "#747474"
            font.pixelSize: 14
        }
        RowLayout {
            id: batchOperationLayout
            height: 16
            anchors.right: operateRect.right
            anchors.top: operateRect.bottom
            anchors.topMargin: 22
            visible: !editHistoryState
            Image {
                id: batchOperationImage
                width: 16
                height: 16
                sourceSize.width: width
                sourceSize.height: height
                source: "qrc:/image/assets/guide_multi_state.png"
            }
            Text {
                id: batchOperationText
                Layout.leftMargin: 7
                text: qsTr("Batch Operation")
                color: "#BFBFBF"
                font.pixelSize: 12
            }
        }
        MouseArea {
            anchors.fill: batchOperationLayout
            onClicked: {
                editHistoryState = true
                choosedHistory = []
            }
        }
        RowLayout {
            height: 16
            anchors.right: operateRect.right
            anchors.top: operateRect.bottom
            anchors.topMargin: 22
            visible: editHistoryState
            Text {
                id: selectAllText
                text: qsTr("Select All")
                color: "#BFBFBF"
                font.pixelSize: 12
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        var ct = guidePageDatasource.projectHistoryCount()
                        if (choosedHistory.length === ct) {
                            choosedHistory = []
                        } else {
                            for (var i = 0; i < ct; i++) {
                                choosedHistory.push(i)
                            }
                            choosedHistory = choosedHistory
                        }
                    }
                }
            }
            Rectangle {
                width: 1
                height: 14
                Layout.leftMargin: 10
                color: "#FFFFFF"
                opacity: 0.2
            }
            Text {
                id: deleteText
                Layout.leftMargin: 10
                text: qsTr("Delete")
                color: "#EB291C"
                font.pixelSize: 12
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        guidePageDatasource.removeProject(choosedHistory)
                        choosedHistory = []
                        editHistoryState = false
                    }
                }
            }
            Rectangle {
                width: 1
                height: 14
                Layout.leftMargin: 10
                color: "#FFFFFF"
                opacity: 0.2
            }
            Text {
                id: cancelText
                Layout.leftMargin: 10
                text: qsTr("Cancel")
                color: "#BFBFBF"
                font.pixelSize: 12
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        editHistoryState = false
                    }
                }
            }
        }
        MouseArea {
            anchors.fill: historyListview
            onWheel: Logic.onMouseWheel(wheel, historyListview)
        }
        GridView {
            id: historyListview
            anchors.left: operateRect.left
            anchors.right: operateRect.right
            anchors.top: draftBoxText.bottom
            anchors.topMargin: 21
            anchors.rightMargin: -32
            height: 286
            cellWidth: 90 + 30
            cellHeight: 128 + 30
            clip: true
            model: guidePageDatasource.projectHistorys()
            interactive: false
            ScrollBar.vertical: ScrollBar {
                width: 8
                policy: ScrollBar.AlwaysOn
                visible: parent.contentHeight > parent.height
            }
            delegate: Item {
                width: 90
                height: 128
                Rectangle {
                    width: 90
                    height: 90
                    gradient: Gradient {
                        GradientStop {
                            position: 0.0
                            color: "#3F3B3E47"
                        }
                        GradientStop {
                            position: 1.0
                            color: "#003B3E47"
                        }
                    }
                }
                Image {
                    id: coverImage
                    width: 90
                    height: 90
                    sourceSize.width: width
                    sourceSize.height: height
                    fillMode: Image.PreserveAspectCrop
                    source: "file:/" + cover
                    cache: false
                }
                Image {
                    id: checkImage
                    width: 20
                    height: 20
                    anchors.top: coverImage.top
                    anchors.right: coverImage.right
                    anchors.topMargin: 4
                    anchors.rightMargin: 4
                    visible: editHistoryState
                    source: choosedHistory.includes(
                                index) ? "qrc:/image/assets/guide_check.png" : "qrc:/image/assets/guide_uncheck.png"
                }
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    enabled: !titleEdit.focus
                    onClicked: {
                        if (mouse.button === Qt.LeftButton) {
                            if (editHistoryState) {
                                if (!choosedHistory.includes(index)) {
                                    choosedHistory.push(index)
                                    choosedHistory = choosedHistory
                                } else {
                                    choosedHistory = choosedHistory.filter(
                                                function (ele) {
                                                    return ele !== index
                                                })
                                }
                            } else {
                                historyListview.currentIndex = index
                                guidePageDatasource.openProject(index)
                            }
                        } else if (mouse.button === Qt.RightButton) {
                            var component = Qt.createComponent(
                                        "qrc:/ui/widget/GuideCopyMenu.qml")
                            var page = component.createObject(this)
                            page.projectIndex = index
                            page.x = mouse.x
                            page.y = mouse.y
                            page.open()
                            guidePage.movable = false
                            page.closed.connect(function () {
                                guidePage.movable = true
                            })
                        }
                    }
                }
                TextField {
                    id: titleEdit
                    anchors.top: coverImage.bottom
                    anchors.left: coverImage.left
                    anchors.right: coverImage.right
                    anchors.topMargin: 4
                    width: coverImage.width
                    color: "#FFFFFF"
                    selectionColor: "#5465FF"
                    selectedTextColor: "#FFFFFF"
                    verticalAlignment: TextEdit.AlignVCenter
                    padding: 0
                    text: name
                    selectByMouse: true
                    maximumLength: 100
                    Component.onCompleted: titleEdit.ensureVisible(0)
                    background: Rectangle {
                        border.width: 1
                        border.color: parent.focus ? "#5465FF" : "transparent"
                        color: "transparent"
                        radius: 2
                    }
                    onFocusChanged: {
                        if (focus) {
                            selectAll()
                        }
                    }
                    onEditingFinished: {
                        if (!focus) {
                            guidePageDatasource.renameProject(index, text)
                        }
                    }
                    Keys.onPressed: {
                        switch (event.key) {
                            // @disable-check M20
                        case Qt.Key_Escape:
                            titleEdit.text = name
                        case Qt.Key_Enter:
                        case Qt.Key_Return:
                            focus = false
                            break
                        default:
                            return
                        }
                    }
                }
                Text {
                    anchors.top: titleEdit.bottom
                    anchors.left: titleEdit.left
                    anchors.right: titleEdit.right
                    anchors.topMargin: 2
                    text: duration + " · " + size
                    color: "#747474"
                    font.pixelSize: 12
                }
            }
        }
        ToastManager {
            id: toastMgr
            anchors {
                top: parent.top
                bottom: parent.bottom
                horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
