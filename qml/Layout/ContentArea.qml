import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9

Rectangle {
    id: contentArea
    Layout.fillWidth: true
    Layout.fillHeight: true
    radius: 4

    PageLoader {
        id: loader
        anchors.fill: parent
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.margins: 8
        currentRoute: Router ? Router.currentRoute : null
    }
}
