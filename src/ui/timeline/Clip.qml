import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml 2.12
import QtGraphicalEffects 1.0
import AsyncProcessModel 1.0

Item {
    id: clipRoot
    property alias clip: clip
    property bool selected: false
    property bool transitionSelected: false
    property bool dragging: false
    property bool moving: false
    //最小1个像素，不然缩放最小后看不到
    property int minWidth: 1
    property int borderWidth: 12

    property real clipStartTime: startTime + (selected
                                              || moving ? 0 : Math.max(
                                                              0,
                                                              transitonDuring / 2))
    property real blankWidth: (clipStartTime - preEndTime) * timeValue
    property real clipWidth: Math.min((endTime - clipStartTime) * timeValue,
                                      scrollView.width)
    property real clipContentX: (cropStartTime / avgSpeed + (selected
                                                             || moving ? 0 : Math.max(
                                                                             0,
                                                                             transitonDuring / 2)))
                                * timeValue + pausePosition
    property real pausePosition: {
        if (isVideoClip() || isImageClip() || isAudioClip()) {
            return Math.max(0, scrollView.contentX - blankWidth - Math.max(
                                -blankWidth, clipRoot.x) - spaceWidth)
        } else {
            return 0
        }
    }

    property real leftX: blankWidth - borderWidth + pausePosition

    property real rightX: {
        var returnValue
        if (isVideoClip() || isImageClip() || isAudioClip()) {
            returnValue = Math.min(width,
                                   blankWidth + clipWidth + pausePosition)
        } else {
            returnValue = width
        }
        if (returnValue + borderWidth - blankWidth - pausePosition < minWidth) {
            return minWidth + blankWidth + pausePosition - borderWidth
        } else {
            return returnValue
        }
    }

    property real transitionDrogOffset: 0
    signal dragged(var clip, var x, var leftMargin)
    signal clicked(var clip, var isTransiton)
    signal released
    width: Math.max(minWidth, (endTime - preEndTime) * timeValue)

    function itemIndex() {
        return index
    }

    function trackIndex() {
        return trackRoot.itemIndex()
    }

    function getClipContentX() {
        if (clip.item.contentX) {
            return clip.item.contentX
        } else {
            return 0
        }
    }

    //    VideoResource = 0,
    //    AudioResource = 1,
    //    ImageResource = 2,
    //    EffectResource = 3,
    //    StickerResource = 4,
    //    TextResource = 5,
    //    TransitionResource = 6,]
    //    FilterResource = 7
    function isVideoClip() {
        return resourceType == 0
    }
    function isAudioClip() {
        return resourceType == 1
    }
    function isImageClip() {
        return resourceType == 2
    }
    function isEffectClip() {
        return resourceType == 3
    }
    function isStickerClip() {
        return resourceType == 4
    }
    function isTextClip() {
        return resourceType == 5
    }
    function isTransitionClip() {
        return resourceType == 6
    }
    function isFilterClip() {
        return resourceType == 7
    }
    function isAiClip() {
        return resourceType == 8
    }
    function inClipFromXY(x, y) {
        var clipXY = clip.mapToGlobal(0, 0)
        if (x >= clipXY.x && x <= clipXY.x + clip.width) {
            return clipRoot
        }
        return null
    }
    function isEffectClip2() {
        return resourceType != 0 && resourceType != 1 && resourceType != 2
                && resourceType != 3
    }

    Shortcut {
        context: shortcutContext
        sequence: "Ctrl+Left"
        enabled: selected
        onActivated: {
            clipMoveTimer.backwards = true
            clipMoveTimer.start()
        }
    }
    Shortcut {
        context: shortcutContext
        sequence: "Ctrl+Right"
        enabled: selected
        onActivated: {
            clipMoveTimer.backwards = false
            clipMoveTimer.start()
        }
    }
    //左右导航键控制片段移动
    Timer {
        id: redoTimer
        interval: 500
        running: false
        repeat: false
        property real preStartTime: -1
        property real startTime
        onTriggered: {
            timelineController.moveClip(startTime, trackIndex(), false, true,
                                        preStartTime)
            preStartTime = -1
        }
    }
    Timer {
        id: clipMoveTimer
        interval: 0
        repeat: false
        triggeredOnStart: true
        property bool backwards
        onTriggered: {
            //精度丢失，需加一点容差来配合计算
            var tolerance = 0.0000001
            var step = timeValue / fps
            var curX = clipStartTime * timeValue
            var curIndex = Math.floor(curX / rulerWidth + tolerance)
            //下面的目的是要左右滑动时，如果符合条件，优先落在刻度上
            var newStep
            if (backwards) {
                var nextStep = rulerWidth * curIndex
                if (Math.abs(curX - nextStep) > tolerance
                        && curX - step < nextStep) {
                    newStep = nextStep
                } else {
                    newStep = curX - step
                }
            } else {
                var lastStep = rulerWidth * (curIndex + 1)
                newStep = Math.min(lastStep, curX + step)
            }
            if (newStep < 0) {
                newStep = 0
            }
            var startTime = newStep / timeValue
            var preStartTime = timelineController.moveClip(startTime,
                                                           trackIndex(),
                                                           false, false)
            if (redoTimer.preStartTime < 0) {
                redoTimer.preStartTime = preStartTime
            }
            redoTimer.startTime = startTime
            redoTimer.restart()
        }
    }
    MouseArea {
        id: mouseArea
        anchors.fill: clip
        acceptedButtons: Qt.LeftButton
        drag.target: parent
        drag.axis: Drag.XAndYAxis
        propagateComposedEvents: true
        property var lastItemX
        property var clipEdgeList
        property var clipPressedTime: 0
        onPressed: {
            clipPressedTime = new Date().getTime()
            movingIndex = index
            moving = true
            clipEdgeList = timelineController.getClipEdgeList(
                        trackRoot.itemIndex(), index)
            lastItemX = clipRoot.x
        }
        onPositionChanged: {
            //拖拽特效、滤镜片段到视频片段上
            if (isEffectClip() || isFilterClip()) {
                var xy = mapToGlobal(mouseX, mouseY)
                var clip = timeRoot.inClipFromXY(xy.x, xy.y)
                if (clip !== null) {
                    var videoClip = clip.isVideoClip() || clip.isImageClip()
                    if (videoClip) {
                        clipRoot.opacity = 0.8
                        timeRoot.dragToclip = clip
                    } else {
                        clipRoot.opacity = 1
                        timeRoot.dragToclip = null
                    }
                } else {
                    clipRoot.opacity = 1
                    timeRoot.dragToclip = null
                }
            }

            var mouseHeight = mapToGlobal(mouse.x, mouse.y).y
            var trackIndex = 0
            var tempClipDrogNewIndex = -1
            var tempTrackMovingIndex = -1
            for (; trackIndex < trackDelegateModel.count + 1; trackIndex++) {
                var track = tracksGuideRepeater.itemAt(trackIndex)
                if (track.height === 0) {
                    continue
                }
                var itemHeight = track.mapToGlobal(0, 0).y
                var itemSpace = 4
                if ((trackIndex === 0 && mouseHeight <= itemHeight)
                        || (mouseHeight >= itemHeight - itemSpace
                            && mouseHeight <= itemHeight + trackSpaceHeight + itemSpace)) {
                    tempClipDrogNewIndex = trackIndex
                    tempTrackMovingIndex = -1
                } else {
                    var nextItemHeight = itemHeight + trackHeight + trackSpaceHeight
                    if (mouseHeight > itemHeight + trackSpaceHeight
                            && mouseHeight < nextItemHeight
                            && trackType === timelineController.getTypeFromIndex(
                                trackIndex)) {
                        tempTrackMovingIndex = trackIndex
                        tempClipDrogNewIndex = -1
                    }
                }
            }
            clipDrogNewIndex = tempClipDrogNewIndex
            trackMovingIndex = tempTrackMovingIndex
            //磁吸，需要将选中片段取消掉
            timelineController.selection = [-1, -1, false]
            var cursorEdge = timelineController.cursorTime * timeValue
            if (Math.abs(clipRoot.x + blankWidth - cursorEdge) <= 10) {
                //左边缘吸光标
                clipRoot.x = cursorEdge - blankWidth
            } else if (Math.abs(
                           clipRoot.x + clipRoot.width - cursorEdge) <= 10) {
                //右边缘吸附光标
                clipRoot.x = cursorEdge - clipRoot.width
            } else {
                for (var clipIndex in clipEdgeList) {
                    var clipEdge = clipEdgeList[clipIndex] * timeValue
                    if (Math.abs(clipRoot.x + blankWidth - clipEdge) <= 10) {
                        //左边缘吸附
                        clipRoot.x = clipEdge - blankWidth
                        guideCursor.visible = true
                        guideCursor.x = clipRoot.x - scrollView.contentX + blankWidth + spaceWidth
                        break
                    } else if (Math.abs(
                                   clipRoot.x + clipRoot.width - clipEdge) <= 10) {
                        //右边缘吸附
                        clipRoot.x = clipEdge - clipRoot.width
                        guideCursor.visible = true
                        guideCursor.x = clipRoot.x + clipRoot.width
                                - scrollView.contentX + spaceWidth
                        break
                    } else {
                        guideCursor.visible = false
                    }
                }
            }
            clipRoot.x = Math.max(-(leftDrog.x + borderWidth - pausePosition),
                                  clipRoot.x)
            lastItemX = clipRoot.x
            xy = mapToGlobal(mouse.x, mouse.y)
            var scroll = scrollView.mapToGlobal(0, 0)
            parent.dragged(clipRoot, xy.x - scroll.x,
                           leftDrog.x + borderWidth - pausePosition)
        }
        onReleased: {
            //恢复拖拽素材到视频片段上的显示效果
            if (timeRoot.dragToclip !== null) {
                clipRoot.opacity = 1
                timeRoot.dragToclip = null
            }
            var during = new Date().getTime() - clipPressedTime
            movingIndex = -1
            trackMovingIndex = -1
            moving = false
            guideCursor.visible = false
            parent.released()
            clipRoot.clicked(clipRoot, false)
            var trackIndex = 0
            var moveClip = Math.max(0, (lastItemX + blankWidth)) / timeValue
            if (clipDrogNewIndex >= 0) {
                trackIndex = clipDrogNewIndex
                clipDrogNewIndex = -1
                console.log("timeline=>moveClip add new,trackIndex:"
                            + trackIndex + ",moveClip:" + moveClip)
                timelineController.moveClip(moveClip, trackIndex, true)
            } else {
                if (during > 200) {
                    var mouseHeight = mapToGlobal(mouse.x, mouse.y).y
                    for (; trackIndex < trackDelegateModel.count + 1; trackIndex++) {
                        var track = tracksGuideRepeater.itemAt(trackIndex)
                        if (track.height === 0) {
                            continue
                        }
                        var itemHeight = track.mapToGlobal(0, 0).y
                        if ((mouseHeight - itemHeight) < track.height) {
                            break
                        }
                    }
                    if (trackIndex > trackDelegateModel.count) {
                        trackIndex = trackDelegateModel.count
                    }
                    console.log("timeline=>moveClip,trackIndex:" + trackIndex
                                + ",moveClip:" + moveClip + " Spend:" + during
                                + " Count:" + trackDelegateModel.count)
                    timelineController.moveClip(
                                moveClip, trackIndex,
                                trackIndex === trackDelegateModel.count)
                } else {
                    console.log("timeline=>moveClip,is Click , Skip Move Clip:" + during)
                }
            }
            clipRoot.x = preEndTime * timeValue
            clipRoot.y = 0
        }
        onDoubleClicked: {
            timelineController.doubleClickClip()
        }
    }
    MouseArea {
        anchors.fill: clip
        acceptedButtons: Qt.RightButton
        cursorShape: isStoreDrag ? Qt.DragCopyCursor : Qt.ArrowCursor
        onClicked: {
            clipRoot.clicked(clipRoot, false)
            var component
            var page
            if (isVideoClip()) {
                component = Qt.createComponent(
                            "qrc:/ui/timeline/VideoClipMenu.qml")
                page = component.createObject(this)
                page.selectCurrent = timelineController.cursorTime >= startTime
                        && timelineController.cursorTime <= endTime
                page.clipDuration = endTime - preEndTime
            } else if (isImageClip()) {
                component = Qt.createComponent(
                            "qrc:/ui/timeline/ImageClipMenu.qml")
                page = component.createObject(this)
                page.clipDuration = endTime - preEndTime
            } else if (isEffectClip2()) {
                component = Qt.createComponent(
                            "qrc:/ui/timeline/EffectClipMenu.qml")
                page = component.createObject(this)
            } else {
                component = Qt.createComponent(
                            "qrc:/ui/timeline/OtherClipMenu.qml")
                page = component.createObject(this)
            }
            page.x = mouse.x
            page.y = mouse.y
            page.open()
        }
    }
    Rectangle {
        width: 16
        height: 16
        anchors.right: leftDrog.left
        anchors.top: topBorder.top
        anchors.rightMargin: -2
        border.width: 1
        border.color: "#FFFFFF"
        radius: 2
        rotation: 270
        visible: selected && hasInputDevice
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: "#04D7C9"
            }
            GradientStop {
                position: 1
                color: "#02BDE4"
            }
        }
        Image {
            width: 10
            height: 10
            anchors.centerIn: parent
            source: "qrc:/image/assets/timeline_input_flag_icon.png"
        }
    }
    onLeftXChanged: {
        leftDrog.x = leftX
    }
    onRightXChanged: {
        rightDrog.x = rightX
    }
    BorderImage {
        id: leftDrog
        width: borderWidth
        height: parent.height
        visible: selected
        border.top: 2
        border.bottom: 2
        x: leftX
        source: "qrc:/image/assets/timeline_left_drog.png"
        Rectangle {
            anchors.centerIn: parent
            width: 2
            height: 12
            color: "#BFBFBF"
            visible: !hasInputDevice
        }
        Rectangle {
            anchors.centerIn: parent
            width: 12
            height: 2
            rotation: 270
            visible: hasInputDevice
            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: "#04D7C9"
                }
                GradientStop {
                    position: 1
                    color: "#02BDE4"
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeHorCursor
            drag.target: parent
            drag.axis: Drag.XAxis
            acceptedButtons: Qt.LeftButton
            enabled: !freezeDuration
            property real startX
            property real originalClipRowX
            property var clipEdgeList
            onPressed: {
                dragging = true
                startX = leftDrog.x
                originalClipRowX = getClipContentX()
                clipEdgeList = timelineController.getClipEdgeList(
                            trackRoot.itemIndex(), index)
            }
            onPositionChanged: {
                var drogValue = leftDrog.x - startX
                var cursorEdge = timelineController.cursorTime * timeValue
                //大于0说明向右拖，否则向左拖
                if (drogValue > 0) {
                    //计算出拖到右边尽头的位置
                    var endEdge = rightDrog.x - borderWidth - timeScale
                    var value = Math.min(leftDrog.x, endEdge)
                    if (Math.abs(clipRoot.x + value + borderWidth - cursorEdge) <= 10) {
                        //磁吸片段 - 边缘宽度 - 片段的x，就等于磁吸后的位置，但需要对边缘做二次检测，防止超出片段允许的范围
                        value = Math.min(endEdge,
                                         cursorEdge - borderWidth - clipRoot.x)
                    } else {
                        //做磁吸判断
                        for (var clipIndex in clipEdgeList) {
                            var clipEdge = clipEdgeList[clipIndex] * timeValue
                            //片段的x + 左边缘的x + 边缘宽度，就等于片段内容的实际位置，再与要磁吸的片段做比较
                            if (Math.abs(clipRoot.x + value + borderWidth - clipEdge) <= 10) {
                                //磁吸片段 - 边缘宽度 - 片段的x，就等于磁吸后的位置，但需要对边缘做二次检测，防止超出片段允许的范围
                                value = Math.min(
                                            endEdge,
                                            clipEdge - borderWidth - clipRoot.x)
                                guideCursor.visible = true
                                //片段的x - 滚动条的contentX + spaceWidth，等于片段可见区域的x，再加上上面计算出的绝对x位置和边缘宽度，就可得到最后引导光标的正确位置
                                guideCursor.x = clipRoot.x - scrollView.contentX
                                        + spaceWidth + value + borderWidth
                                break
                            } else {
                                guideCursor.visible = false
                            }
                        }
                    }
                } else {
                    //如果是向左拖拽，需要考虑左边缘尽头，音视频片段有尽头，其他可以无限拉伸，所以这里需要做判断
                    var startEdge = Math.max(-borderWidth, leftDrog.x)
                    if (isVideoClip() || isAudioClip()) {
                        //startX - originalClipRowX用来计算音视频片段左边缘还有多少拉伸空间
                        value = Math.max(startX - originalClipRowX, startEdge)
                    } else {
                        value = startEdge
                    }
                    if (Math.abs(clipRoot.x + leftDrog.x + borderWidth - cursorEdge) <= 10) {
                        var startClipEdge1 = Math.max(
                                    -borderWidth,
                                    cursorEdge - borderWidth - clipRoot.x)
                        if (isVideoClip() || isAudioClip()) {
                            value = Math.max(startX - originalClipRowX,
                                             startClipEdge1)
                        } else {
                            value = startClipEdge1
                        }
                    } else {
                        //这里的磁吸和上面不同点是，这里考虑左边缘的允许范围
                        for (clipIndex in clipEdgeList) {
                            clipEdge = clipEdgeList[clipIndex] * timeValue
                            if (Math.abs(clipRoot.x + leftDrog.x + borderWidth - clipEdge) <= 10) {
                                var startClipEdge2 = Math.max(
                                            -borderWidth,
                                            clipEdge - borderWidth - clipRoot.x)
                                if (isVideoClip() || isAudioClip()) {
                                    value = Math.max(startX - originalClipRowX,
                                                     startClipEdge2)
                                } else {
                                    value = startClipEdge2
                                }
                                guideCursor.visible = true
                                guideCursor.x = clipRoot.x - scrollView.contentX
                                        + value + borderWidth + spaceWidth
                                break
                            } else {
                                guideCursor.visible = false
                            }
                        }
                    }
                }
                //重新赋值到左边缘的x，修改成最后值
                leftDrog.x = value
                //对音视频片段里的内容，做同步滚动，leftDrog.x - startX是计算出滚动偏移量，再加上原始偏移量
                if (isVideoClip() || isAudioClip()) {
                    clip.item.contentX = originalClipRowX + leftDrog.x - startX
                }
                console.log("cropPositionChanged:" + leftDrog.x + " -- " + preEndTime)
                timelineController.cropPositionChanged(
                            (leftDrog.x + borderWidth) / timeValue + preEndTime)
            }
            onReleased: {
                dragging = false
                guideCursor.visible = false
                console.log("onReleased cropPositionChanged:" + leftDrog.x + " -- " + clipRoot.x
                            + " ---> " + ((leftDrog.x + borderWidth - blankWidth) / timeValue))
                timelineController.cropLeft(
                            (leftDrog.x + borderWidth) / timeValue + preEndTime)
            }
            onCanceled: {
                timelineController.cancelCropPosition()
                console.log("onCanceled cropPositionChanged:" + leftDrog.x)
            }
        }
    }

    BorderImage {
        id: rightDrog
        width: borderWidth
        height: parent.height
        visible: selected
        border.top: 2
        border.bottom: 2
        x: rightX
        source: "qrc:/image/assets/timeline_right_drog.png"
        Rectangle {
            anchors.centerIn: parent
            width: 2
            height: 12
            color: "#BFBFBF"
            visible: !hasInputDevice
        }
        Rectangle {
            anchors.centerIn: parent
            width: 12
            height: 2
            rotation: 270
            visible: hasInputDevice
            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: "#04D7C9"
                }
                GradientStop {
                    position: 1
                    color: "#02BDE4"
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.SizeHorCursor
            drag.target: parent
            drag.axis: Drag.XAxis
            enabled: !freezeDuration
            acceptedButtons: Qt.LeftButton
            property real startX
            property real endEdge
            property real originalClipWidth
            property var clipEdgeList
            onPressed: {
                startX = rightDrog.x
                originalClipWidth = clip.width
                var nextStartTime_
                if (nextStartTime < 0) {
                    nextStartTime_ = Number.MAX_SAFE_INTEGER
                } else {
                    nextStartTime_ = nextStartTime
                }
                if (isVideoClip() || isAudioClip()) {
                    endEdge = Math.min(
                                (nextStartTime_ - endTime) * timeValue / avgSpeed,
                                resouceTime * timeValue / avgSpeed - getClipContentX(
                                    ) - clip.width)
                } else {
                    endEdge = (nextStartTime_ - cropEndTime) * timeValue
                }
                clipEdgeList = timelineController.getClipEdgeList(
                            trackRoot.itemIndex(), index)
            }
            onPositionChanged: {
                var drogValue = rightDrog.x - startX
                if (drogValue > endEdge) {
                    rightDrog.x = Math.min(startX + endEdge, rightDrog.x)
                } else {
                    rightDrog.x = Math.max(
                                leftDrog.x + borderWidth + timeScale,
                                rightDrog.x)
                }
                var cursorEdge = timelineController.cursorTime * timeValue
                if (Math.abs(clipRoot.x + rightDrog.x - cursorEdge) <= 10) {
                    if (drogValue > 0) {
                        rightDrog.x = Math.min(startX + endEdge,
                                               cursorEdge - clipRoot.x)
                    } else {
                        rightDrog.x = Math.max(
                                    leftDrog.x + borderWidth + timeScale,
                                    cursorEdge - clipRoot.x)
                    }
                } else {
                    //右边缘吸附，原理和左边缘相识，注释看上面
                    for (var clipIndex in clipEdgeList) {
                        var clipEdge = clipEdgeList[clipIndex] * timeValue
                        if (Math.abs(clipRoot.x + rightDrog.x - clipEdge) <= 10) {
                            if (drogValue > 0) {
                                rightDrog.x = Math.min(startX + endEdge,
                                                       clipEdge - clipRoot.x)
                            } else {
                                rightDrog.x = Math.max(
                                            leftDrog.x + borderWidth + timeScale,
                                            clipEdge - clipRoot.x)
                            }
                            guideCursor.visible = true
                            guideCursor.x = clipRoot.x - scrollView.contentX
                                    + rightDrog.x + spaceWidth
                            break
                        } else {
                            guideCursor.visible = false
                        }
                    }
                }
                timelineController.cropPositionChanged(
                            rightDrog.x / timeValue + preEndTime)
                if (isVideoClip() || isAudioClip()) {
                    clip.item.contentX = cropStartTime * timeValue + pausePosition
                }
            }
            onReleased: {
                guideCursor.visible = false
                timelineController.cropRight(
                            rightDrog.x / timeValue + preEndTime)
            }
            onCanceled: {
                timelineController.cancelCropPosition()
                console.log("onCanceled right cropPositionChanged!!")
            }
        }
    }

    Loader {
        id: clip
        anchors.top: topBorder.bottom
        anchors.bottom: bottomBorder.top
        anchors.left: leftDrog.right
        anchors.right: rightDrog.left
        clip: true
        sourceComponent: {
            if (isVideoClip()) {
                return videoClip
            } else if (isAudioClip()) {
                return audioClip
            } else if (isImageClip()) {
                return imageClip
            } else if (isAiClip()) {
                return aiClip
            } else {
                return effectClip
            }
        }
    }

    onClipContentXChanged: {
        if (isVideoClip() || isAudioClip()) {
            if (clip.item.contentX !== clipContentX && !dragging) {
                clip.item.contentX = clipContentX
            }
        }
    }
    Component {
        id: videoClip
        VideoClip {
            onContentXChanged: {
                if (contentX !== clipContentX && !dragging) {
                    contentX = clipContentX
                }
            }
        }
    }
    Component {
        id: audioClip
        AudioClip {
            contentX: clipContentX
            onContentXChanged: {
                if (contentX !== clipContentX && !dragging) {
                    contentX = clipContentX
                }
            }
        }
    }
    Component {
        id: imageClip
        ImageClip {}
    }
    Component {
        id: effectClip
        EffectClip {}
    }
    Component {
        id: aiClip
        AiClip {}
    }

    Rectangle {
        id: topBorder
        anchors.top: parent.top
        anchors.left: leftDrog.right
        anchors.right: rightDrog.left
        height: 1
        color: "white"
        visible: selected
    }

    Rectangle {
        id: bottomBorder
        anchors.bottom: parent.bottom
        anchors.left: leftDrog.right
        anchors.right: rightDrog.left
        height: 1
        color: "white"
        visible: selected
    }
    Item {
        id: linkIcon
        width: 20
        height: parent.height
        anchors.horizontalCenter: clip.left
        anchors.horizontalCenterOffset: -pausePosition
        visible: {
            if (isVideoClip() || isImageClip()) {
                var moving
                if (movingIndex >= 0 && (index - movingIndex) >= 0) {
                    moving = (index - movingIndex) > 1
                } else {
                    moving = true
                }
                if (Math.abs(blankWidth) < 0.0000001 && index > 0 && !selected
                        && !transitionIcon.visible && moving) {
                    return true
                }
            }
            return false
        }
        Image {
            width: 20
            height: 20
            anchors.centerIn: parent
            source: "qrc:/image/assets/timeline_link.png"
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                timelineController.addTransitionModelIndex(
                            trackRoot.itemIndex(), index)
                storeRect.addTransitionFromClip()
                timelineController.cursorTime = startTime
            }
        }
    }
    //转场图标
    Item {
        id: transitionIcon
        width: transitonDuring * timeValue
        height: clip.height
        anchors.horizontalCenter: clip.left
        anchors.horizontalCenterOffset: -pausePosition
        anchors.verticalCenter: clip.verticalCenter
        visible: {
            var moving
            if (movingIndex >= 0 && (index - movingIndex) >= 0) {
                moving = (index - movingIndex) > 1
            } else {
                moving = true
            }
            return index > 0 && !selected && moving && transitonDuring >= 0
        }
        Rectangle {
            height: parent.height
            anchors.left: transitionLeftDrog.right
            anchors.right: transitionRightDrog.left
            color: "#cccccc"
            opacity: 0.6
            radius: transitionSelected ? 0 : 2
        }
        BorderImage {
            id: transitionLeftDrog
            width: borderWidth
            height: parent.height
            visible: transitionSelected
            border.top: 2
            border.bottom: 2
            x: -borderWidth + transitionDrogOffset
            source: "qrc:/image/assets/timeline_left_drog.png"
            Rectangle {
                anchors.centerIn: parent
                width: 2
                height: 12
                color: "#BFBFBF"
            }
        }
        Item {
            width: borderWidth
            height: parent.height
            x: -borderWidth
            enabled: transitionSelected
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.SizeHorCursor
                acceptedButtons: Qt.LeftButton
                property real startX
                property real beginTransitionDuring: 0
                onPressed: {
                    startX = mouseX
                    beginTransitionDuring = transitonDuring
                }
                onPositionChanged: {
                    var offset = mouseX - startX
                    var newTransitionDuring = (transitionIcon.width
                                               - (transitionLeftDrog.x - transitionDrogOffset
                                                  + offset + borderWidth) * 2) / timeValue
                    if (newTransitionDuring < minTransitonDuring) {
                        transitionDrogOffset = (transitionIcon.width
                                                - minTransitonDuring * timeValue) / 2
                    } else if (newTransitionDuring > maxTransitonDuring) {
                        transitionDrogOffset = (transitionIcon.width
                                                - maxTransitonDuring * timeValue) / 2
                    } else {
                        transitionDrogOffset = offset
                    }
                    timelineController.setTransitionDuring(newTransitionDuring)
                }
                onReleased: {
                    var newTransitionDuring = (transitionIcon.width
                                               - (transitionLeftDrog.x
                                                  + borderWidth) * 2) / timeValue
                    timelineController.setTransitionDuring(
                                beginTransitionDuring, newTransitionDuring)
                    transitionDrogOffset = 0
                }
                onCanceled: {
                    timelineController.setTransitionDuring(
                                beginTransitionDuring)
                }
            }
        }
        BorderImage {
            id: transitionRightDrog
            width: borderWidth
            height: parent.height
            visible: transitionSelected
            border.top: 2
            border.bottom: 2
            x: transitionIcon.width - transitionDrogOffset
            source: "qrc:/image/assets/timeline_right_drog.png"
            Rectangle {
                anchors.centerIn: parent
                width: 2
                height: 12
                color: "#BFBFBF"
            }
        }
        Item {
            width: borderWidth
            height: parent.height
            x: transitionIcon.width
            enabled: transitionSelected
            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.SizeHorCursor
                acceptedButtons: Qt.LeftButton
                property real startX
                property real beginTransitionDuring: 0
                onPressed: {
                    startX = mouseX
                    beginTransitionDuring = transitonDuring
                }
                onPositionChanged: {
                    var offset = startX - mouseX
                    var newTransitionDuring = (transitionIcon.width
                                               - (transitionLeftDrog.x - transitionDrogOffset
                                                  + offset + borderWidth) * 2) / timeValue
                    if (newTransitionDuring < minTransitonDuring) {
                        transitionDrogOffset = (transitionIcon.width
                                                - minTransitonDuring * timeValue) / 2
                    } else if (newTransitionDuring > maxTransitonDuring) {
                        transitionDrogOffset = (transitionIcon.width
                                                - maxTransitonDuring * timeValue) / 2
                    } else {
                        transitionDrogOffset = offset
                    }
                    timelineController.setTransitionDuring(newTransitionDuring)
                }
                onReleased: {
                    var newTransitionDuring = (transitionIcon.width
                                               - (transitionLeftDrog.x
                                                  + borderWidth) * 2) / timeValue
                    timelineController.setTransitionDuring(
                                beginTransitionDuring, newTransitionDuring)
                    transitionDrogOffset = 0
                }
            }
        }
        Rectangle {
            id: transitionTopBorder
            anchors.top: parent.top
            anchors.left: transitionLeftDrog.right
            anchors.right: transitionRightDrog.left
            height: 1
            color: "white"
            visible: transitionSelected
        }
        Rectangle {
            id: transitionBottomBorder
            anchors.bottom: parent.bottom
            anchors.left: transitionLeftDrog.right
            anchors.right: transitionRightDrog.left
            height: 1
            color: "white"
            visible: transitionSelected
        }
        Image {
            width: 20
            height: 20
            anchors.centerIn: parent
            source: "qrc:/image/assets/timeline_transiton.png"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    clipRoot.clicked(clipRoot, true)
                }
            }
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                clipRoot.clicked(clipRoot, true)
            }
        }
    }
    states: [
        State {
            name: 'normal'
            when: !(selected || moving)
            PropertyChanges {
                target: clipRoot
                z: 0
            }
        },
        State {
            name: 'selected'
            when: selected || moving
            PropertyChanges {
                target: clipRoot
                z: 1
            }
        }
    ]
    Rectangle {
        anchors.fill: clip
        border.color: "white"
        border.width: 1
        color: "transparent"
        visible: clipRoot === dragToclip
        Rectangle {
            anchors.fill: parent
            color: "white"
            opacity: 0.3
        }
    }
    Keyframe {
        anchors.fill: clip
        visible: selected
    }
    AsyncProcessModel {
        id: asyncProcessModel
    }
    Flickable {
        id: asynccontainer
        anchors.fill: clip
        interactive: false
        clip: false
        AsyncProcess {
            id: asyncProgress
            width: parent.width
            height: parent.height
        }
    }

    Row {
        anchors.left: leftDrog.right
        anchors.right: rightDrog.left
        anchors.bottom: parent.bottom
        spacing: 4
        Repeater {
            model: fashionLineGroupInfos
            Label {
                color: "#FFFFFF"
                font.pixelSize: 8
                text: fashionLineGroupInfos[index]
                leftPadding: 2
                rightPadding: 2
                topPadding: 1
                bottomPadding: 1
                background: Rectangle {
                    color: timelineController.getRandomColor(text)
                    radius: 2
                }

                onTextChanged: {
                    background.color = timelineController.getRandomColor(text)
                }
            }
        }
    }

    Component.onCompleted: {
        timelineController.attachAsyncProcessModel(trackRoot.itemIndex(),
                                                   index, asyncProcessModel)
        if (isVideoClip() || isAudioClip()) {
            if (clip.item.contentX !== clipContentX && !dragging) {
                clip.item.contentX = clipContentX
            }
        }
    }
}
