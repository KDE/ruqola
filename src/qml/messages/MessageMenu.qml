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

import org.kde.kirigami 2.1 as Kirigami
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Layouts 1.1
import KDE.Ruqola.DebugCategory 1.0
QQC2.Menu {
    id: menu
    property bool can_editing_message
    property bool starred

    function updateFavoriteLabelText()
    {
        return (i_starred === true) ? i18n("Remove as Favorite") : i18n("Set as Favorite")
    }

    QQC2.MenuItem {
        id: editMessageItem
        contentItem: QQC2.Label {
            text: i18n("Edit")
        }
        onTriggered: {
            messageMain.editMessage(i_messageID);
            console.log(RuqolaDebugCategorySingleton.category, "Edit", i_messageID, i_messageText);
            console.log(RuqolaDebugCategorySingleton.category, "User", i_own_username, i_username);
        }
    }
    QQC2.MenuItem {
        contentItem: QQC2.Label {
            text: i18n("Reply")
        }
        onTriggered: {
            console.log(RuqolaDebugCategorySingleton.category, "Reply to", i_messageID);
            messageMain.replyMessage(i_messageID);
        }
    }
    QQC2.MenuItem {
        contentItem: QQC2.Label {
            id: favoriteLabel
            text: updateFavoriteLabelText()
        }
        onTriggered: {
            console.log(RuqolaDebugCategorySingleton.category, "Set as favorite", i_messageID);
            messageMain.setFavoriteMessage(i_messageID);
        }
    }
    QQC2.MenuItem {
        visible: i_username === i_own_username
        contentItem: QQC2.Label {
            text: i18n("Delete")
        }
        onTriggered: {
            messageMain.deleteMessage(i_messageID);
        }
    }
    onAboutToShow: {
        editMessageItem.visible = (i_username === i_own_username) && rcAccount.allowEditingMessages() && can_editing_message
        favoriteLabel.text = updateFavoriteLabelText()
    }
}
