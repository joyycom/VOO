function clamp(x, minimum, maximum) {
    return Math.min(Math.max(x, minimum), maximum)
}

function scrollMax(scrollView) {
    var maxWidth = Math.max(scrollView.contentWidth - scrollView.width + 14, 0)
    var maxHeight = Math.max(scrollView.contentHeight - scrollView.height + 14,
                             0)
    return Qt.point(maxWidth, maxHeight)
}

function onMouseWheel(wheel, scrollView) {
    // Scroll
    if ((wheel.pixelDelta.x || wheel.pixelDelta.y)
            && wheel.modifiers === Qt.NoModifier) {
        var x = wheel.pixelDelta.x
        var y = wheel.pixelDelta.y
        // Track pads provide both horizontal and vertical.
        if (!y || Math.abs(x) > 2) {
            if (scrollView.contentWidth > scrollView.width) {
                scrollView.contentX = clamp(scrollView.contentX - x, 0,
                                            scrollMax(scrollView).x)
            }
        }
        if (scrollView.contentHeight > scrollView.height) {
            scrollView.contentY = clamp(scrollView.contentY - y, 0,
                                        scrollMax(scrollView).y)
        }
    } else {
        // Vertical only mouse wheel requires modifier for vertical scroll.
        var n
        if (wheel.modifiers === Qt.AltModifier) {
            if (scrollView.contentWidth > scrollView.width) {
                n = Math.round(
                            (Qt.platform.os == "osx" ? wheel.angleDelta.y : wheel.angleDelta.x) / 2)
                scrollView.contentX = clamp(scrollView.contentX - n, 0,
                                            scrollMax(scrollView).x)
            }
        } else {
            if (scrollView.contentHeight > scrollView.height) {
                n = Math.round(wheel.angleDelta.y / 2)
                scrollView.contentY = clamp(scrollView.contentY - n, 0,
                                            scrollMax(scrollView).y)
            }
        }
    }
}
