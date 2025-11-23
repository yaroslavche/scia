import QtQuick 6.9

Item {
    id: pageRoot

    anchors.fill: parent

    property var routeParams: ({})

    function canLeave() {
        return true;
    }
}
