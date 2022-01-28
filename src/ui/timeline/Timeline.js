Qt.include("../widget/Mouse.js")

function selectionContains(trackIndex, clipIndex) {
    var selection = timelineController.selection
    if (selection.x === clipIndex && selection.y === trackIndex)
        return true
    return false
}

function onTimelineMouseWheel(wheel, scrollView) {
    if ((wheel.modifiers === Qt.ControlModifier) && (Qt.platform.os != "osx")) {
        // Zoom
        adjustZoom(wheel.angleDelta.y / 2000)
    } else {
        onMouseWheel(wheel, scrollView)
    }
}

function updateCursor(lastPosition) {
    var distance = cursor.x - lastPosition
    if (scrollView.contentX + distance / 2 < maxSpaceWidth) {
        if (scrollView.contentX >= maxSpaceWidth) {
            scrollView.contentX = Math.max(0,
                                           scrollView.contentX + distance / 2)
        } else {
            scrollView.contentX = Math.max(0,
                                           scrollView.contentX + distance / 2)
        }
    } else {
        scrollView.contentX = Math.max(
                    0, scrollView.contentX + distance - spaceWidth)
    }
}
