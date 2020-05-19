import QtQuick 2.9
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.9

import "../delegates"

Rectangle {
    id: root

    property var model: null

    signal homeButtonPressed()
    signal directoryButtonPressed()
    signal roomButtonPressed(string roomId)

    color: "#2F343D"
    ColumnLayout {
        anchors.fill: parent

        Rectangle {
            // Menu goes here
            color: "#2F343D"
            Layout.preferredHeight: 68
            Layout.fillWidth: true

            RowLayout {
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 24
                anchors.topMargin: 22
                height: 24
                spacing: 0

                Rectangle {
                    // User icon
                    Layout.preferredHeight: 24
                    Layout.preferredWidth: 24
                    color: "green"
                }

                Button {
                    // Home button
                    Layout.leftMargin: 58
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 20
                    background: Rectangle {
                        anchors.fill: parent
                        color: "#2F343D"
                    }

                    Image {
                        source: _iconsModelQml.shieldIcon
                        sourceSize.width: 20
                        sourceSize.height: 20

                    }
                    onClicked: homeButtonPressed()
                }

                Button {
                    // Search rooms
                    Layout.leftMargin: 16
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 20
                    background: Rectangle {
                        anchors.fill: parent
                        color: "#2F343D"
                    }

                    Image {
                        source: _iconsModelQml.magnifierIcon
                        sourceSize.width: 20
                        sourceSize.height: 20
                    }
                }

                Button {
                    // Directory
                    Layout.leftMargin: 16
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 20
                    background: Rectangle {
                        anchors.fill: parent
                        color: "#2F343D"
                    }

                    Image {
                        source: _iconsModelQml.discoverIcon
                        sourceSize.width: 20
                        sourceSize.height: 20
                    }
                    onClicked: directoryButtonPressed()
                }

                Button {
                    // Sort 
                    Layout.leftMargin: 16
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 20
                    background: Rectangle {
                        anchors.fill: parent
                        color: "#2F343D"
                    }

                    Image {
                        source: _iconsModelQml.sortIcon
                        sourceSize.width: 20
                        sourceSize.height: 20
                    }
                }

                Button {
                    // Create new 
                    Layout.leftMargin: 16
                    Layout.preferredHeight: 20
                    Layout.preferredWidth: 20
                    background: Rectangle {
                        anchors.fill: parent
                        color: "#2F343D"
                    }

                    Image {
                        source: _iconsModelQml.editRoundedIcon
                        sourceSize.width: 20
                        sourceSize.height: 20
                    }
                }
            }
        }

        ScrollView {
            id: roomsScrollView
            Layout.fillHeight: true
            Layout.fillWidth: true
            ScrollBar.vertical.policy: ScrollBar.AlwaysOn
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            clip: true

            ListView {
                spacing: 0
                anchors.fill: parent
                model: root.model
                boundsBehavior: Flickable.StopAtBounds
                delegate: RoomEntryDelegate {
                    width: parent.width - roomsScrollView.ScrollBar.vertical.width
                    roomName: model.name
                    roomType: model.type
                    roomId: model.room_id
                    roomUnread: model.unread
                    roomAlert: model.alert
                    avatarURL: model.type === "d" ? _chatScreenController.avatarUrl(model.directChannelUserId) : ""
                    onClicked: roomButtonPressed(model.room_id)
                    // Connections {
                    //     target: _chatScreenController
                    //     onUsersModelRefreshed: {
                    //         if (avatarURL.length === 0) {
                    //             avatarURL = _chatScreenController.roomAvatarUrl(model.room_id);
                    //         }
                    //     }
                    // }
                }
            }
        }

        Rectangle {
            Layout.preferredHeight: 48
            Layout.fillWidth: true
            color: "#2F343D"

            Image {
                id: logoImage

                // Logo goes here
                anchors.margins: 10
                anchors.fill: parent
                anchors.left: parent.left

                source: _serverSelectorController.logoUrlForServer(_serverSelectorController.selectedServer)
                fillMode: Image.PreserveAspectFit

                Connections {
                    target: _serverSelectorController
                    onSelectedServerChanged: {
                        logoImage.source = _serverSelectorController.logoUrlForServer(_serverSelectorController.selectedServer)
                        faviconUpdate.cnt = 0;
                        faviconUpdate.repeat = true;
                        faviconUpdate.running = true;
                    }
                }

                Timer {
                    id: faviconUpdate

                    property int cnt: 0

                    interval: 1000
                    running: true
                    repeat: true

                    onTriggered: {
                        if (_serverSelectorController.logoUrlForServer(_serverSelectorController.selectedServer) != logoImage.source || cnt == 10) {
                            logoImage.source = _serverSelectorController.logoUrlForServer(_serverSelectorController.selectedServer);
                            repeat = false;
                        }
                        interval = 1000
                        cnt++;
                    }
                }
            }
        }
    }
}
