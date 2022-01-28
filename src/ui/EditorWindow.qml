import QtQuick 2.12
import QtQuick.Window 2.12
import QtQml 2.12

QtObject {
    signal newProject
    signal openProjFile(var projFile)
    signal updateConfig(var localPath, var save, var title)
    signal showGuide
    signal release(QtObject window)
    signal exitApp
    property var splashWindow: Window {
        modality: Qt.ApplicationModal
        flags: Qt.SplashScreen
        color: "transparent"
        width: 1
        height: 1
        Timer {
            interval: 0
            running: true
            repeat: false
            onTriggered: {
                editorPage.source = "EditorPage.qml"
            }
        }
        Component.onCompleted: visible = true
    }
    property var editorPage: Loader {}
}
