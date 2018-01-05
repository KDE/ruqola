/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Window 2.0
import KDE.Ruqola.DebugCategory 1.0
import KDE.Ruqola.RocketChatAccount 1.0

QQC2.Dialog {
    id: channelInfoDialog

    title: i18n("Info about this channel")
    standardButtons: Dialog.Close

    modal: true
    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    property string channelName: ""

    signal modifyChannelSetting(string roomId, int type, var newVal)
    signal deleteRoom(string roomId)
    signal readOnly(bool roomIsReadOnly)
    signal archive(bool roomIsArchived)

    function initializeAndOpen()
    {
        channelNameField.clear();
        channelCommentField.clear();
        channelAnnoucementField.clear();
        channelDescriptionField.clear();
        archiveRoom.checked = false;
        readOnlyRoom.checked = false;
        open();
    }

    GridLayout {
        columns: 2
        QQC2.Label {
            text: i18n("name:");
        }
        TextFieldEditor {
            id: channelNameField
            onUpdateValue: {
                if (newVal != "") {
                    channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Name, newVal)
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
            onUpdateValue: {
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Topic, newVal)
            }
        }
        QQC2.Label {
            text: i18n("Annoucement:");
        }
        TextFieldEditor {
            id: channelAnnoucementField
            onUpdateValue: {
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Annoucement, newVal)
            }
        }
        QQC2.Label {
            text: i18n("Description:");
        }
        TextFieldEditor {
            id: channelDescriptionField
            onUpdateValue: {
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Description, newVal)
            }
        }

        QQC2.Label {
            text: i18n("Read-Only:");
        }
        QQC2.Switch {
            id: readOnlyRoom
            checked: false
            onClicked: {
                channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.ReadOnly, checked)
            }
        }

        QQC2.Label {
            text: i18n("Archive:");
        }
        QQC2.Switch {
            id: archiveRoom
            checked: false
            onClicked: {
                archiveRoomDialog.open()
            }
        }

        ToolButton {
            //visible if user is owner of room
            iconName: "edit-delete.svg"
            onClicked: {
                deleteRoomDialog.open();
            }
        }
    }

    ArchiveRoomDialog {
        id: archiveRoomDialog
        onArchiveRoom: {
            channelInfoDialog.modifyChannelSetting(channelName, RocketChatAccount.Archive, true)
        }
        //TODO switch off if we cancel it.
    }

    DeleteRoomDialog {
        id: deleteRoomDialog
        rId: channelName
        onDeleteRoom: {
            channelInfoDialog.deleteRoom(roomId)
        }
    }
}
