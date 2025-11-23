import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9

Rectangle {
    id: topPanel
    Layout.fillWidth: true
    height: 64
    radius: 4

    RowLayout {
        id: menuItems
        anchors.fill: parent
        anchors.topMargin: 8
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        anchors.bottomMargin: 8
        spacing: 8

        Repeater {
            model: QmlRouterManager.menuItems
            delegate: Button {
                id: menuItemDelegate
                Layout.preferredHeight: 32
                text: modelData.title
                font.pointSize: 14
                background: Rectangle {
                    radius: 8
                    border.color: "gray"
                    border.width: 1
                }
                contentItem: Text {
                    text: parent.text
                    font: parent.font
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                onClicked: {
                    Router.navigateTo(modelData.page)
                }
            }
        }
        Item {
            Layout.fillWidth: true
        }
        Button {
            text: "Back"
            enabled: Router.canGoBack
            onClicked: Router.goBack()
        }
    }
}
