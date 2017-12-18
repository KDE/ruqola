/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.0

import KDE.Ruqola.RocketChatAccount 1.0

Dialog {
    id: channelInfoDialog

    property string channelName: ""

    signal modifyChannelSetting(string roomId, int type, string newVal)

    title: i18n("Info about this channel")

    standardButtons: Dialog.Close

    signal deleteRoom(string roomId)

    modal: true
    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2


    function initializeAndOpen()
    {
        channelNameField.clear();
        channelCommentField.clear();
        channelAnnoucementField.clear();
        channelDescriptionField.clear();
        open();
    }

    GridLayout {
        columns: 2
        Label {
            text: i18n("name:");
        }
        TextFieldEditor {
            id: channelNameField
            onUpdateValue: {
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Name, channelNameField.textInfo)
            }
        }
        Label {
            text: i18n("Comment:");
        }
        TextFieldEditor {
            id: channelCommentField
            onUpdateValue: {
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Topic, channelNameField.textInfo)
            }
        }
        Label {
            text: i18n("Annoucement:");
        }
        TextFieldEditor {
            id: channelAnnoucementField
            onUpdateValue: {
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Annoucement, channelNameField.textInfo)
            }
        }
        Label {
            text: i18n("Description:");
        }
        TextFieldEditor {
            id: channelDescriptionField
            onUpdateValue: {
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Description, channelNameField.textInfo)
            }
        }
    }

    DeleteRoomDialog {
        id: deleteRoomDialog
        rId: channelName
        onDeleteRoom: {
            channelInfoDialog.deleteRoom(roomId)
        }
    }
}
