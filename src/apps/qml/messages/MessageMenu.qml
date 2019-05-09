/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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

import org.kde.kirigami 2.7 as Kirigami
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Layouts 1.12
import KDE.Ruqola.DebugCategory 1.0
QQC2.Menu {
    id: menu
    property bool can_editing_message
    property bool user_ignored
    property bool starred
    property bool pinned_message

    function updateFavoriteLabelText()
    {
        return (starred === true) ? i18n("Remove as Favorite") : i18n("Set as Favorite")
    }

    function updateIgnoreLabelText()
    {
        return (user_ignored === true) ? i18n("Unignore") : i18n("Ignore")
    }

    function updatePinnedMessageLabelText()
    {
        return (pinned_message === true) ?  i18n("Unpin Message") : i18n("Pin Message")
    }

    //Need to hidding them when server is not 1.0.0
    QQC2.MenuItem {
        id: startADiscussionItem
        visible: rcAccount.discussionEnabled()
        contentItem: QQC2.Label {
            text: i18n("Start a Discussion")
        }
        onTriggered: {
            messageMain.createDiscussion(i_messageID);
            console.log(RuqolaDebugCategorySingleton.category, "Create discussion: messageId: ", i_messageID);
        }
    }
    QQC2.MenuItem {
        id: startAThreadItem
        visible: rcAccount.threadsEnabled()
        contentItem: QQC2.Label {
            text: i18n("Reply in Thread")
        }
        onTriggered: {
//            messageMain.copyMessage(i_messageID, i_originalMessage);
//            console.log(RuqolaDebugCategorySingleton.category, "Copy", i_messageID, i_originalMessage);
        }
    }

    QQC2.MenuItem {
        id: copyMessageItem
        contentItem: QQC2.Label {
            text: i18n("Copy")
        }
        onTriggered: {
            messageMain.copyMessage(i_messageID, i_originalMessage);
            console.log(RuqolaDebugCategorySingleton.category, "Copy", i_messageID, i_originalMessage);
        }
    }

    QQC2.MenuItem {
        id: editMessageItem
        contentItem: QQC2.Label {
            text: i18n("Edit")
        }
        onTriggered: {
            messageMain.editMessage(i_messageID, i_originalMessage);
            console.log(RuqolaDebugCategorySingleton.category, "Edit", i_messageID, i_originalMessage);
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
        id: starredMessageItem
        contentItem: QQC2.Label {
            id: favoriteLabel
            text: updateFavoriteLabelText()
        }
        onTriggered: {
            console.log(RuqolaDebugCategorySingleton.category, "Set as favorite", i_messageID);
            messageMain.setFavoriteMessage(i_messageID, !starred);
        }
    }

    QQC2.MenuItem {
        id: pinnedMessageItem
        contentItem: QQC2.Label {
            id: pinnedMessageLabel
            text: updatePinnedMessageLabelText()
        }
        onTriggered: {
            messageMain.pinMessage(i_messageID, !pinned_message);
        }
    }

    QQC2.MenuItem {
        id: deleteMessageItem
        visible: i_username === i_own_username && rcAccount.allowMessageDeletingEnabled()
        contentItem: QQC2.Label {
            text: i18n("Delete")
        }
        onTriggered: {
            messageMain.deleteMessage(i_messageID);
        }
    }
    QQC2.MenuItem {
        id: reportMessageItem
        contentItem: QQC2.Label {
            text: i18n("Report Message")
        }
        onTriggered: {
            messageMain.reportMessage(i_messageID);
        }
    }
    QQC2.MenuItem {
        id: ignoreUserItem
        contentItem: QQC2.Label {
            id: ignoreLabel
            text: updateIgnoreLabelText()
        }
        onTriggered: {
            console.log(RuqolaDebugCategorySingleton.category, "Ignore", i_messageID);
            messageMain.ignoreUser(!user_ignored);
        }
    }
    onAboutToShow: {
        starredMessageItem.visible = rcAccount.allowMessageStarringEnabled()
        if (starredMessageItem.visible) {
            favoriteLabel.text = updateFavoriteLabelText()
        }
        pinnedMessageItem.visible = rcAccount.allowMessagePinningEnabled()
        if (pinnedMessageItem.visible) {
            pinnedMessageLabel.text = updatePinnedMessageLabelText();
        }

        editMessageItem.visible = (i_username === i_own_username) && rcAccount.allowEditingMessages() && can_editing_message
        ignoreLabel.text = updateIgnoreLabelText()
        ignoreUserItem.visible = (i_username != i_own_username)
    }
}
