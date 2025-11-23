import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9

ApplicationWindow {
    id: root
    width: 1200
    height: 800
    visible: true
    title: qsTr("scia")

    Component.onCompleted: {
        UIMessage.messageRaised.connect(uiMessages.requestToast)
    }

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 8

        TopPanel {
        }

        ContentArea {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    UIMessageOverlay {
        id: uiMessages
    }

    Connections {
        target: Router

        function onNavigationError(message) {
            UIMessage.show(qsTr("Navigation error: ") + message, "error");
        }
    }

}
