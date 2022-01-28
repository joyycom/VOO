import QtQuick 2.12
import "../basicmenubar"
import QrCodeController 1.0
import QtQuick.Controls 2.12

BasicMenu {
    id: qrcode
    topPadding: 5
    bottomPadding: 5
    background: Rectangle {
        implicitWidth: 150
        implicitHeight: 150
        radius: 2
        color: "#2D2F33" //25% aplha 2D2F33
    }
    QrCodeController {
        id: controller

        onQrcodeLinkReady: {
            qrImage.source = "image://qrcode/" + url + "?width=" + qrImage.width
                    + "&height=" + qrImage.height
            qrImage.visible = true
            rotationAnimation.running = false
        }
        onQrcodeLinkFail: {
            qrImageTips.visible = true
            qrImage.visible = false
            rotationAnimation.running = false
        }
    }
    Rectangle {
        id: container
        width: 140
        height: 140
        color: qrImage.visible ? "white" : "transparent"
        anchors.centerIn: parent
        radius: 2

        Image {
            id: loading_icon
            width: 20
            height: 20
            anchors.centerIn: parent
            visible: !qrImage.visible && !qrImageTips.visible
            source: "qrc:/image/assets/res_downloading.png"
        }
        RotationAnimation {
            id: rotationAnimation
            target: loading_icon
            from: 0
            to: 360
            direction: RotationAnimation.Clockwise
            duration: 500
            loops: Animation.Infinite
        }
        Image {
            id: qrImage
            width: 124
            height: 124
            anchors.centerIn: parent
            cache: false
            visible: false
        }
        Label {
            id: qrImageTips
            anchors.centerIn: parent
            text: qsTr("QR code Generation Failed")
            font.pixelSize: 13
            color: "white"
            visible: false
            wrapMode: Text.WordWrap
        }
    }
    Component.onCompleted: {
        rotationAnimation.running = true
        controller.updateQrcode()
    }
}
