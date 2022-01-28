import QtQuick 2.12
import BasicImageDataSource 1.0

Item {
    id: root

    width: parent.width
    height: parent.height

    property alias imgSrc:itemImage.source
    property var animatedImgSrc: ""
    property bool isEntered: false

    BasicImageDataSource {
        id: imageLoader
        onLoadAnimatedImgSuccess: {
            if (animatedImgSrc != null && animatedImgSrc.length > 0) {
                animatedItemImage.visible = isEntered
                itemImage.visible = !isEntered
                animatedItemImage.source = imageLoader.localAnimatedImgSrc()
            }
        }
    }

    Image {
        id: itemImage
        width: root.width
        height: root.height
        mipmap: true
        anchors.centerIn: parent
        visible: true
        fillMode: Image.PreserveAspectCrop
    }

    AnimatedImage {
        id: animatedItemImage
        anchors.fill: itemImage
        visible: false
        onStatusChanged: {
            if (status === Image.Error) {
                console.log("BasicImageDataSource Load Error:" + animatedItemImage.source)
                animatedItemImage.source = ""
                animatedItemImage.visible = false
                itemImage.visible = true
            }
        }
    }

    onIsEnteredChanged: {
        if (animatedImgSrc != null && animatedImgSrc.length > 0 && isEntered) {
            imageLoader.loadAnimatedImg(animatedImgSrc)
        } else if (!isEntered) {
            animatedItemImage.visible = isEntered
            itemImage.visible = !isEntered
        }
    }

    onAnimatedImgSrcChanged: {
        animatedItemImage.source = ""
        animatedItemImage.visible = false
        itemImage.visible = true
        if (animatedImgSrc != null && animatedImgSrc.length > 0 && isEntered) {
            imageLoader.loadAnimatedImg(animatedImgSrc)
        } else if (!isEntered) {
            animatedItemImage.visible = isEntered
            itemImage.visible = !isEntered
        }
    }
}
