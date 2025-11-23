import QtQuick 6.9
import QtQuick.Controls 6.9
import QtQuick.Layouts 6.9

Page {
    id: root

    property int loadedPersons: personViewModel.rowCount()

    function randomName() {
        const names = ["John", "Jane", "Michael", "Sarah", "David", "Laura", "Chris", "Anna"];
        const surnames = ["Smith", "Johnson", "Williams", "Brown", "Jones", "Miller", "Davis", "Garcia"];
        const randomNameIndex = Math.floor(Math.random() * names.length);
        const randomName = names[randomNameIndex];
        const randomSurnameIndex = Math.floor(Math.random() * surnames.length);
        const randomSurname = surnames[randomSurnameIndex];
        return randomName + " " + randomSurname;
    }

    ColumnLayout {
        Button {
            text: qsTr("Load person")
            onClicked: () => {
                personViewModel.load()
                root.loadedPersons = personViewModel.rowCount()
            }
        }
        Button {
            text: qsTr("Create random person")
            onClicked: () => {
                personViewModel.createPerson(root.randomName())
                root.loadedPersons = personViewModel.rowCount()
            }
        }
        Repeater {
            model: personViewModel
            delegate: Text {
                text: model.name
            }
        }
        Text {
            text: qsTr("Loaded person count: ") + root.loadedPersons
        }
    }
}
