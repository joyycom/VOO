function onPositionChanged(item, window, mouse, startMouseX, startMouseY, cate) {
    var editorPageXY = window.parent.mapToGlobal(0, 0)
    var xy = item.mapToGlobal(0, 0)
    window.x = xy.x + mouse.x - startMouseX - editorPageXY.x
    window.y = Math.max(0, xy.y + mouse.y - startMouseY - editorPageXY.y)
    var scroolXY = timeLineRec.scrollView.mapToGlobal(-editorPageXY.x,
                                                      -editorPageXY.y)
    if (window.y + startMouseY >= scroolXY.y) {
        window.opacity = 0
        //拖拽特效、滤镜到视频片段有特殊显示
        if (cate === "EFFECT" || cate === "FILTER" || cate === "MORE") {
            var trackIndex = 0
            var clip = timeLineRec.inClipFromXY(
                        window.x + startMouseX + editorPageXY.x,
                        window.y + startMouseY + editorPageXY.y)
            if (clip !== null) {
                var videoClip = clip.isVideoClip() || clip.isImageClip()
                if (videoClip) {
                    timeLineRec.dragToclip = clip
                } else {
                    timeLineRec.dragToclip = null
                }
            } else {
                timeLineRec.dragToclip = null
            }
        }
    } else {
        window.opacity = 1
    }
}

function onReleased(item, window, mouse, type, list) {
    if (typeof (timeLineRec.dragToclip) != "undefined"
            && timeLineRec.dragToclip) {
        console.log("onReleased DragToClip:" + timeLineRec.dragToclip + " Type:"
                    + typeof (timeLineRec.dragToclip) + " " + !timeLineRec.dragToclip)
        datasource.addSubEffectToVideoClip(list,
                                           timeLineRec.dragToclip.trackIndex(),
                                           timeLineRec.dragToclip.itemIndex())
        timeLineRec.dragToclip = null
        return
    }
    timeLineRec.dragToclip = null
    var xy = item.mapToGlobal(0, 0)
    window.x = xy.x
    window.y = Math.max(0, xy.y)
    var scroolXY = timeLineRec.scrollView.mapToGlobal(0, 0)
    if (window.x + mouse.x >= scroolXY.x
            && window.y + mouse.y >= scroolXY.y && window.x
            + mouse.x <= scroolXY.x + timeLineRec.scrollView.width && window.y
            + mouse.y <= scroolXY.y + timeLineRec.scrollView.height) {
        var mouseHeight = window.y + mouse.y - scroolXY.y
                + timeLineRec.scrollView.contentY - timeLineRec.spaceHeight
        var trackIndex = 0
        var itemHeight = 0
        for (; trackIndex < timeLineRec.trackCount; trackIndex++) {
            var child = timeLineRec.tracksContainer.childAt(0, trackIndex)
            if (child) {
                itemHeight += child.height
            }
        }
        var time = (window.x + mouse.x - scroolXY.x + timeLineRec.scrollView.contentX
                    - timeLineRec.spaceWidth) / timeLineRec.timeValue
        if (type === 0) {
            datasource.addLocalVideoResource(list, trackIndex,
                                             Math.max(0, time))
        } else if (type === 1) {
            datasource.addLocalAudioResource(list, trackIndex,
                                             Math.max(0, time))
        } else if (type === 2) {
            datasource.addResourceToTimeline(list, trackIndex,
                                             Math.max(0, time))
        } else if (type === 3) {
            datasource.addLocalMoreResource(list, trackIndex, Math.max(0, time))
        }
        return true
    }
    return false
}
