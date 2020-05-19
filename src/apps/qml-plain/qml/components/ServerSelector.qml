import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12

// import Ruqola 1.0

Item {
    id: root
    property var model

    implicitWidth: 68

    Rectangle {
        anchors.fill: parent
        color: "#2a2f37"

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 25
            anchors.bottomMargin: 25
            spacing: 32

            ButtonGroup { id: serverButtonGroup }

            Repeater {
                model: root.model

                ServerSelectorButton {
                    id: serverButton

                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
                    text: model.siteurl
                    ButtonGroup.group: serverButtonGroup
                    checked: model.index === _serverSelectorController.selectedServer
                    onClicked: _serverSelectorController.select(model.index)

                    imageUrl: _serverSelectorController.faviconUrlForServer(model.index)
                    Timer {
                        id: faviconUpdate

                        property int cnt: 0
                        interval: 1000
                        running: true
                        repeat: true

                        onTriggered: {
                            if (_serverSelectorController.faviconUrlForServer(model.index) != serverButton.imageUrl || cnt == 10) {
                                serverButton.imageUrl = _serverSelectorController.faviconUrlForServer(model.index);
                                repeat = false;
                            }
                            interval = 1000
                            cnt++
                        }
                    }
                }
            }


            Button {
                Layout.preferredHeight: 50
                Layout.fillWidth: true
                checked: _serverSelectorController.selectedServer === _serverSelectorController.count
                onClicked: _serverSelectorController.select(_serverSelectorController.count)

                background: Rectangle {
                    color: "#272c34"
                    height: 50
                    width: 50
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Rectangle {
                    color: parent.checked ? "white" : "#76797e"
                    height: 3
                    width: 24
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }

                Rectangle {
                    color: parent.checked ? "white" : "#76797e"
                    height: 24
                    width: 3
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                }

                ButtonGroup.group: serverButtonGroup
            }

            Item {
                id: layoutFiller
                Layout.fillHeight: true
            }
        }
    }
}
