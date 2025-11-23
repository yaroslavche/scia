import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9

Item {
    id: root
    property var person: null
    Layout.preferredWidth: 200

    Text {
        anchors.fill: parent
        text: root.person.name
    }
}
