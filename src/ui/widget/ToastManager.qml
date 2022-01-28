import QtQuick 2.0

/**
  * adapted from StackOverflow:
  * http://stackoverflow.com/questions/26879266/make-toast-in-android-by-qml
  * @brief Manager that creates Toasts dynamically
  */
ListView {
    /**
      * Public
      */

    /**
      * @brief Shows a Toast
      *
      * @param {string} text Text to show
      * @param {real} duration Duration to show in milliseconds, defaults to 3000
      */
    function show(text, duration) {
        root.width = text.length * 18;
        if (root.width > 800) {
            root.width = 800;
        }
        model.insert(0, {text: text, duration: duration});
    }

    /**
      * Private
      */

    id: root

    z: Infinity
    spacing: 10
    anchors {
        topMargin: 20
        top: parent.top
        bottom: parent.bottom
        horizontalCenter: parent.horizontalCenter
    }
    verticalLayoutDirection: ListView.TopToBottom

    interactive: false

    displaced: Transition {
        NumberAnimation {
            properties: "y"
            easing.type: Easing.InOutQuad
        }
    }
    
    delegate: Toast {
        Component.onCompleted: {
            if (typeof duration === "undefined") {
                show(text);
            }
            else {
                show(text, duration);
            }
        }
    }

    model: ListModel {id: model}
}
