import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtQuick.Layouts 1.3
import SimpleTransformComponent 1.0
import "../widget/"
import "../utils/Utils.js" as Utils
Item {
    id: container
    visible: false
    property real tansform_transx: 0
    property real tansform_transy: 0
    property real tansform_scaleX: 1
    property real tansform_scaleY: 1
    property real tansform_rotate: 0
    property real centerX: 0.5
    property real centerY:  0.5
    property real xRatio: 0
    property real yRatio: 0
    property int stickDist: 10
    property int myAngle: tansform_rotate

    SimpleTransformComponent {
        id: simpleComponent
        onUiChange: {
            //int transx, int transy, float scale, float rotate);
            //            rectangleLayout.transform = Qt.matrix4x4(1,         transx,     0,      0,
            //                                                     transy,    1,          0,      0,
            //                                                     0,         0,          1,      0,
            //                                                     0,         0,          0,      1);

            console.log("onUiChange transX:"+transx+" TransY:"+transy+" ScaleX:"+scaleX+" ScaleY:"+scaleY + " container.width:"+container.width+ " container.height:"+container.height)
            tansform_transx = transx
            tansform_transy = transy
            tansform_scaleX = scaleX
            tansform_scaleY = scaleY
        }
        onSourceChange: {

            xRatio = container.width / videoWidth
            yRatio = container.height / videoHeight
            rectangleLayout.width = container.width * width / videoWidth
            rectangleLayout.height = container.height * height / videoHeight

            console.log("onSourceChange1 heightScale:"+height+" widthScale:"+width+ " container.width:"+container.width+ " container.height:"+container.height)

            rectangleLayout.opacity = 1
        }


        onDisplayTransformView: {
            container.visible = ready && inTime
            if (!ready) {
                rectangleLayout.opacity = 0
            }
        }
    }
    Shortcut {
        context: shortcutContext
        sequence: "Alt+Left"
        enabled: container.visible
        onActivated: {
            simpleComponent.changeTranslateX(true)
        }
    }
    Shortcut {
        context: shortcutContext
        sequence: "Alt+Right"
        enabled: container.visible
        onActivated: {
            simpleComponent.changeTranslateX(false)
        }
    }
    Shortcut {
        context: shortcutContext
        sequence: "Alt+Up"
        enabled: container.visible
        onActivated: {
            simpleComponent.changeTranslateY(true)
        }
    }
    Shortcut {
        context: shortcutContext
        sequence: "Alt+Down"
        enabled: container.visible
        onActivated: {
            simpleComponent.changeTranslateY(false)
        }
    }
    Rectangle {
        id: borderRect
        width: rectangleLayout.width * Math.abs(tansform_scaleX)
        height: rectangleLayout.height * Math.abs(tansform_scaleY)
        x: container.width * centerX - width / 2 + tansform_transx * xRatio
        y: container.height * centerY - height / 2 + tansform_transy * yRatio
        border.color: "white"
        border.width: 2
        color: "transparent"
        transform: [
            Rotation {
                axis {
                    x: 0
                    y: 0
                    z: 1
                }
                origin.x: borderRect.width / 2
                origin.y: borderRect.height / 2
                angle: tansform_rotate
            }
        ]
    }
    Item {
        id: rectangleLayout
        width: 100
        height: 100
        smooth: true
        antialiasing: true
        opacity: 0
        x: container.width * centerX - rectangleLayout.width / 2
        y: container.height * centerY - rectangleLayout.height / 2
        transform: [
            Scale {
                origin.x: rectangleLayout.width / 2
                origin.y: rectangleLayout.height / 2
                xScale: tansform_scaleX
                yScale: tansform_scaleY
            },
            Rotation {
                axis {
                    x: 0
                    y: 0
                    z: 1
                }
                origin.x: rectangleLayout.width / 2
                origin.y: rectangleLayout.height / 2
                angle: tansform_rotate
            },
            Translate {
                x: tansform_transx * xRatio
                y: tansform_transy * yRatio
            }
        ]

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            cursorShape: Qt.SizeAllCursor
            property real startx
            property real starty
            property real startTransX
            property real startTransY
            property bool isMove: false
            onPressed: {
                previewController.stop()
                var previewXY = container.mapFromItem(rectangleLayout,
                                                      mouseX, mouseY)
                startx = previewXY.x
                starty = previewXY.y
                startTransX = tansform_transx
                startTransY = tansform_transy
                myAngle = tansform_rotate
                isMove = false
            }
            onPositionChanged: {
                if (mouse.buttons === Qt.LeftButton) {
                    var previewXY = container.mapFromItem(rectangleLayout,
                                                          mouseX, mouseY)
                    var moveX = previewXY.x - startx
                    var moveY = previewXY.y - starty
                    if (!isMove && (Math.abs(moveX) > 1 || Math.abs(
                                        moveY) > 1)) {
                        isMove = true
                    }

                    var xPos = startTransX + moveX / xRatio
                    var yPos = startTransY + moveY / yRatio

                    if (mouse.modifiers & Qt.ShiftModifier) {
                        if(Math.abs(xPos) < Math.abs(yPos)){
                            xPos = 0
                        }else{
                            yPos = 0
                        }
                    }

                    if (Math.abs(xPos) < stickDist) {
                        centerColumnLine.visible = true
                        xPos = 0
                    } else {
                        centerColumnLine.visible = false
                    }
                    if (Math.abs(yPos) < stickDist) {
                        centerRowLine.visible = true
                        yPos = 0
                    } else {
                        centerRowLine.visible = false
                    }

                    if (myAngle == 0 || myAngle == 90 || myAngle == 180
                            || myAngle == 270) {
                        var myHeight = rectangleLayout.height
                        var myWidth = rectangleLayout.width
                        if (myAngle == 90 || myAngle == 270) {
                            myHeight = rectangleLayout.width
                            myWidth = rectangleLayout.height
                        }
                        var top = (container.height - myHeight * tansform_scaleY) / 2 + yPos * yRatio
                        var bottom = container.height - (top + myHeight * tansform_scaleY)
                        if ((Math.abs(top) < stickDist)
                                || (Math.abs(bottom) < stickDist)) {
                            topLine.visible = (Math.abs(
                                                   top) < stickDist) ? true : false
                            bottomLine.visible = (Math.abs(
                                                      bottom) < stickDist) ? true : false
                            yPos = (Math.abs(
                                        top) < stickDist) ? -(container.height - myHeight * tansform_scaleY) / 2 / yRatio : (container.height - myHeight * tansform_scaleY) / 2 / yRatio
                        } else {
                            topLine.visible = false
                            bottomLine.visible = false
                        }

                        var left = (container.width - myWidth * tansform_scaleX) / 2 + xPos * xRatio
                        var right = container.width - (left + myWidth * tansform_scaleX)
                        if ((Math.abs(left) < stickDist)
                                || (Math.abs(right) < stickDist)) {
                            leftLine.visible = (Math.abs(
                                                    left) < stickDist) ? true : false
                            rightLine.visible = (Math.abs(
                                                     right) < stickDist) ? true : false
                            xPos = (Math.abs(
                                        left) < stickDist) ? -(container.width - myWidth * tansform_scaleX) / 2 / xRatio : (container.width - myWidth * tansform_scaleX) / 2 / xRatio
                        } else {
                            leftLine.visible = false
                            rightLine.visible = false
                        }
                    }
                    simpleComponent.changeTranslate(xPos, yPos)
                }
            }
            onReleased: {
                centerColumnLine.visible = false
                centerRowLine.visible = false
                topLine.visible = false
                bottomLine.visible = false
                leftLine.visible = false
                rightLine.visible = false
                if (isMove) {
                    simpleComponent.changeTranslateOnTouchRelease(
                                startTransX, startTransY, tansform_transx,
                                tansform_transy)
                } else {
                    var previewXY = previewLayout.mapFromItem(rectangleLayout,
                                                              mouseX, mouseY)
                    propertyEditController.selectClipFromPreview(
                                previewXY.x, previewXY.y, previewLayout.width,
                                previewLayout.height)
                }
            }
        }

        RetangleScaleEdge{
            id:left
            width: 8
            anchors.left: parent.left
            height: parent.height
            anchors.leftMargin: -4 / tansform_scaleX
            roateRangle: tansform_rotate
            targetHeight: rectangleLayout.height
            targetWidth: rectangleLayout.width
            scaleX: tansform_scaleX
            scaleY: tansform_scaleY
            pointType:"left"
            onScaleXYChanged: {
                simpleComponent.changeScale(scaleX,scaleY)
            }

            onScaleXYChangedOnRelease: {
                simpleComponent.changeScaleOnTouchRelease(startScaleX,scaleX,startScaleY, scaleY)
            }
        }

        RetangleScaleEdge{
            id:top
            height: 8
            anchors.top: parent.top
            width: parent.width
            roateRangle: tansform_rotate
            anchors.topMargin: -4 / tansform_scaleY

            targetHeight: rectangleLayout.height
            targetWidth: rectangleLayout.width
            scaleX: tansform_scaleX
            scaleY: tansform_scaleY
            pointType:"top"
            onScaleXYChanged: {
                simpleComponent.changeScale(scaleX,scaleY)
            }

            onScaleXYChangedOnRelease: {
                simpleComponent.changeScaleOnTouchRelease(startScaleX,scaleX,startScaleY, scaleY)
            }
        }

        RetangleScaleEdge{
            id:right
            width: 8
            anchors.left: parent.right
            anchors.leftMargin: -4 / tansform_scaleX
            height: parent.height
            roateRangle: tansform_rotate
            targetHeight: rectangleLayout.height
            targetWidth: rectangleLayout.width
            scaleX: tansform_scaleX
            scaleY: tansform_scaleY
            pointType:"right"
            onScaleXYChanged: {
                simpleComponent.changeScale(scaleX,scaleY)
            }

            onScaleXYChangedOnRelease: {
                simpleComponent.changeScaleOnTouchRelease(startScaleX,scaleX,startScaleY, scaleY)
            }
        }

        RetangleScaleEdge{
            id:bottom
            height: 8
            anchors.top: parent.bottom
            anchors.topMargin: -4 / tansform_scaleY
            width: parent.width
            roateRangle: tansform_rotate
            targetHeight: rectangleLayout.height
            targetWidth: rectangleLayout.width
            scaleX: tansform_scaleX
            scaleY: tansform_scaleY
            pointType:"bottom"
            onScaleXYChanged: {
                simpleComponent.changeScale(scaleX,scaleY)
            }

            onScaleXYChangedOnRelease: {
                simpleComponent.changeScaleOnTouchRelease(startScaleX,scaleX,startScaleY, scaleY)
            }
        }

        ScaleCornerPoint {
            id: leftTop
            width: 10
            height: 10
            targetHeight: rectangleLayout.height
            targetWidth: rectangleLayout.width
            roateRangle: tansform_rotate
            scaleX: tansform_scaleX
            scaleY: tansform_scaleY
            pointType:"left_top"

            onScaleXYChanged: {
                simpleComponent.changeScale(scaleX,scaleY)
            }

            onScaleXYChangedOnRelease: {
                simpleComponent.changeScaleOnTouchRelease(startScaleX,scaleX,startScaleY, scaleY)
            }
            anchors.left: rectangleLayout.left
            anchors.top: rectangleLayout.top
            anchors.leftMargin: -5 / tansform_scaleX
            anchors.topMargin: -height/2 / tansform_scaleY

        }

        ScaleCornerPoint {
            id: rightTop
            width: 10
            height: 10
            targetHeight: rectangleLayout.height
            targetWidth: rectangleLayout.width
            roateRangle: tansform_rotate
            scaleX: tansform_scaleX
            scaleY: tansform_scaleY
            pointType:"right_top"

            onScaleXYChanged: {
                simpleComponent.changeScale(scaleX,scaleY)
            }

            onScaleXYChangedOnRelease: {
                simpleComponent.changeScaleOnTouchRelease(startScaleX,scaleX,startScaleY, scaleY)
            }
            anchors.leftMargin: -5 / tansform_scaleX
            anchors.left: rectangleLayout.right
            anchors.topMargin: -height/2 / tansform_scaleY
            anchors.top: rectangleLayout.top

        }

        ScaleCornerPoint {
            id: rightBottom
            width: 10
            height: 10
            targetHeight: rectangleLayout.height
            targetWidth: rectangleLayout.width
            scaleX: tansform_scaleX
            scaleY: tansform_scaleY
            roateRangle: tansform_rotate
            pointType:"right_bottom"
            anchors.left: rectangleLayout.right
            anchors.top: rectangleLayout.bottom
            anchors.leftMargin: -5 / tansform_scaleX
            anchors.topMargin: -height/2 / tansform_scaleY
            onScaleXYChanged: {
                simpleComponent.changeScale(scaleX,scaleY)
            }

            onScaleXYChangedOnRelease: {
                simpleComponent.changeScaleOnTouchRelease(startScaleX,scaleX,startScaleY, scaleY)
            }
        }

        ScaleCornerPoint {
            id: leftBottom
            width: 10
            height: 10
            targetHeight: rectangleLayout.height
            targetWidth: rectangleLayout.width
            scaleX: tansform_scaleX
            scaleY: tansform_scaleY
            pointType:"left_bottom"
            roateRangle: tansform_rotate
            anchors.left: rectangleLayout.left
            anchors.top: rectangleLayout.bottom
            anchors.leftMargin: -5 / tansform_scaleX
            anchors.topMargin: -height/2 / tansform_scaleY
            onScaleXYChanged: {
                simpleComponent.changeScale(scaleX,scaleY)
            }

            onScaleXYChangedOnRelease: {
                simpleComponent.changeScaleOnTouchRelease(startScaleX,scaleX,startScaleY, scaleY)
            }
        }

    }


    property color lineColor: "#02E4D6"
    property int lineWidth: 2
    Rectangle {
        id: centerRowLine
        width: container.width
        height: lineWidth
        anchors.verticalCenter: container.verticalCenter
        anchors.left: container.left
        color: lineColor
        visible: false
    }
    Rectangle {
        id: centerColumnLine
        width: lineWidth
        height: container.height
        anchors.horizontalCenter: container.horizontalCenter
        anchors.top: container.top
        color: lineColor
        visible: false
    }
    Rectangle {
        id: topLine
        width: container.width
        height: lineWidth
        anchors.top: container.top
        anchors.left: container.left
        color: lineColor
        visible: false
    }
    Rectangle {
        id: bottomLine
        width: container.width
        height: lineWidth
        anchors.bottom: container.bottom
        anchors.left: container.left
        color: lineColor
        visible: false
    }
    Rectangle {
        id: leftLine
        width: lineWidth
        height: container.height
        anchors.top: container.top
        anchors.left: container.left
        color: lineColor
        visible: false
    }
    Rectangle {
        id: rightLine
        width: lineWidth
        height: container.height
        anchors.top: container.top
        anchors.right: container.right
        color: lineColor
        visible: false
    }

    function updateUI() {
        simpleComponent.updateUI()
    }


    Component.onCompleted: {
        propertyEditController.attachSimpleTransformComponent(simpleComponent)
    }
}
