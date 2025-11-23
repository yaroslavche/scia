import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9

// TODO: ListPage.qml { model, delegate }
Page {
    id: root
    ScrollView {
        id: scroller
        anchors.fill: parent
        clip: true
        padding: 8

        ColumnLayout {
            id: list
            anchors.fill: parent
            width: scroller.width

            Repeater {
                model: personViewModel
                delegate: RowLayout {
                    id: personRow
                    Person {
                        person: model
                        width: list.width
                        height: 20
                    }
                    Button {
                        id: removePersonButton
                        Layout.margins: 8
                        text: personRow.hevered || removePersonButton.hovered ? "❌" : "✖"
                        onClicked: () => {
                            personViewModel.deletePerson(model.id)
                        }
                    }
                }
            }
        }
    }
}
