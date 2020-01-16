/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Window 2.2
import QtQuick 2.9
import org.kde.kirigami 2.7 as Kirigami
QQC2.Dialog {
    id: listMessagesDialogBase

    property QtObject listMessagesModel
    property string roomId
    width: parent.width * 9 / 10
    height: parent.height * 9 / 10
    anchors.centerIn: parent

    title: listMessagesModel.title

    onLoadMoreMessage: {
        appid.rocketChatAccount.loadMoreListMessages(roomId)
    }

    modal: true
    focus: true
    standardButtons: QQC2.Dialog.Close

    signal loadMoreMessage(string roomId)

    function initializeAndOpen()
    {
        listMessagesModel.clearFilter();
        searchField.text = "";
        searchField.forceActiveFocus();
        open();
    }

    contentItem: ColumnLayout {
        LineEditWithClearButton {
            id: searchField
            placeholderText: i18n("Search Messages...")
            Layout.fillWidth: true
            onTextChanged: {
                listMessagesModel.setFilterString(text);
            }
        }

        SearchLabel {
            hasFullList: listMessagesModel.hasFullList
            numberOfElements: listview.count
            labelText: listview.count === 0 ? i18n("No Messages found") : i18np("%1 Message in room (Total: %2)", "%1 Messages in room (Total: %2)", listview.count, listMessagesModel.total)
            onLoadMoreElements: {
                listMessagesDialogBase.loadMoreMessage(roomId)
            }
        }
        Item {
            Layout.fillHeight: true
            visible: listview.count === 0
        }

        ActiveChat {
            id: listview
            onDragEnded : {
                if (roomId !== "" && listview.atYBeginning ) {
                    listMessagesDialogBase.loadMoreMessage(roomId)
                }
            }
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: listMessagesModel
            rcAccount: appid.rocketChatAccount
            roomId: appid.selectedRoomID // ??????
            clip: true
            useMenuMessage: false

            QQC2.ScrollBar.vertical: QQC2.ScrollBar { }
        }
    }
}
