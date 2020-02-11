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

import org.kde.kirigami 2.5 as Kirigami

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

    contentItem: Kirigami.FormLayout {

        QQC2.TextField {
            id: channelNameEdit
            visible: roomInfo.canBeModify
            text: roomInfo.name
            Kirigami.FormData.label: i18n("Name:")
            onTextChanged: newText => {
                if (newText != "") {
                    if (roomInfo.name !== newText) {
                        channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Name, newText, roomInfo.channelType)
                    }
                } else {
                    //see https://doc.qt.io/qt-5/qml-qtqml-loggingcategory.html
                    console.log(RuqolaDebugCategorySingleton.category, "New name is empty. We can't rename room name to empty name.")
                }
            }
        }

        QQC2.Label {
            visible: !channelNameEdit.visible
            text: roomInfo.name
            Kirigami.FormData.label: i18n("Name:")
        }

        QQC2.TextField {
            id: channelCommentEdit
            visible: roomInfo.canBeModify
            text: roomInfo.topic
            Kirigami.FormData.label: i18n("Comment:")
            onTextChanged: newText => {
                if (newText === roomInfo.topic)
                    return
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Topic, newText, roomInfo.channelType)
            }
        }

        QQC2.Label {
            visible: !channelCommentEdit.visible
            text: roomInfo.topic
            Kirigami.FormData.label: i18n("Comment:")
        }

        QQC2.TextField {
            id: channelAnnouncementEdit
            visible: roomInfo.canBeModify
            text: roomInfo.announcement
            Kirigami.FormData.label: i18n("Announcement:")
            onTextChanged: newText => {
                if (newText === roomInfo.announcement)
                    return
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Announcement, newText, roomInfo.channelType)
            }
        }

        QQC2.Label {
            visible: !channelAnnouncementEdit.visible
            text: roomInfo.announcement
            Kirigami.FormData.label: i18n("Announcement:")
        }

        QQC2.TextField {
            id: channelDescriptionEdit
            visible: roomInfo.canBeModify
            text: roomInfo.description
            Kirigami.FormData.label: i18n("Description:")
            onTextChanged: newText => {
                if (newText === roomInfo.description)
                    return
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Description, newText, roomInfo.channelType)
            }
        }

        QQC2.Label {
            visible: !channelDescriptionEdit.visible
            text: roomInfo.description
            Kirigami.FormData.label: i18n("Description:")
        }

        PasswordLineEdit {
            id: password
            visible: roomInfo.canBeModify
            //Add i18n context ?
            placeholderText: roomInfo === null ? i18n("Add password") : (roomInfo.joinCodeRequired ? i18n("This Room has a password") : i18n("Add password"))
            Kirigami.FormData.label: i18n("Password:")
        }

        QQC2.Switch {
            id: readOnlyRoom
            visible: roomInfo.canBeModify
            checked: roomInfo.readOnly
            onClicked: {
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.ReadOnly, checked, roomInfo.channelType)
            }
            Kirigami.FormData.label: i18n("Read-Only:")
        }

        QQC2.Switch {
            id: broadcast
            visible: roomInfo.canBeModify
            checked: roomInfo.broadcast
            onClicked: {
                console.log(RuqolaDebugCategorySingleton.category, "Broadcast not implemented yet")
                //TODO
            }
            Kirigami.FormData.label: i18n("Broadcast:")
        }

        QQC2.Switch {
            id: archiveRoom
            visible: roomInfo.canBeModify
            checked: roomInfo.archived
            onClicked: {
                archiveRoomDialog.archive = checked
                archiveRoomDialog.open()
            }
            Kirigami.FormData.label: i18n("Archive:")
        }

        QQC2.Switch {
            id: roomType
            visible: roomInfo.canBeModify
            checked: roomInfo.channelType === "p"
            onClicked: {
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.RoomType, checked, roomInfo.channelType)
            }
            Kirigami.FormData.label: i18n("Private:")
        }

        QQC2.Switch {
            id: encrypted
            visible: roomInfo.canBeModify && roomInfo.encryptedEnabled
            checked: roomInfo.encrypted
            onClicked: {
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Encrypted, checked, roomInfo.channelType)
            }
            Kirigami.FormData.label: i18n("Encrypted:")
        }

        DeleteButton {
            id: deleteButton
            visible: roomInfo.canBeModify
            onClicked: {
                deleteRoomDialog.rId = roomInfo.roomId
                deleteRoomDialog.open();
            }
            Kirigami.FormData.label: i18n("Delete Room:")
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
