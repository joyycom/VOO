import QtQuick 2.12
import "../widget"

MouseArea {
    property Item borderRect
    property Item contentRect
    property real videoRatio: 0
    property bool isLeft: false
    property bool isTop: false
    property bool isVertical: true
    property bool isHorizontal: true
    property point clickPos: "0,0"
    property point originalRect: "0,0"
    property int leftSpace: 0
    property int rightSpace: 0
    property int topSpace: 0
    property int bottomSpace: 0
    onPressed: {
        clickPos = mapToGlobal(mouseX, mouseY)
        originalRect = Qt.point(borderRect.width, borderRect.height)
        leftSpace = borderRect.x - contentRect.x
        rightSpace = contentRect.x + contentRect.width - borderRect.width - borderRect.x
        topSpace = borderRect.y - contentRect.y
        bottomSpace = contentRect.y + contentRect.height - borderRect.height - borderRect.y
    }
    onPositionChanged: {
        if (mouse.buttons !== Qt.LeftButton) {
            return
        }
        //保持在最小100
        if (isVertical && videoRatio === 0) {
            if (borderRect.width + 0.0000001 < 100) {
                return
            }
        } else if (isHorizontal && videoRatio === 0) {
            if (borderRect.height + 0.0000001 < 100) {
                return
            }
        } else {
            if (borderRect.width + 0.0000001 < 100
                    || borderRect.height + 0.0000001 < 100) {
                return
            }
        }
        var positionPos = mapToGlobal(mouseX, mouseY)
        var delta = Qt.point(positionPos.x - clickPos.x,
                             positionPos.y - clickPos.y)
        if (isVertical && videoRatio === 0) {
            borderRect.width = Math.max(
                        100,
                        Math.min(contentRect.width - (isLeft ? 0 : leftSpace)
                                 - (isLeft ? rightSpace : 0),
                                 originalRect.x - delta.x * (isLeft ? 1 : -1)))
        }
        if (isHorizontal && videoRatio === 0) {
            borderRect.height = Math.max(
                        100,
                        Math.min(contentRect.height - (isTop ? 0 : topSpace)
                                 - (isTop ? bottomSpace : 0),
                                 originalRect.y - delta.y * (isTop ? 1 : -1)))
        }
        if (videoRatio > 0) {
            if (isHorizontal && !isVertical) {
                var maxHeight = (contentRect.width + Math.min(
                                     leftSpace - rightSpace,
                                     rightSpace - leftSpace)) / videoRatio
                borderRect.height = Math.max(
                            100 / videoRatio, 100, Math.min(
                                maxHeight,
                                contentRect.height - (isTop ? 0 : topSpace)
                                - (isTop ? bottomSpace : 0),
                                originalRect.y - delta.y * (isTop ? 1 : -1)))
                borderRect.width = borderRect.height * videoRatio
            } else {
                var maxWidth
                if (!isHorizontal && isVertical) {
                    maxWidth = (contentRect.height + Math.min(
                                    topSpace - bottomSpace,
                                    bottomSpace - topSpace)) * videoRatio
                } else {
                    maxWidth = (contentRect.height - (isTop ? 0 : topSpace)
                                - (isTop ? bottomSpace : 0)) * videoRatio
                }
                borderRect.width = Math.max(
                            100 * videoRatio, 100, Math.min(
                                maxWidth,
                                contentRect.width - (isLeft ? 0 : leftSpace)
                                - (isLeft ? rightSpace : 0),
                                originalRect.x - delta.x * (isLeft ? 1 : -1)))
                borderRect.height = borderRect.width / videoRatio
            }
        }
        if (isLeft) {
            borderRect.x = contentRect.x + contentRect.width - borderRect.width - rightSpace
        } else {
            if (videoRatio > 0 && !isVertical && isHorizontal) {
                borderRect.x = contentRect.x + leftSpace
                        + (contentRect.width - borderRect.width - leftSpace - rightSpace) / 2
            }
        }
        if (isTop) {
            borderRect.y = contentRect.y + contentRect.height - borderRect.height - bottomSpace
        } else {
            if (videoRatio > 0 && isVertical && !isHorizontal) {
                borderRect.y = contentRect.y + topSpace
                        + (contentRect.height - borderRect.height - topSpace - bottomSpace) / 2
            }
        }
    }
}
