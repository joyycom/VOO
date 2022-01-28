import QtQuick 2.0

/**
 * adapted from StackOverflow:
 * http://stackoverflow.com/questions/26879266/make-toast-in-android-by-qml
 */

/**
  * @brief An Android-like timed message text in a box that self-destroys when finished if desired
  */
Rectangle {

    /**
      * Public
      */

    /**
      * @brief Shows this Toast
      *
      * @param {string} text Text to show
      * @param {real} duration Duration to show in milliseconds, defaults to 3000
      */
    function show(text, duration) {
        message.text = text;
        if (typeof duration !== "undefined") { // checks if parameter was passed
            time = Math.max(duration, 2 * fadeTime);
        }
        else {
            time = defaultTime;
        }
        animation.start();
    }

    property bool selfDestroying: false  // whether this Toast will self-destroy when it is finished

    /**
      * Private
      */

    id: root

    readonly property real defaultTime: 3000
    property real time: defaultTime
    readonly property real fadeTime: 300

    property real margin: 50

    anchors {
        left: parent.left
        right: parent.right
        margins: margin
    }

    height: message.height + margin
    radius: 10

    opacity: 0
    color: "#555555"

    Text {
        id: message
        color: "white"
        font.pixelSize: 18
        verticalAlignment: Text.AlignVCenter
        wrapMode: Text.Wrap
        horizontalAlignment: Text.AlignHCenter
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: margin / 2
        }
    }

    SequentialAnimation on opacity {
        id: animation
        running: false


        NumberAnimation {
            to: .9
            duration: fadeTime
        }

        PauseAnimation {
            duration: time - 2 * fadeTime
        }

        NumberAnimation {
            to: 0
            duration: fadeTime
        }

        onRunningChanged: {
            if (!running && selfDestroying) {
                root.destroy();
            }
        }
    }
}
