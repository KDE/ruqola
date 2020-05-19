import QtQuick 2.9
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.9

Rectangle {
    id: root

    color: "white"

    CheckBox {
        id: compactModeCheck
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 10
        checked: _settings.compactMode
        text: "Enable compact mode"
        checkable: false
        onClicked: _settings.setCompactMode(!_settings.compactMode)
    }

    Button {
        anchors.left: compactModeCheck.left
        anchors.top: compactModeCheck.bottom
        anchors.topMargin: 5
        onClicked: _settings.reset()
        text: "Reset settings"
    }
}