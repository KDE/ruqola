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
import org.kde.kirigami 2.8 as Kirigami
QQC2.Dialog {
    id: showDiscussionsInRoomDialog

    title: i18n("Discussions")

    signal openDiscussion(string discussionId)

    property QtObject discussionsModel
    property string roomId
    width: parent.width * 9 / 10
    height: parent.height * 9 / 10
    anchors.centerIn: parent

    modal: true
    focus: true
    standardButtons: QQC2.Dialog.Close

    function initializeAndOpen()
    {
        discussionsModel.clearFilter();
        searchField.text = "";
        searchField.forceActiveFocus();
        open();
    }

    contentItem: ColumnLayout {
        Kirigami.SearchField {
            id: searchField
            placeholderText: i18n("Search Discussions...")
            Layout.fillWidth: true
            onTextChanged: {
                discussionsModel.setFilterString(text);
            }
        }

        SearchLabel {
            hasFullList: discussionsModel.hasFullList
            numberOfElements: listview.count
            labelText: listview.count === 0 ? i18n("No Discussion found") : i18np("%1 discussion in room (Total: %2)", "%1 discussions in room (Total: %2)", listview.count, discussionsModel.total())
            onLoadMoreElements: {
                appid.rocketChatAccount.loadMoreDiscussions(roomId)
            }
        }
        ListView {
            id: listview
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            // Scrollars
            QQC2.ScrollIndicator.vertical: QQC2.ScrollIndicator { }
            QQC2.ScrollIndicator.horizontal: QQC2.ScrollIndicator { }
            onDragEnded : {
                if (roomId !== "" && listview.atYBeginning ) {
                    appid.rocketChatAccount.loadMoreDiscussions(roomId)
                }
            }

            model: discussionsModel
            delegate: DiscussionMessageDelegate {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.rightMargin: Kirigami.Units.largeSpacing
                anchors.leftMargin: Kirigami.Units.largeSpacing

                i_numberofmessages: numberofmessages
                i_lastmessage: lastmessage
                i_discussionid: discussionid
                i_description: description
                i_timestamp: timestamp
                onOpenDiscussion: {
                    showDiscussionsInRoomDialog.openDiscussion(messageDiscussionId)
                }
            }
        }
    }
}
