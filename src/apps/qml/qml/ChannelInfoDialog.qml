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
import Ruqola 1.0


import "common"
QQC2.Dialog {
    id: channelInfoDialog

    title: i18n("Info about this channel")
    standardButtons: QQC2.Dialog.Close

    modal: true
    focus: true

    width: parent.width * 9 / 10
    height: parent.height * 9 / 10
    anchors.centerIn: parent

    property string channelName: ""
    property QtObject roomInfo

    signal modifyChannelSetting(string roomId, int type, var newVal, string channelType)
    signal deleteRoom(string roomId)

    function initializeAndOpen()
    {
        var enabledField = !roomInfo.canBeModify;
        channelNameField.setReadOnly(enabledField);
        channelCommentField.setReadOnly(enabledField);
        channelAnnouncementField.setReadOnly(enabledField);
        channelDescriptionField.setReadOnly(enabledField);
        labelReadOnlyRoom.visible = !enabledField
        readOnlyRoom.visible = !enabledField
        labelArchiveRoom.visible = !enabledField
        archiveRoom.visible = !enabledField
        labelDeleteButton.visible = !enabledField
        deleteButton.visible = !enabledField
        labelRoomType.visible = !enabledField
        roomType.visible = !enabledField
        labelEncrypted.visible = !enabledField && roomInfo.encryptedEnabled
        encrypted.visible = !enabledField && roomInfo.encryptedEnabled
        labelPassword.visible = !enabledField
        password.visible = !enabledField
        labelBroadcast.visible = !enabledField
        broadcast.visible = !enabledField
        //TODO fix me! Readd password
        password.text = "";
        open();
    }

    contentItem: GridLayout {
        columns: 2
        QQC2.Label {
            text: i18n("Name:");
        }
        TextFieldEditor {
            id: channelNameField
            Layout.fillWidth: true
            textField: roomInfo === null ? "" : roomInfo.name

            onUpdateValue: {
                if (newVal != "") {
                    if (roomInfo.name !== newVal) {
                        channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Name, newVal, roomInfo.channelType)
                    }
                } else {
                    //see https://doc.qt.io/qt-5/qml-qtqml-loggingcategory.html
                    console.log(RuqolaDebugCategorySingleton.category, "New name is empty. We can't rename room name to empty name.")
                }
            }
        }
        QQC2.Label {
            text: i18n("Comment:");
        }
        TextFieldEditor {
            id: channelCommentField
            Layout.fillWidth: true
            textField: roomInfo === null ? "" : roomInfo.topic
            onUpdateValue: {
                if (roomInfo.topic !== newVal) {
                    channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Topic, newVal, roomInfo.channelType)
                }
            }
        }
        QQC2.Label {
            text: i18n("Announcement:");
        }
        TextFieldEditor {
            id: channelAnnouncementField
            Layout.fillWidth: true
            textField: roomInfo === null ? "" : roomInfo.announcement;
            onUpdateValue: {
                if (roomInfo.announcement !== newVal) {
                    channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Announcement, newVal, roomInfo.channelType)
                }
            }
        }
        QQC2.Label {
            text: i18n("Description:");
        }
        TextFieldEditor {
            id: channelDescriptionField
            Layout.fillWidth: true
            textField: roomInfo === null ? "" : roomInfo.description;
            onUpdateValue: {
                if (roomInfo.description !== newVal) {
                    channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Description, newVal, roomInfo.channelType)
                }
            }
        }

        QQC2.Label {
            id: labelPassword
            text: i18n("Password:");
        }
        PasswordLineEdit {
            id: password
            Layout.fillWidth: true
            selectByMouse: true
            //Add i18n context ?
            placeholderText: roomInfo === null ? i18n("Add password") : (roomInfo.joinCodeRequired ? i18n("This Room has a password") : i18n("Add password"))
        }

        QQC2.Label {
            id: labelReadOnlyRoom
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
            checked: roomInfo === null ? false : roomInfo.readOnly
            onClicked: {
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.ReadOnly, checked, roomInfo.channelType)
            }
        }

        QQC2.Label {
            id: labelBroadcast
            text: i18n("Broadcast:");
        }
        QQC2.Switch {
            id: broadcast
            checked: roomInfo === null ? false : roomInfo.broadcast
            onClicked: {
                console.log(RuqolaDebugCategorySingleton.category, "Broadcast not implemented yet")
                //TODO
            }
        }

        QQC2.Label {
            id: labelArchiveRoom
            text: i18n("Archive:");
        }
        QQC2.Switch {
            id: archiveRoom
            checked: roomInfo === null ? false : roomInfo.archived
            onClicked: {
                archiveRoomDialog.archive = checked
                archiveRoomDialog.open()
            }
        }

        QQC2.Label {
            id: labelRoomType
            text: i18n("Private:")
        }
        QQC2.Switch {
            id: roomType
            checked: roomInfo === null ? false : roomInfo.channelType === "p"
            onClicked: {
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.RoomType, checked, roomInfo.channelType)
            }
        }

        //TODO hide it if we don't have this support
        QQC2.Label {
            id: labelEncrypted
            text: i18n("Encrypted:")
        }
        QQC2.Switch {
            id: encrypted
            checked: roomInfo === null ? false : roomInfo.encrypted
            onClicked: {
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Encrypted, checked, roomInfo.channelType)
            }
        }

        QQC2.Label {
            id: labelDeleteButton
            text: i18n("Delete Room:");
        }
        DeleteButton {
            id: deleteButton
            onDeleteButtonClicked: {
                deleteRoomDialog.rId = roomInfo.rid
                deleteRoomDialog.open();
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }

    ArchiveRoomDialog {
        id: archiveRoomDialog
        onAccepted: {
            channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Archive, archiveRoomDialog.archive, roomInfo.channelType)
        }
        onRejected: {
            archiveRoom.checked = false
        }
    }

    DeleteRoomDialog {
        id: deleteRoomDialog
        rId: channelName
        onDeleteRoom: {
            channelInfoDialog.deleteRoom(roomId)
            channelInfoDialog.close()
        }
    }
}
