import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQml.Models 2.12
import QtQml 2.12
import "Timeline.js" as Logic
import "Ruler.js" as RulerLogic
import TimelineModel 1.0
import TimelineController 1.0
import "../widget/"

Rectangle {
    id: timeRoot
    property alias scrollView: scrollView
    property alias tracksContainer: tracksContainer
    property alias trackCount: trackDelegateModel.count
    property var timelineWatchError: ""
    property bool timelineWatchChecked: false
    property var dragToclip
    property bool isStoreDrag: false
    property int ruleHeight: 40
    property int trackHeight: 32
    property int trackSpaceHeight: 2
    property int fps: timelineController.fps
    property int minScaleIndex: 0
    property int maxScaleIndex: 103
    property int clipDrogNewIndex: -1
    property int scroolBarWidth: 8
    property int maxSpaceWidth: 80
    property int guideEndCount: 1
    property int spaceWidth: Math.max(0, maxSpaceWidth - scrollView.contentX)
    property int spaceHeight: Math.max(0, 22 - scrollView.contentY)
    property real timeScale: timelineController.timeScale.toFixed(7)
    property real timeValue: fps * timeScale

    //缩放
    property int scaleIndex: maxScaleIndex
    property int bestMaxScaleIndex: maxScaleIndex
    property int numFrame: 2
    property int allFrame: 2
    property real rulerWidth: 44
    property int updateVideoClip
    property int trackMovingIndex: -1
    property bool storePreview: storeRect.storeFocusItem !== null
                                && storeRect.storeFocusItem.focus
    function adjustZoom(by) {
        if (timelineModel.rowCount() > 0) {
            var lastPosition = cursor.x
            RulerLogic.setTimeScale(-by)
            Logic.updateCursor(lastPosition)
        }
    }

    function getScaleIndexFromMinMax(min, max, scale) {
        return RulerLogic.getScaleIndexFromMinMax(min, max, scale)
    }

    function updateBestScale(maxScaleIndexWidthScale, bestWidthScale, notifyBestScale) {
        bestMaxScaleIndex = getScaleIndexFromMinMax(minScaleIndex,
                                                    maxScaleIndex,
                                                    maxScaleIndexWidthScale)
        var bestScaleIndex = getScaleIndexFromMinMax(minScaleIndex,
                                                     bestMaxScaleIndex,
                                                     maxScaleIndexWidthScale)
        return RulerLogic.getTimeScale(bestScaleIndex, notifyBestScale)
    }

    function getTrackHeigthFromIndex(index) {
        if (trackDelegateModel.count === 0) {
            return 0
        }
        return trackHeight
    }

    function changeCursor(mouseX) {
        timeRoot.focus = true
        var cursorX = scrollView.contentX + mouseX - spaceWidth
        if (cursorX < 0) {
            cursorX = 0
        } else if (cursorX > tracksContainer.width) {
            cursorX = tracksContainer.width
        }
        //位置不同才更新
        if ((cursorX - scrollView.contentX + spaceWidth - cursor.width / 2) != cursor.x) {
            timelineController.cursorTime = cursorX / timeValue
        }
    }

    function autoScroll(x, leftMargin, item, mouseX) {
        if (x > scrollView.width - 50) {
            scrollTimer.item = item
            scrollTimer.leftMargin = leftMargin
            scrollTimer.mouseX = mouseX
            scrollTimer.backwards = false
            scrollTimer.start()
        } else if (x < 50) {
            scrollTimer.item = item
            scrollTimer.leftMargin = leftMargin
            scrollTimer.mouseX = mouseX
            scrollTimer.backwards = true
            scrollTimer.start()
        } else {
            scrollTimer.stop()
        }
    }

    function inClipFromXY(x, y) {
        var trackIndex = 0
        for (; trackIndex < trackDelegateModel.count; trackIndex++) {
            var track = tracksRepeater.itemAt(trackIndex)
            if (track === null || track.height === 0) {
                continue
            }
            var trackXy = track.mapToGlobal(0, 0)
            if (y >= trackXy.y && y <= trackXy.y + track.height) {
                var clip = track.inClipFromXY(x, y)
                if (clip === null) {
                    continue
                } else {
                    return clip
                }
            }
        }
        return null
    }

    function watchTimeline() {
        if (!timelineWatchChecked) {
            return
        }
        timelineWatchError = ""
        for (var trackIndex = 0; trackIndex < trackDelegateModel.count; trackIndex++) {
            var track = tracksRepeater.itemAt(trackIndex)
            if (track.height === 0) {
                timelineModel.checkTrackError(trackIndex, 0)
            } else {
                timelineModel.checkTrackError(trackIndex,
                                              track.trackModel.count)
                for (var clipIndex = 0; clipIndex < track.trackModel.count; clipIndex++) {
                    var clip = track.trackRepeater.itemAt(clipIndex)
                    timelineModel.checkClipError(trackIndex, clipIndex,
                                                 clip.clipStartTime,
                                                 clip.clip.width / timeValue)
                }
            }
        }
    }
    function hotkeyDelete() {
        var isClipSelected = timelineController.selection[0] >= 0
                && timelineController.selection[1] >= 0
        if (isClipSelected && !storePreview) {
            timelineController.removeClip()
        }
    }
    color: "#15171C"
    TimelineModel {
        id: timelineModel
        onWatchError: {
            timelineWatchError += error + "\n"
        }
    }

    TimelineController {
        id: timelineController
        onToastContentChanged: {
            timelineToastMgr.show(msg, during)
        }
        onWatchChanged: {
            timelineWatchChecked = check
        }
        onUndoTextChanged: {
            watchTimeline()
        }
        onRedoTextChanged: {
            watchTimeline()
        }
    }

    Component.onCompleted: {
        timelineController.attachModel(timelineModel)
    }
    Connections {
        target: timelineModel
        onRowsInserted: {
            guideEndCount++
            guideEndCount--
        }
        onRowsRemoved: {
            guideEndCount++
            guideEndCount--
        }
    }
    //工具栏
    TimelineToolbar {
        id: toolbar
        anchors.top: parent.top
        anchors.left: parent.left
    }

    Rectangle{
        id:divider
        width: parent.width
        height: 1
        anchors.top: toolbar.bottom

        color:"black"
    }

    ToastManager {
        id: timelineToastMgr
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter
        }
    }

    onStorePreviewChanged: {
        if (storePreview && timelineController.selection.length > 0
                && timelineController.selection[0] >= 0) {
            timelineController.selection = [-1, -1, false]
        }
    }

    Rectangle {
        id: tracksLayout
        anchors.top: divider.bottom
        anchors.bottom: parent.bottom
        Column {
            //时间轴
            MouseArea {
                width: timeRoot.width
                height: ruleHeight
                acceptedButtons: Qt.LeftButton
                property bool isPlaying
                onPressed: {
                    isPlaying = timelineController.isPlaying()
                    if (isPlaying) {
                        timelineController.stop()
                    }
                    changeCursor(mouse.x)
                }
                onPositionChanged: {
                    if (!scrollTimer.running) {
                        changeCursor(mouse.x)
                    }
                    var cursorX = scrollView.contentX + mouse.x - spaceWidth
                    if (cursorX >= 0 && cursorX <= tracksContainer.width) {
                        autoScroll(mouse.x, 0, null, mouse.x)
                    }
                }
                onReleased: {
                    scrollTimer.running = false
                    changeCursor(mouse.x)
                    if (isPlaying && !timelineController.isPlayEnd()) {
                        timelineController.play()
                    }
                }
                Flickable {
                    id: rulercontainer
                    width: parent.width
                    height: parent.height
                    contentWidth: scrollView.width
                    interactive: false
                    clip: false
                    x: spaceWidth
                    onContentXChanged: {
                        if (rulercontainer.contentX != scrollView.contentX) {
                            rulercontainer.contentX = scrollView.contentX
                        }
                    }
                    Ruler {
                        id: ruler
                        width: parent.width
                        height: parent.height
                    }
                }
            }
            MouseArea {
                width: timeRoot.width
                height: tracksLayout.height - ruleHeight
                acceptedButtons: Qt.LeftButton
                onWheel: Logic.onTimelineMouseWheel(wheel, scrollView)
                cursorShape: isStoreDrag ? Qt.DragCopyCursor : Qt.ArrowCursor
                property int pressedX: -1
                property bool isClick: false
                onPressed: {
                    isClick = true
                    pressedX = mouseX
                }
                onPositionChanged: {
                    if (Math.abs(mouseX - pressedX) > 1) {
                        isClick = false
                    }
                }
                onReleased: {
                    if (isClick && mouse.button === Qt.LeftButton) {
                        timelineController.selection = [-1, -1, false]
                        changeCursor(mouse.x)
                    }
                }
                PinchHandler {
                    target: null
                    property real lastScale: scale
                    property int scaleCount: 0
                    onActiveChanged: {
                        if (!active) {
                            ++updateVideoClip
                        }
                    }
                    onScaleChanged: {
                        if (Math.abs(scaleCount) > 7) {
                            adjustZoom(scaleCount)
                            scaleCount = 0
                        } else {
                            if (scale - lastScale > 0) {
                                if (scaleCount > 0) {
                                    scaleCount++
                                } else {
                                    scaleCount = 1
                                }
                            } else {
                                if (scaleCount < 0) {
                                    scaleCount--
                                } else {
                                    scaleCount = -1
                                }
                            }
                            lastScale = scale
                        }
                    }
                }
                //轨道
                Flickable {
                    id: scrollView
                    width: parent.width
                    height: parent.height
                    interactive: false
                    clip: true
                    contentWidth: tracksContainer.width + 550
                    contentHeight: tracksContainer.height + 30 // 30 is padding
                    ScrollBar.horizontal: ScrollBar {
                        height: scroolBarWidth
                        minimumSize: 0.02
                        policy: ScrollBar.AlwaysOn
                        visible: parent.contentWidth > parent.width
                    }
                    ScrollBar.vertical: ScrollBar {
                        width: scroolBarWidth
                        minimumSize: 0.1
                        policy: ScrollBar.AlwaysOn
                        visible: parent.contentHeight > parent.height
                    }
                    onContentXChanged: {
                        if (rulercontainer.contentX != scrollView.contentX) {
                            rulercontainer.contentX = scrollView.contentX
                        }
                    }
                    // 视频轨道
                    Column {
                        id: tracksContainer
                        spacing: trackSpaceHeight
                        x: spaceWidth
                        y: spaceHeight
                        Repeater {
                            id: tracksRepeater
                            model: trackDelegateModel
                        }
                    }
                    //边缘新增轨道引导线
                    Column {
                        y: spaceHeight - trackSpaceHeight
                        Repeater {
                            id: tracksGuideRepeater
                            model: tracksRepeater.count + guideEndCount
                            Item {
                                clip: true
                                width: Math.max(scrollView.contentWidth,
                                                scrollView.width)
                                height: {
                                    if (timelineController.getClipCount(
                                                index) > 0
                                            || index === tracksRepeater.count) {
                                        return getTrackHeigthFromIndex(
                                                    index) + trackSpaceHeight
                                    }
                                    return 0
                                }
                                Rectangle {
                                    width: parent.width
                                    height: trackSpaceHeight
                                    color: "#02E4D6"
                                    opacity: clipDrogNewIndex == index ? 1 : 0
                                }
                            }
                        }
                    }
                }
            }
        }
        BorderImage {
            id: cursor
            width: 18
            height: tracksLayout.height - scroolBarWidth
            visible: timelineController.cursorTime >= 0
            border.top: 22
            border.bottom: 22
            verticalTileMode: BorderImage.Repeat
            x: timelineController.cursorTime * timeValue - scrollView.contentX
               + spaceWidth - width / 2
            source: "qrc:/image/assets/timeline_cursor.png"
            opacity: storePreview ? 0.5 : 1
        }
        MouseArea {
            anchors.fill: cursor
            acceptedButtons: Qt.LeftButton
            cursorShape: isStoreDrag ? Qt.DragCopyCursor : Qt.ArrowCursor
            property bool isPlaying
            onPressed: {
                isPlaying = timelineController.isPlaying()
                if (isPlaying) {
                    timelineController.stop()
                }
                changeCursor(cursor.x + mouseX)
            }
            onPositionChanged: {
                if (!scrollTimer.running) {
                    changeCursor(cursor.x + mouseX)
                }
                var cursorX = scrollView.contentX + mouse.x + mouseX - spaceWidth
                if (cursorX >= 0 && cursorX <= tracksContainer.width) {
                    autoScroll(cursor.x + mouseX, 0, null, cursor.x + mouseX)
                }
            }
            onReleased: {
                scrollTimer.running = false
                changeCursor(cursor.x + mouseX)
                if (isPlaying) {
                    timelineController.play()
                }
            }
        }
        Rectangle {
            id: guideCursor
            width: 2
            height: tracksLayout.height - scroolBarWidth
            color: "#02E4D6"
            visible: false
        }
    }
    Shortcut {
        context: shortcutContext
        sequence: StandardKey.Undo
        onActivated: {
            timelineController.undo()
        }
    }
    Shortcut {
        context: shortcutContext
        sequence: StandardKey.Redo
        onActivated: {
            timelineController.redo()
        }
    }
    Shortcut {
        context: shortcutContext
        sequence: StandardKey.Copy
        onActivated: {
            timelineController.preparewCopySelectedClip()
        }
    }
    Shortcut {
        context: shortcutContext
        sequence: StandardKey.Paste
        onActivated: {
            timelineController.copySelectedClip()
        }
    }
    Shortcut {
        context: shortcutContext
        sequence: "Space"
        enabled: !storePreview
        onActivated: {
            if (timelineController.isPlaying()) {
                timelineController.stop()
            } else {
                timelineController.play()
            }
        }
    }
    Shortcut {
        context: shortcutContext
        sequence: "Left"
        enabled: !storePreview
        onActivated: {
            cursorTimer.backwards = true
            cursorTimer.start()
        }
    }
    Shortcut {
        context: shortcutContext
        sequence: "Right"
        enabled: !storePreview
        onActivated: {
            cursorTimer.backwards = false
            cursorTimer.start()
        }
    }
    DropArea {
        id: dropArea
        anchors.fill: parent
        onDropped: {
            if (drop.hasUrls) {
                for (var i = 0; i < drop.urls.length; i++) {
                    console.log("Open:" + drop.urls[i])
                    timelineController.addResource(drop.urls[i])
                }
            }
        }
        onEntered: {

        }
    }

    DelegateModel {
        id: trackDelegateModel
        model: timelineModel
        Track {
            model: timelineModel
            rootIndex: trackDelegateModel.modelIndex(index)
            height: getTrackHeigthFromIndex(index)
            onClipDragged: {
                autoScroll(x, leftMargin, clip, -1)
            }
            onClipReleased: {
                scrollTimer.running = false
            }
            onClipClicked: {
                var clipIndex = clip.DelegateModel.itemsIndex
                var trackIndex = track.DelegateModel.itemsIndex
                if (!Logic.selectionContains(trackIndex, clipIndex)) {
                    timelineController.selection = [trackIndex, clipIndex, isTransiton]
                }
                timeRoot.focus = true
            }
        }
    }
    //光标移动
    Timer {
        id: cursorTimer
        interval: 1 / fps
        repeat: false
        triggeredOnStart: true
        property bool backwards
        onTriggered: {
            //停止预览播放
            timelineController.stop()
            //精度丢失，需加一点容差来配合计算
            var tolerance = 0.0000001
            var step = timeValue / fps
            var cursorX = cursor.x + cursor.width / 2
            var left = scrollView.contentX % rulerWidth
                    == 0 ? 0 : rulerWidth - scrollView.contentX % rulerWidth
            var cursorIndex = Math.floor(
                        (cursorX - left - spaceWidth) / rulerWidth + tolerance)
            //下面的目的是要左右滑动时，如果符合条件，优先落在刻度上
            if (backwards) {
                var nextRuler = rulerWidth * cursorIndex + left + spaceWidth
                if (Math.abs(cursorX - nextRuler) > tolerance
                        && cursorX - step < nextRuler) {
                    changeCursor(nextRuler)
                } else {
                    changeCursor(cursorX - step)
                }
            } else {
                var lastRuler = rulerWidth * (cursorIndex + 1) + left + spaceWidth
                changeCursor(Math.min(lastRuler, cursorX + step))
            }
        }
    }
    //拖片段到左右边缘时自动滚动
    Timer {
        id: scrollTimer
        interval: 25
        repeat: true
        triggeredOnStart: true
        property var item
        property bool backwards
        property int leftMargin
        property int mouseX
        onTriggered: {
            var delta = backwards ? -10 : 10
            if (item) {
                item.x = Math.max(-leftMargin, item.x + delta)
            }
            if (mouseX >= 0) {
                changeCursor(mouseX)
            }
            if (scrollView.contentX + delta < 0) {
                stop()
            } else {
                scrollView.contentX += delta
            }
        }
    }
}
