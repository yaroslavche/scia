import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9

Item {
    id: overlay
    property int toastDuration: 3000

    signal requestToast(string message, string type)

    anchors.fill: parent

    Column {
        id: toastColumn
        spacing: 8
        anchors {
            right: parent.right
            bottom: parent.bottom
            margins: 16
        }
    }

    Connections {
        target: overlay

        function onRequestToast(msg, type) {
            const comp = Qt.createComponent("UIToast.qml");
            if (comp.status === Component.Ready) {
                const toast = comp.createObject(toastColumn, {
                    message: msg,
                    type: type,
                    duration: overlay.toastDuration
                });
            }
        }
    }
}
