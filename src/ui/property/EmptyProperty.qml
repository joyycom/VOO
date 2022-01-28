import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import "../widget"

Item {
    height:parent.height
    width: parent.width

    Text {
        id: videoEditPropertyTitle
        width: parent.width
        height: 40
        anchors.top: parent.top
        anchors.topMargin: parent.height / 3
        text: qsTr("The material on the timeline is not currently selected")
        font.pointSize: 18
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: "#999999"
    }
    Component.onCompleted: {
    }
}
