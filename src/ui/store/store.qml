import QtQuick 2.12
import QtQuick.Controls 2.12
import StoreDataSource 1.0
import CategoryListModel 1.0
import StoreResourceListModel 1.0
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.12
import QmlUtils 1.0
import "../widget/"

Rectangle {
    id: store
    width: parent.width
    height: parent.height
    color: "#15171C"
    property var storeFocusItem: resourceList.focus ? resourceList : videoLocalList.focus ? videoLocalList : audioLocalList.focus ? audioLocalList : moreLocalList.focus ? moreLocalList : null
    property var downloadingRes: []

    function hotkeyDelete() {
        if (videoLocalList.focus) {
            videoMouseSelect.execDelete()
        } else if (audioLocalList.focus) {
            audioMouseSelect.execDelete()
        } else if (moreLocalList.focus) {
            moreMouseSelect.execDelete()
        }
    }
    function hotkeySelectAll() {
        if (storeFocusItem) {
            if (videoLocalList.visible) {
                videoMouseSelect.selectAll(videoLocalList.count)
            } else if (audioLocalList.visible) {
                audioMouseSelect.selectAll(audioLocalList.count)
            } else if (moreLocalList.visible) {
                moreMouseSelect.selectAll(moreLocalList.count)
            }
        }
    }
    StoreDataSource {
        id: datasource
        onCategoryListChanged: {
            categorylistView.model = data
            categorylistView.currentIndex = 0
            storeCategoryVideo.updateSelectState()
            storeCategoryAudio.updateSelectState()
            storeCategoryText.updateSelectState()
            storeCategorySticker.updateSelectState()
            storeCategoryEffect.updateSelectState()
            storeCategoryTransition.updateSelectState()
            storeCategoryFilter.updateSelectState()
            storeCategoryMore.updateSelectState()
        }
        onShowVideoLocalList: {
            hidAllPanel()
            videoLocalContainer.visible = true
        }
        onVideoListChanged: {
            videoEmptyView.visible = datasource.localVideoDataCount() <= 1
            videoLocalList.visible = datasource.localVideoDataCount() > 1
            videoLocalList.model = data
        }
        onShowAudioLocalList: {
            hidAllPanel()
            audioLocalContainer.visible = true
        }
        onAudioListChanged: {
            audioEmptyView.visible = datasource.localAudioDataCount() <= 1
            audioLocalList.visible = datasource.localAudioDataCount() > 1
            audioLocalList.model = data
        }
        onShowMoreLocalList: {
            hidAllPanel()
            moreLocalContainer.visible = true
        }
        onMoreListChanged: {
            moreEmptyView.visible = datasource.localMoreDataCount() <= 1
            moreLocalList.visible = datasource.localMoreDataCount() > 1
            moreLocalList.model = data
        }
        onShowResourceList: {
            hidAllPanel()
            resourceList.visible = true
        }
        onResourceListChanged: {
            downloadingRes = []
            resourceList.model = data
        }
        onResourceDownloadComplate: {
            downloadingRes = downloadingRes.filter(function (ele) {
                return ele !== index
            })
        }

        onToastContentChanged: {
            storeToastMgr.show(msg, during)
        }

        function hidAllPanel() {
            videoLocalContainer.visible = false
            audioLocalContainer.visible = false
            moreLocalContainer.visible = false
            resourceList.visible = false
        }
    }
    ToolBar {
        id: topCategory
        anchors {
            leftMargin: 20
            left: parent.left
            right: parent.right
            top: parent.top
        }
        height: 60
        background: Rectangle {
            anchors.fill: parent
            color: "#15171C"
        }

        contentItem: Item {
            Row {
                spacing: 16
                StoreMenuBar {
                    id: storeCategoryVideo
                    imageSource: "qrc:/image/assets/store_video.png"
                    hoveredImageSource: "qrc:/image/assets/store_video_hovered.png"
                    cate: "VIDEO"
                    txt: qsTr("Video")
                }
                StoreMenuBar {
                    id: storeCategoryAudio
                    imageSource: "qrc:/image/assets/store_audio.png"
                    hoveredImageSource: "qrc:/image/assets/store_audio_hovered.png"
                    cate: "AUDIO"
                    txt: qsTr("Audio")
                }
                StoreMenuBar {
                    id: storeCategoryText
                    imageSource: "qrc:/image/assets/store_text.png"
                    hoveredImageSource: "qrc:/image/assets/store_text_hovered.png"
                    cate: "WORDS"
                    txt: qsTr("Text")
                }
                StoreMenuBar {
                    id: storeCategorySticker
                    imageSource: "qrc:/image/assets/store_sticker.png"
                    hoveredImageSource: "qrc:/image/assets/store_sticker_hovered.png"
                    cate: "STICKER"
                    txt: qsTr("Sticker")
                }
                StoreMenuBar {
                    id: storeCategoryEffect
                    imageSource: "qrc:/image/assets/store_effect.png"
                    hoveredImageSource: "qrc:/image/assets/store_effect_hovered.png"
                    cate: "EFFECT"
                    txt: qsTr("Effect")
                }
                StoreMenuBar {
                    id: storeCategoryTransition
                    imageSource: "qrc:/image/assets/store_transition.png"
                    hoveredImageSource: "qrc:/image/assets/store_transition_hovered.png"
                    cate: "TRANSTIONS"
                    txt: qsTr("Transition")
                }
                StoreMenuBar {
                    id: storeCategoryFilter
                    imageSource: "qrc:/image/assets/store_filter.png"
                    hoveredImageSource: "qrc:/image/assets/store_filter_hovered.png"
                    cate: "FILTER"
                    txt: qsTr("Filter")
                }
                StoreMenuBar {
                    id: storeCategoryMore
                    imageSource: "qrc:/image/assets/store_more.png"
                    hoveredImageSource: "qrc:/image/assets/store_more_hovered.png"
                    cate: "MORE"
                    txt: qsTr("More")
                }
            }
        }
    }

    Rectangle {
        id: topCategoryBottoLine
        width: parent.width
        height: 2
        color: "#000000"
        anchors.top: topCategory.bottom
    }

    ListView {
        id: categorylistView
        width: 120
        anchors.top: topCategoryBottoLine.bottom
        anchors.bottom: parent.bottom
        model: datasource.categoryListModel()
        clip: true
        //        snapMode: ListView.SnapToItem
        //        headerPositioning: ListView.OverlayHeader
        delegate: Rectangle {
            width: parent.width
            height: t1.height + subCategorylistView.height
            focus: true
            color: "#15171C"

            property int subIndex: index

            Rectangle {
                id: t1
                color: selectedSubIndex == index
                       && selectedThirdIndex == -1 ? "#19ffffff" : "#00ffffff"
                radius: 2
                width: 100
                height: 34
                visible: true
                x: 10
                y: 5
                Text {
                    text: title
                    width: parent.width
                    height: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: 20
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    clip: true
                    color: "#ffffff"
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }
                Image {
                    id: icon
                    width: 12
                    height: 12
                    y: 11
                    visible: hasSub
                    source: subCategorylistView.visible ? "qrc:/image/assets/down.png" : "qrc:/image/assets/right.png"
                }

                MouseArea {
                    width: parent.width
                    height: parent.height
                    onClicked: {
                        categorylistView.currentIndex = index
                        console.log("=====selectedSubIndex111:" + selectedSubIndex
                                    + " selectedThirdIndex:" + selectedThirdIndex
                                    + " subIndex:" + subIndex + " Index:" + index)
                        subCategorylistView.visible = !subCategorylistView.visible
                        datasource.selectSubCategory(index)
                    }
                }
            }

            ListView {
                id: subCategorylistView
                width: 100
                x: 10
                height: visible == true ? contentHeight : 0
                anchors.top: t1.bottom
                spacing: 5
                visible: hasSub ? true : false
                model: subCategorys
                delegate: Rectangle {
                    id: thirdCateBg
                    width: parent.width
                    height: 30
                    focus: true
                    color: selectedThirdIndex == index
                           && selectedSubIndex == subIndex ? "#19ffffff" : "#00ffffff"
                    Text {
                        text: subCategorys[index]
                        width: parent.width
                        height: parent.height
                        clip: true
                        anchors.left: parent.left
                        anchors.leftMargin: 20
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        color: "#BFBFBF"
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                    }

                    MouseArea {
                        width: parent.width
                        height: parent.height
                        onClicked: {
                            datasource.selectThirdCategory(subIndex, index)
                            console.log("=====selectedSubIndex222:" + selectedSubIndex
                                        + " selectedThirdIndex:" + selectedThirdIndex
                                        + " subIndex:" + subIndex + " Index:" + index
                                        + " Select:" + (selectedSubIndex === subIndex && selectedThirdIndex === index))
                        }
                    }
                }
            }
        }

        onCurrentItemChanged: {

            //            console.log("onEntered " + categorylistView.currentItem)
        }
    }

    ToastManager {
        id: storeToastMgr

        anchors.fill: resourceList
    }

    Rectangle {
        id: categorylistViewRightLine
        width: 2
        height: parent.height
        color: "#000000"
        anchors.top: topCategory.bottom
        anchors.left: categorylistView.right
    }

    MouseArea {
        anchors.fill: resourceList
        preventStealing: true
        onClicked: {
            resourceList.focus = false
        }
    }
    GridView {
        id: resourceList
        anchors.margins: 8
        anchors.top: topCategoryBottoLine.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: categorylistViewRightLine.right
        cellWidth: 100
        cellHeight: 120
        clip: true
        model: datasource.resources()
        delegate: StoreResourceItem {}
        property int selectIndex: -1
        ScrollBar.vertical: ScrollBar {
            width: 8
            policy: ScrollBar.AlwaysOn
            visible: parent.contentHeight > parent.height
        }
        onSelectIndexChanged: {
            datasource.selectResourceIndex(selectIndex)
        }
        onFocusChanged: {
            if (!focus) {
                resourceList.selectIndex = -1
                videoLocalList.selectIndex = -1
                audioLocalList.selectIndex = -1
                moreLocalList.selectIndex = -1
            }
        }
    }

    function checkFileExtension(filePath, avaliableExtension) {
        var avaliable = false
        for (var i = 0; i < avaliableExtension.length; i++) {
            avaliable = avaliable || filePath.split('.').pop().toLowerCase(
                        ) === avaliableExtension[i]
        }
        return avaliable
    }

    function fileExtension(filePath) {
        return filePath.split('.').pop().toUpperCase()
    }

    function showMessage(content) {
        storeToastMgr.show(content, 3000)
    }

    function addTransitionFromClip(trackIndex, clipIndex) {
        datasource.selectTopCategory(storeCategoryTransition.cate)
    }

    FileUtils {
        id: fileUtils
    }

    Rectangle {
        id: videoLocalContainer
        anchors.margins: 8
        anchors.top: topCategoryBottoLine.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: categorylistViewRightLine.right
        clip: true
        visible: false
        color: "#15171C"
        property bool videoItemHover: false

        FileDialog {
            id: videoOpenDialog
            title: qsTr("Select an Video")
            folder: shortcuts.documents
            selectMultiple: true
            nameFilters: ["Media files (*.mp4 *.avi *.m4v *.mkv *.mpg *.f4v *.flv *.rmvb *.mov *.rm *.wmv *.3gp *.png *.jpeg *.jpg *.webp *.bmp *.tiff *.gif)"]
            onAccepted: {
                for (var i = 0; i < fileUrls.length; ++i) {
                    datasource.openNewVideo(fileUrls[i])
                }
            }
        }

        DropArea {
            id: dropVideoArea
            anchors.fill: parent
            onDropped: {
                if (drop.hasUrls) {
                    var failed = false
                    for (var i = 0; i < drop.urls.length; i++) {
                        if (fileUtils.isVideoExtension(drop.urls[i])
                                || fileUtils.isImageExtension(drop.urls[i])) {
                            datasource.openNewVideo(drop.urls[i])
                        } else {
                            failed = true
                        }
                    }
                    if (failed) {
                        storeToastMgr.show(qsTr("File Format is Illegal"), 3000)
                    }
                }
            }
        }

        Rectangle {
            id: videoEmptyView
            anchors {
                centerIn: parent
            }
            width: 240
            height: 120
            color: "#00000000"
            visible: datasource.localVideoDataCount() <= 1

            Image {
                width: parent.width
                height: parent.height
                source: videoLocalContainer.videoItemHover ? "qrc:/image/assets/video_add_border_hover.png" : "qrc:/image/assets/video_add_border.png"
            }
            Image {
                width: 48
                height: 48
                source: videoLocalContainer.videoItemHover ? "qrc:/image/assets/video_add_hover.png" : "qrc:/image/assets/video_add.png"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 20
                }
            }
            Text {
                color: videoLocalContainer.videoItemHover ? "#ffffff" : "#BFBFBF"
                text: qsTr("Import Video")
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                    bottomMargin: 20
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: {
                    videoLocalContainer.videoItemHover = true
                }
                onExited: {
                    videoLocalContainer.videoItemHover = false
                }
                onClicked: {
                    videoOpenDialog.open()
                }
            }
        }
        GridView {
            id: videoLocalList
            width: parent.width
            height: parent.height
            cellWidth: 100
            cellHeight: 120
            model: datasource.localVideoResource()
            clip: true
            interactive: !videoMouseSelect.mousePressed
            visible: datasource.localVideoDataCount() > 1
            property int selectIndex: 0
            property var selectList: {
                var list = videoMouseSelect.selectList()
                if (list.indexOf(0) !== -1) {
                    list.splice(list.indexOf(0), 1)
                }
                return list
            }
            property int selectCount: selectList.length
            delegate: StoreVideoItem {
                isOverlap: videoMouseSelect.isOverlap(this)
                isMouseSelected: videoMouseSelect.isMouseSelected(index)
                selectCount: videoLocalList.selectCount
                onIsOverlapChanged: videoMouseSelect.select(index, isOverlap)
                onIsMouseSelectedChanged: {
                    if (isMouseSelected) {
                        videoLocalList.focus = true
                    }
                }
            }
            ScrollBar.vertical: ScrollBar {
                width: 8
                policy: ScrollBar.AlwaysOn
                visible: parent.contentHeight > parent.height
            }
            onSelectIndexChanged: {
                videoMouseSelect.setOnce(selectIndex)
                datasource.selectLocalVideoResourceIndex(selectIndex)
            }
            onSelectCountChanged: {
                if (selectCount !== 1) {
                    selectIndex = -1
                }
            }
            onFocusChanged: {
                if (!focus) {
                    resourceList.selectIndex = -1
                    videoLocalList.selectIndex = -1
                    audioLocalList.selectIndex = -1
                    moreLocalList.selectIndex = -1
                    videoMouseSelect.clear()
                }
            }
            onVisibleChanged: {
                if (!visible && focus) {
                    focus = false
                }
            }
        }
        MouseSelect {
            id: videoMouseSelect
            anchors.fill: videoLocalList
            listWidget: videoLocalList
            visible: videoLocalList.visible
            onDeleteData: {
                timeLineRec.focus = true
                datasource.deleteLocalVideoResource(list)
            }
            onClick: {
                if (!(isCtrl || isShift) && videoLocalList.selectCount === 0) {
                    timeLineRec.focus = true
                }
            }
            onImportData: {
                videoOpenDialog.open()
            }

            onOpenDir: {
                datasource.openVideoDir(index)
            }
        }
    }

    Rectangle {
        id: audioLocalContainer
        anchors.margins: 8
        anchors.top: topCategoryBottoLine.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: categorylistViewRightLine.right
        clip: true
        visible: false
        color: "#15171C"

        property bool audioItemHover: false

        FileDialog {
            id: audioOpenDialog
            title: qsTr("Select an audio")
            folder: shortcuts.documents
            selectMultiple: true
            nameFilters: ["Media files (*.mp3 *.wma *.aac *.ape *.flac *.wav)"] // *.mp3 *.wma *.aac *.ape *.flac *.wav
            onAccepted: {
                for (var i = 0; i < fileUrls.length; ++i) {
                    datasource.openNewAudio(fileUrls[i])
                }
            }
        }

        DropArea {
            id: dropAudioArea
            anchors.fill: parent
            onDropped: {
                if (drop.hasUrls) {
                    var failed = false
                    for (var i = 0; i < drop.urls.length; i++) {
                        if (fileUtils.isAudioExtension(drop.urls[i])) {
                            datasource.openNewAudio(drop.urls[i])
                        } else {
                            failed = true
                        }
                    }
                    if (failed) {
                        showMessage(qsTr("File Format is Illegal"))
                    }
                }
            }
        }

        Rectangle {
            id: audioEmptyView
            anchors {
                centerIn: parent
            }
            width: 240
            height: 120
            color: "#00000000"
            visible: datasource.localAudioDataCount() <= 1

            Image {
                width: parent.width
                height: parent.height
                source: audioLocalContainer.audioItemHover ? "qrc:/image/assets/video_add_border_hover.png" : "qrc:/image/assets/video_add_border.png"
            }

            Image {
                width: 48
                height: 48
                source: audioLocalContainer.audioItemHover ? "qrc:/image/assets/audio_add_hover.png" : "qrc:/image/assets/audio_add.png"
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 20
                }
            }
            Text {
                color: audioLocalContainer.audioItemHover ? "#ffffff" : "#BFBFBF"
                text: qsTr("Import Audio")
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                    bottomMargin: 20
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: {
                    audioLocalContainer.audioItemHover = true
                }
                onExited: {
                    audioLocalContainer.audioItemHover = false
                }
                onClicked: {
                    audioOpenDialog.open()
                }
            }
        }
        GridView {
            id: audioLocalList
            width: parent.width
            height: parent.height
            cellWidth: 100
            cellHeight: 120
            objectName: "FocusStore"
            model: datasource.localAudioResource()
            clip: true
            interactive: !videoMouseSelect.mousePressed
            visible: datasource.localAudioDataCount() > 1
            property int selectIndex: 0
            property var selectList: {
                var list = audioMouseSelect.selectList()
                if (list.indexOf(0) !== -1) {
                    list.splice(list.indexOf(0), 1)
                }
                return list
            }
            property int selectCount: selectList.length

            delegate: StoreAudioItem {
                isOverlap: audioMouseSelect.isOverlap(this)
                isMouseSelected: audioMouseSelect.isMouseSelected(index)
                selectCount: audioLocalList.selectCount
                onIsOverlapChanged: audioMouseSelect.select(index, isOverlap)
                onIsMouseSelectedChanged: {
                    if (isMouseSelected) {
                        audioLocalList.focus = true
                    }
                }
            }
            ScrollBar.vertical: ScrollBar {
                width: 8
                policy: ScrollBar.AlwaysOn
                visible: parent.contentHeight > parent.height
            }
            onSelectIndexChanged: {
                audioMouseSelect.setOnce(selectIndex)
                datasource.selectLocalAudioResourceIndex(selectIndex)
            }
            onSelectCountChanged: {
                if (selectCount !== 1) {
                    selectIndex = -1
                }
            }
            onFocusChanged: {
                if (!focus) {
                    resourceList.selectIndex = -1
                    videoLocalList.selectIndex = -1
                    audioLocalList.selectIndex = -1
                    moreLocalList.selectIndex = -1
                    audioMouseSelect.clear()
                }
            }
            onVisibleChanged: {
                if (!visible && focus) {
                    focus = false
                }
            }
        }
        MouseSelect {
            id: audioMouseSelect
            anchors.fill: audioLocalList
            listWidget: audioLocalList
            visible: audioLocalList.visible
            onDeleteData: {
                timeLineRec.focus = true
                datasource.deleteLocalAudioResource(list)
            }
            onClick: {
                if (!(isCtrl || isShift) && audioLocalList.selectCount === 0) {
                    timeLineRec.focus = true
                }
            }
            onImportData: {
                audioOpenDialog.open()
            }

            onOpenDir: {
                datasource.openAudioDir(index)
            }
        }
    }
    Timer {
        id: previewEffectTimer
        interval: 0
        running: false
        repeat: false
        property int index: 0
        onTriggered: {
            moreLocalList.focus = true
            moreLocalList.selectIndex = index
        }
    }
    Rectangle {
        id: moreLocalContainer
        anchors.margins: 8
        anchors.top: topCategoryBottoLine.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: categorylistViewRightLine.right
        clip: true
        visible: false
        color: "#15171C"
        FileDialog {
            id: moreOpenDialog
            title: qsTr("Select an Effect file")
            nameFilters: ["Effect files (*.ofeffect)"]
            selectFolder: false
            onAccepted: {

                var addCount = 0
                for (var i = 0; i < fileUrls.length; ++i) {
                    if (datasource.openNewMore(fileUrls[i])) {
                        addCount++
                    }
                }

                if (addCount === 0) {
                    showMessage(qsTr("Had Invalid File!"))
                }
            }
        }

        FileDialog {
            id: newEffectDirDialog
            title: qsTr("Select an Effect Dir")
            selectFolder: true
            selectMultiple: false
            selectExisting: true
            onAccepted: {
                datasource.addNewEffect(fileUrl)
                previewEffectTimer.index = datasource.localMoreDataCount() - 1
                previewEffectTimer.start()
            }
        }

        DropArea {
            id: dropMoreArea
            anchors.fill: parent
            onDropped: {
                if (drop.hasUrls) {
                    var failed = false
                    for (var i = 0; i < drop.urls.length; i++) {
                        if (!datasource.openNewMore(drop.urls[i])) {
                            failed = true
                        }
                    }
                    if (failed) {
                        showMessage(qsTr("Had Invalid File!"))
                    }
                }
            }
        }

        Item {
            id: moreEmptyView
            height: 240
            anchors.centerIn: parent
            visible: datasource.localMoreDataCount() <= 1
            Item {
                id: importEffectItem
                width: 200
                height: 100
                anchors.horizontalCenter: parent.horizontalCenter
                property bool moreItemHover: false
                Image {
                    width: parent.width
                    height: parent.height
                    source: importEffectItem.moreItemHover ? "qrc:/image/assets/video_add_border_hover.png" : "qrc:/image/assets/video_add_border.png"
                }
                Image {
                    width: 48
                    height: 48
                    source: importEffectItem.moreItemHover ? "qrc:/image/assets/video_add_hover.png" : "qrc:/image/assets/video_add.png"
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                        topMargin: 20
                    }
                }
                Text {
                    color: importEffectItem.moreItemHover ? "#ffffff" : "#BFBFBF"
                    text: qsTr("Import Effect")
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                        bottomMargin: 20
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: {
                        importEffectItem.moreItemHover = true
                    }
                    onExited: {
                        importEffectItem.moreItemHover = false
                    }
                    onClicked: {
                        moreOpenDialog.open()
                    }
                }
            }

            Item {
                id: newEffectItem
                width: 200
                height: 100
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: importEffectItem.bottom
                anchors.topMargin: 40
                property bool moreItemHover: false
                Image {
                    width: parent.width
                    height: parent.height
                    source: newEffectItem.moreItemHover ? "qrc:/image/assets/video_add_border_hover.png" : "qrc:/image/assets/video_add_border.png"
                }
                Image {
                    width: 48
                    height: 48
                    source: newEffectItem.moreItemHover ? "qrc:/image/assets/video_add_hover.png" : "qrc:/image/assets/video_add.png"
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        top: parent.top
                        topMargin: 20
                    }
                }
                Text {
                    color: newEffectItem.moreItemHover ? "#ffffff" : "#BFBFBF"
                    text: qsTr("Add New Effect")
                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.bottom
                        bottomMargin: 20
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true

                    onEntered: {
                        newEffectItem.moreItemHover = true
                    }
                    onExited: {
                        newEffectItem.moreItemHover = false
                    }
                    onClicked: {
                        newEffectDirDialog.open()
                    }
                }
            }
        }
        GridView {
            id: moreLocalList
            width: parent.width
            height: parent.height
            cellWidth: 100
            cellHeight: 120
            objectName: "FocusStore"
            model: datasource.localMoreResource()
            clip: true
            interactive: !videoMouseSelect.mousePressed
            visible: datasource.localMoreDataCount() > 1
            property int selectIndex: 0
            property var selectList: {
                var list = moreMouseSelect.selectList()
                if (list.indexOf(0) !== -1) {
                    list.splice(list.indexOf(0), 1)
                }
                return list
            }
            property int selectCount: selectList.length
            delegate: StoreMoreItem {
                isOverlap: moreMouseSelect.isOverlap(this)
                isMouseSelected: moreMouseSelect.isMouseSelected(index)
                selectCount: moreLocalList.selectCount
                onIsOverlapChanged: moreMouseSelect.select(index, isOverlap)
                onIsMouseSelectedChanged: {
                    if (isMouseSelected) {
                        moreLocalList.focus = true
                    }
                }
            }
            ScrollBar.vertical: ScrollBar {
                width: 8
                policy: ScrollBar.AlwaysOn
                visible: parent.contentHeight > parent.height
            }
            onSelectIndexChanged: {
                moreMouseSelect.setOnce(selectIndex)
                datasource.selectLocalMoreResourceIndex(selectIndex)
            }
            onSelectCountChanged: {
                if (selectCount !== 1) {
                    selectIndex = -1
                }
            }
            onFocusChanged: {
                if (!focus) {
                    resourceList.selectIndex = -1
                    videoLocalList.selectIndex = -1
                    audioLocalList.selectIndex = -1
                    moreLocalList.selectIndex = -1
                    moreMouseSelect.clear()
                }
            }
            onVisibleChanged: {
                if (!visible && focus) {
                    focus = false
                }
            }
        }
        MouseSelect {
            id: moreMouseSelect
            anchors.fill: moreLocalList
            listWidget: moreLocalList
            isLocalEffectTab: true
            visible: moreLocalList.visible
            onDeleteData: {
                timeLineRec.focus = true
                datasource.deleteLocalMoreResource(list)
            }
            onClick: {
                if (!(isCtrl || isShift) && moreLocalList.selectCount === 0) {
                    timeLineRec.focus = true
                }
            }
            onImportData: {
                moreOpenDialog.open()
            }
            onEditData: {
                console.log("onEditData:" + index)
                datasource.editEffect(index)
                previewEffectTimer.index = index
                previewEffectTimer.start()
            }

            onAddNewData: {
                console.log("onAddNewData")
                newEffectDirDialog.open()
            }

            onOpenDir: {
                datasource.openEffectDir(index)
            }

            onUploadEffect: {
                console.log("onUpload:" + index)

                if (loginController.isLogin()) {
                    var component = Qt.createComponent(
                                "qrc:/ui/dialog/UploadEffectDialog.qml")
                    if (component.status === Component.Error) {
                        console.error("Error:" + component.errorString())
                        return
                        // or maybe throw
                    }
                    var page = component.createObject(this, {
                                                          "effectPath": datasource.localMoreResource(
                                                                            ).filePath(
                                                                            index)
                                                      })
                    page.show()
                } else {
                    var component = Qt.createComponent(
                                "qrc:/ui/dialog/LoginDialog.qml")
                    if (component.status === Component.Error) {
                        console.error("Error:" + component.errorString())
                        return
                        // or maybe throw
                    }
                    var page = component.createObject(this)
                    page.show()
                }
            }
        }
    }

    Component.onCompleted: {
        datasource.loadData()
    }
}
