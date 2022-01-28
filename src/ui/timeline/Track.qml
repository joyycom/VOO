import QtQuick 2.12
import QtQml.Models 2.12
import "Track.js" as Logic

Rectangle {
    property alias trackModel: trackModel
    property alias trackRepeater: repeater
    property alias model: trackModel.model
    property alias rootIndex: trackModel.rootIndex
    property int movingIndex: -1
    signal clipDragged(var clip, var x, var leftMargin)
    signal clipClicked(var clip, var track, var isTransiton)
    signal clipReleased

    function itemIndex() {
        return trackRoot.DelegateModel.itemsIndex
    }
    function inClipFromXY(x, y) {
        var clipIndex = 0
        for (; clipIndex < trackModel.count; clipIndex++) {
            var clip = repeater.itemAt(clipIndex).inClipFromXY(x, y)
            if(clip !== null){
                return clip;
            }
        }
        return null
    }

    id: trackRoot
    color: 'transparent' //'red'
    width: clipRow.width
    Rectangle {
        id: trackBgTop
        width: Math.max(scrollView.contentWidth, scrollView.width)
        height: 1
        color: "white"
        opacity: 0.1
        visible: timelineController.selection[0] === index
                 || trackMovingIndex === index
    }
    Rectangle {
        id: trackBgBottom
        width: Math.max(scrollView.contentWidth, scrollView.width)
        height: 1
        color: "white"
        opacity: 0.1
        anchors.bottom: parent.bottom
        visible: timelineController.selection[0] === index
                 || trackMovingIndex === index
    }
    Rectangle {
        id: trackBgCenter
        width: Math.max(scrollView.contentWidth, scrollView.width)
        color: "white"
        opacity: 0.05
        anchors.top: trackBgTop.bottom
        anchors.bottom: trackBgBottom.top
        visible: timelineController.selection[0] === index
                 || trackMovingIndex === index
    }
    DelegateModel {
        id: trackModel
        Clip {
            selected: Logic.selectionContains(
                          timelineController.selection,
                          trackRoot.DelegateModel.itemsIndex, index)
            transitionSelected: Logic.transitionSelectionContains(
                                    timelineController.selection,
                                    trackRoot.DelegateModel.itemsIndex, index)
            height: trackRoot.height - 2
            onDragged: {
                trackRoot.clipDragged(clip, x, leftMargin)
            }
            onClicked: {

                trackRoot.clipClicked(clip, trackRoot, isTransiton)
                console.log("timeline=>onClicked: index: " + index + " DelegateModel.itemsIndex:"
                            + DelegateModel.itemsIndex + " trackRoot.DelegateModel.itemsIndex:"
                            + trackRoot.DelegateModel.itemsIndex)
            }
            Component.onCompleted: {
                released.connect(trackRoot.clipReleased)
            }
        }
    }

    Row {
        id: clipRow
        anchors.verticalCenter: parent.verticalCenter
        Repeater {
            id: repeater
            model: trackModel
        }
    }

    states: [
        State {
            name: 'normal'
            when: !(Logic.selectionContains(timelineController.selection,
                                            itemIndex(),
                                            -1) || movingIndex >= 0)
            PropertyChanges {
                target: trackRoot
                z: 0
            }
            PropertyChanges {
                target: clipRow
                z: 0
            }
        },
        State {
            name: 'selected'
            when: Logic.selectionContains(timelineController.selection,
                                          itemIndex(), -1) || movingIndex >= 0
            PropertyChanges {
                target: trackRoot
                z: 1
            }
            PropertyChanges {
                target: clipRow
                z: 1
            }
        }
    ]
}
