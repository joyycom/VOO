import QtQuick 2.12
import QtQuick.Controls 2.12

Repeater {
    model: asyncProcessModel
    Rectangle {
        width: {
            if (asyncEndTime == -1) {
                return parent.width - asyncStartTime * timeValue
            } else {
                return (asyncEndTime - asyncStartTime) * timeValue
            }
        }
        height: parent.height
        x: asyncStartTime * timeValue
        color: "#80000000"
    }
}
