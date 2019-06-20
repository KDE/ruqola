/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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
import KDE.Ruqola.UsersForRoomFilterProxyModel 1.0
import org.kde.kirigami 2.7 as Kirigami
import "common"
import "messages"
QQC2.Dialog {
    id: showSearchMessageDialog

    title: i18n("Search Message")

    signal searchMessage(string pattern, string rid)

    signal goToMessage(string messageId)

    property QtObject searchMessageModel
    property string roomId
    width: parent.width * 9 / 10
    height: parent.height * 9 / 10
    anchors.centerIn: parent

    modal: true
    focus: true
    standardButtons: QQC2.Dialog.Close

    function initializeAndOpen()
    {
        searchField.text = "";
        searchField.forceActiveFocus();
        open();
    }

    contentItem: ColumnLayout {
        LineEditWithClearButton {
            id: searchField
            placeholderText: i18n("Search Word...")
            Layout.fillWidth: true
            onAccepted: {
                showSearchMessageDialog.searchMessage(text, roomId)
            }
        }

        ListView {
            id: listview
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            // Scrollars
            QQC2.ScrollIndicator.vertical: QQC2.ScrollIndicator { }
            QQC2.ScrollIndicator.horizontal: QQC2.ScrollIndicator { }

            model: searchMessageModel
            delegate: Kirigami.BasicListItem {
                reserveSpaceForIcon: false
                reserveSpaceForLabel: false
                RowLayout {
                    width: parent.width
                    ColumnLayout {
                        //                    AvatarImage {
                        //                        id: avatarRect
                        //                        avatarurl: i_avatar
                        //                        aliasname: i_aliasname
                        //                        username: i_username
                        //                    }
                        QQC2.Label {
                            text: messagetext
                            elide: Text.ElideRight
                            wrapMode: QQC2.Label.Wrap
                        }
                        TimestampText {
                            id: timestampText
                            timestamp: messagetimestamp
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                    }
                }
            }
        }
    }
}
