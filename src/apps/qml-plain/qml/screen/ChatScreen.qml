import QtQuick 2.9
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.9

import Ruqola 1.0

import "../delegates"
import "../components"

Rectangle {
    id: root

    anchors.fill: parent

    RowLayout {
        anchors.fill: parent

        RoomsListView {
            Layout.preferredWidth: 293
            Layout.fillHeight: true
            visible: root.width >= 780
            model: _chatScreenController.roomsModel
            onHomeButtonPressed: _chatScreenController.goToHomePage()
            onDirectoryButtonPressed: _chatScreenController.goToDirectoryPage()
            onRoomButtonPressed: function (roomId) {
                _chatScreenController.selectRoom(roomId);
                _chatScreenController.goToChatPage();
            }
        }

        Loader {
            Layout.fillWidth: true
            Layout.fillHeight: true
            source: _chatScreenController.currentPageUrl

            // Connections for the chat page
            Connections {
                enabled: _chatScreenController.currentPage == ChatScreenController.ChatPage
            }
        }
    }
}