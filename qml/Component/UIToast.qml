import QtQuick 6.9
import QtQuick.Controls 6.9

Rectangle {
    id: toast
    property alias message: label.text
    property string type: "info"
    property int duration: 3000

    width: 300
    height: label.implicitHeight + 16
    color: type === "error" ? "#ff5555" : "#2196F3"
    opacity: 0.75

    Behavior on opacity {
        NumberAnimation {
            duration: 500
        }
    }

    Label {
        id: label
        anchors {
            left: parent.left
            right: parent.right
            leftMargin: 12
            rightMargin: 12
            verticalCenter: parent.verticalCenter
        }
        wrapMode: Text.WordWrap
        color: "white"
    }

    Rectangle {
        id: progressBar
        height: 4
        width: parent.width
        color: "white"
        anchors.bottom: parent.bottom

        SequentialAnimation {
            running: true
            loops: 1
            PropertyAnimation {
                target: progressBar
                property: "width"
                from: parent.width
                to: 0
                duration: toast.duration
            }
            ScriptAction {
                script: toast.destroy()
            }
        }
    }
}
