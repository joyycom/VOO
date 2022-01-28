import QtQuick 2.12
import QmlUtils 1.0
import "../basicmenubar"

Item {
    //1是真实数据，2是临时数据
    property Item listWidget
    property var helpSelectData: new Map()
    property var selectData: new Map()
    property int lastSelectIndex: 0
    property bool selectedRefresh: false
    property bool mousePressed: mouseArea.pressed
    property rect selectRect: Qt.rect(0, 0, 0, 0)
    property bool isLocalEffectTab: false
    signal click(var isCtrl, var isShift)
    signal execDelete
    signal deleteData(var list)
    signal addNewData()
    signal importData()
    signal openDir(var index)
    signal uploadEffect(var index);
    signal editData(var index)

    onExecDelete: {
        var list = selectList()
        if (list.length > 0) {
            clear()
            selectedRefresh = !selectedRefresh
            deleteData(list)
        }
    }
    function clearInner() {
        mouseRect.visible = false
        selectRect.width = 0
        selectRect.height = 0
        selectRect.x = 0
        selectRect.y = 0
        helpSelectData.clear()
        selectData.clear()


    }
    function clear() {
        clearInner()
        selectedRefresh = !selectedRefresh
    }
    function setOnce(index) {
        clearInner()
        if (index > 0) {
            helpSelectData.set(index, true)
            selectData.set(index, true)
            selectedRefresh = !selectedRefresh
        }
    }
    function isOverlap(item) {
        if (item.width === 0 || item.height === 0) {
            return false
        }
        return item.x + item.width > selectRect.x
                && selectRect.x + selectRect.width > item.x
                && item.y + item.height > selectRect.y
                && selectRect.y + selectRect.height > item.y
    }
    function isMouseSelected(index) {
        // @disable-check M127
        selectedRefresh
        return selectData.has(index) ? selectData.get(index) : false
    }
    function select(index, isOverlap) {
        var isCtrl = qmlUtils.keyboardModifiers() & Qt.ControlModifier
        var isShift = qmlUtils.keyboardModifiers() & Qt.ShiftModifier
        if (isShift && !isOverlap) {
            return
        }
        if (isCtrl && isShift) {
            //Ctrl+Shift，最后选择和Shift后的选择范围，不取消掉不在范围内的item
            var min = Math.min(lastSelectIndex, index)
            var max = Math.max(lastSelectIndex, index)
            selectData.clear()
            for (var i = min; i <= max; i++) {
                selectData.set(i, true)
            }
            for (var [key, value] of helpSelectData) {
                if (value) {
                    selectData.set(key, value)
                }
            }
        } else if (isCtrl) {
            //Ctrl框选，异或
            var lastSelected
            if (!helpSelectData.has(index)) {
                lastSelected = false
            } else {
                lastSelected = helpSelectData.get(index)
            }
            selectData.set(index, isOverlap ^ lastSelected)
            if (isOverlap) {
                lastSelectIndex = index
            }
        } else if (isShift) {
            //Shift框选，最后选择和Shift后的选择范围，取消掉不在范围内的item
            min = Math.min(lastSelectIndex, index)
            max = Math.max(lastSelectIndex, index)
            selectData.clear()
            for (i = min; i <= max; i++) {
                selectData.set(i, true)
            }
        } else {
            selectData.set(index, isOverlap)
            if (isOverlap) {
                lastSelectIndex = index
            }
        }
        selectedRefresh = !selectedRefresh
    }
    function selectAll(count) {
        selectData.clear()
        for (var i = 0; i < count; i++) {
            selectData.set(i, true)
        }
        selectedRefresh = !selectedRefresh
    }
    function selectList() {
        // @disable-check M127
        selectedRefresh
        var list = []
        for (var [key, value] of selectData) {
            if (value) {
                list.push(key)
            }
        }
        return list
    }
    QmlUtils {
        id: qmlUtils
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        propagateComposedEvents: true
        property point point1
        property point point2
        anchors.rightMargin: 10
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onPressed: {
            if (mouse.button !== Qt.LeftButton) {
                return
            }
            var isCtrl = qmlUtils.keyboardModifiers() & Qt.ControlModifier
            var isShift = qmlUtils.keyboardModifiers() & Qt.ShiftModifier
            var itemX = mouseX + listWidget.contentX
            var itemY = mouseY + listWidget.contentY
            var item = listWidget.itemAt(itemX, itemY)
            var index = listWidget.indexAt(itemX, itemY)
            if (!(isCtrl || isShift) && item) {
                lastSelectIndex = index
                mouse.accepted = false
                return
            }
            selectData.clear()
            if (isCtrl || isShift) {
                for (var [key, value] of helpSelectData) {
                    selectData.set(key, value)
                }
            } else {
                helpSelectData.clear()
            }
            point1 = Qt.point(mouseX, mouseY)
            point2 = Qt.point(mouseX, mouseY)
            mouseRect.visible = true
            selectRect.width = 0
            selectRect.height = 0
            selectRect.x = itemX
            selectRect.y = itemY
            mouseRect.width = 0
            mouseRect.height = 0
            select(index, item ? item.isOverlap : false)
        }
        onPositionChanged: {
            if (mouse.buttons !== Qt.LeftButton) {
                return
            }
            point2 = Qt.point(mouseX, mouseY)
            mouseRect.x = Math.min(point1.x, point2.x)
            mouseRect.y = Math.min(point1.y, point2.y)
            mouseRect.width = Math.abs(point1.x - point2.x)
            mouseRect.height = Math.abs(point1.y - point2.y)
            selectRect.width = Math.abs(point1.x - point2.x)
            selectRect.height = Math.abs(point1.y - point2.y)
            selectRect.x = Math.min(point1.x, point2.x) + listWidget.contentX
            selectRect.y = Math.min(point1.y, point2.y) + listWidget.contentY
        }
        onReleased: {
            if (mouse.button !== Qt.LeftButton) {
                return
            }
            point2 = Qt.point(mouseX, mouseY)
            mouseRect.visible = false
            selectRect.width = Math.abs(point1.x - point2.x)
            selectRect.height = Math.abs(point1.y - point2.y)
            selectRect.x = Math.min(point1.x, point2.x) + listWidget.contentX
            selectRect.y = Math.min(point1.y, point2.y) + listWidget.contentY
            var isCtrl = qmlUtils.keyboardModifiers() & Qt.ControlModifier
            var isShift = qmlUtils.keyboardModifiers() & Qt.ShiftModifier
            helpSelectData.clear()
            for (var [key, value] of selectData) {
                helpSelectData.set(key, value)
            }
            click(isCtrl, isShift)
        }
        onClicked: {
            if (mouse.button !== Qt.RightButton) {
                return
            }
            var itemX = mouseX + listWidget.contentX
            var itemY = mouseY + listWidget.contentY
            var index = listWidget.indexAt(itemX, itemY)

            for (var [key, value] of selectData) {
                if (value && key === index) {
                    showOptMenu()
                    return
                }
            }
            console.log("==========setOnce:"+selectData.size + " "+index)

            setOnce(index)
            showOptMenu()
            console.log("==========showOptMenu:"+selectData.size + " "+index)

        }
    }
    Rectangle {
        id: mouseRect
        color: "#330099FF"
        border.width: 1
        border.color: "#CC0099FF"
        visible: false
    }


    function showOptMenu(){
        if(editMenuItem !== null){
            editMenuItem.enabled = selectData.size === 1
        }
        if(uploadEffectItem !== null){
            uploadEffectItem.enabled = selectData.size > 0
        }
        openDirItem.enabled = selectData.size === 1
        deleteMenuItem.enabled = selectData.size > 0

        optMenu.popup()
    }


    BasicMenu {
        id: optMenu
        implicitHeight: count*30
        implicitWidth: 100

        BasicMenuItem {
            id:importEffectItem
            text: qsTr("Import")
            height: 30
            width: 100
            implicitHeight: 30
            implicitWidth: 100
            font.pointSize: 10
            onTriggered: {
                importData()
            }
        }

        BasicMenuItem {
            id:addEffectItem
            text: qsTr("Add New Effect")
            height: 30
            width: 100
            implicitHeight: 30
            implicitWidth: 100
            font.pointSize: 10
            visible: false
            onTriggered: {
                addNewData()
            }
        }

        BasicMenuItem {
            id:editMenuItem
            text: qsTr("Edit")
            height: 30
            width: 100
            implicitHeight: 30
            implicitWidth: 100
            font.pointSize: 10
            enabled: selectData.size === 1
            onTriggered: {
                console.log("editData:"+selectData.size)

                if(selectData.size === 0){
                    return;
                }
                var selectedIndex = selectData.keys().next().value
                editData(selectedIndex)

                clear()
            }
        }

        BasicMenuItem {
            id:openDirItem
            text: qsTr("Open Dir")
            height: 30
            width: 100
            implicitHeight: 30
            implicitWidth: 100
            font.pointSize: 10
            onTriggered: {
                console.log("openDir:"+selectData.size)

                if(selectData.size === 0){
                    return;
                }
                var selectedIndex = selectData.keys().next().value
                openDir(selectedIndex)
                clear()
            }
        }

        BasicMenuItem {
            id:deleteMenuItem
            text: qsTr("Delete")
            height: 30
            width: 100
            implicitHeight: 30
            implicitWidth: 100
            font.pointSize: 10
            enabled: selectData.size > 0
            onTriggered: {
                execDelete()
            }
        }

        BasicMenuItem {
            id:uploadEffectItem
            text: qsTr("Upload Effect")
            height: 30
            width: 100
            implicitHeight: 30
            implicitWidth: 100
            font.pointSize: 10
            visible: true
            onTriggered: {
                console.log("==========Upload Effect:"+selectData.size)

                if(selectData.size === 0){
                    return;
                }
                var selectedIndex = selectData.keys().next().value
                uploadEffect(selectedIndex)
                clear()
            }
        }
    }

    Component.onCompleted: {
        if(isLocalEffectTab){
            addEffectItem.visible = true;
            editMenuItem.visible = true;
            optMenu.implicitHeight = 180
        }else{
            optMenu.removeItem(addEffectItem)
            optMenu.removeItem(editMenuItem)
            optMenu.removeItem(uploadEffectItem)
            optMenu.implicitHeight = 90
        }
    }

}
