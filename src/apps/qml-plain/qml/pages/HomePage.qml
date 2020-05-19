import QtQuick 2.9
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.9

Rectangle {
    id: root

    color: "white"

    Text {
        id: text
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 10
        text: "Some text that should be read from the server"
    }

    Button {
        anchors.top: text.bottom
        anchors.left: text.left
        anchors.topMargin: 20
        text: "settings"
        onClicked: _chatScreenController.goToSettingsPage()
    }
}