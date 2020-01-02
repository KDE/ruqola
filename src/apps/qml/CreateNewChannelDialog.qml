/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

import QtQuick 2.9
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Window 2.2

QQC2.Dialog {
    id: createNewChannelDialog

    signal createNewChannel(string name, bool readOnly, bool privateRoom, string usernames, bool encryptedRoom, string password, bool broadcast)

    title: i18n("Create Channel")

    standardButtons: QQC2.Dialog.Ok | QQC2.Dialog.Cancel

    width: parent.width * 9 / 10
    height: parent.height * 9 / 10
    anchors.centerIn: parent

    modal: true
    focus: true
    property bool encryptedRoomEnabled
    function initializeAndOpen()
    {
        channelName.text = "";
        userList.text = "";
        password.text = "";
        readOnlyRoom.checked = false
        privateRoom.checked = false
        encryptedRoom.checked = false
        broadcast.checked = false
        encryptedRoom.visible = encryptedRoomEnabled
        encryptedLabelRoom.visible = encryptedRoomEnabled
        open()
    }

    contentItem: GridLayout {
        columns: 2
        QQC2.Label {
            text: i18n("Name:");
        }
        QQC2.TextField {
            id: channelName
            Layout.fillWidth: true
            selectByMouse: true
            placeholderText: i18n("Channel Name")
        }

        QQC2.Label {
            text: i18n("Users:");
        }
        //TODO add model for searching users
        QQC2.TextField {
            id: userList
            Layout.fillWidth: true
            selectByMouse: true
            placeholderText: i18nc("List of users separated by ','", "User separate with ','")
        }

        QQC2.Label {
            text: i18n("Read-Only:");
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton | Qt.LeftButton
                hoverEnabled: true
                QQC2.ToolTip {
                    id: tooltipReact
                    text: i18n("Only Authorized people can write.")
                }
            }
        }
        QQC2.Switch {
            id: readOnlyRoom
            checked: false
        }
        QQC2.Label {
            text: i18n("Broadcast:");
        }
        QQC2.Switch {
            id: broadcast
            checked: false
        }
        QQC2.Label {
            text: i18n("Private:");
        }
        QQC2.Switch {
            id: privateRoom
            checked: false
        }
        QQC2.Label {
            id: encryptedLabelRoom
            text: i18n("Encrypted:");
        }
        QQC2.Switch {
            id: encryptedRoom
            checked: false
        }

        QQC2.Label {
            text: i18n("Password:");
        }
        PasswordLineEdit {
            id: password
            Layout.fillWidth: true
            selectByMouse: true
            placeholderText: i18n("Add password")
        }
        Item {
            Layout.fillHeight: true
        }
    }

    onAccepted: {
        if (channelName !== "") {
            createNewChannelDialog.createNewChannel(channelName.text, readOnlyRoom.checked, privateRoom.checked, userList.text, encryptedRoom.checked, password.text, broadcast.checked)
        } else {
            console.log(RuqolaDebugCategorySingleton.category, "Channel name is empty!")
        }
    }
}
