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

import org.kde.kirigami 2.7 as Kirigami
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Layouts 1.12
import Ruqola 1.0

QQC2.Menu {
    id: menu
    property bool can_edit_message
    property bool user_ignored
    property bool starred
    property bool pinned_message
    property bool showTranslatedMessage

    QQC2.MenuItem {
        id: startADiscussionItem
        visible: rcAccount.discussionEnabled
        height: visible ? implicitHeight : 0
        icon.name: "user-group-new"
        text: i18n("Start a Discussion")
        onTriggered: {
            messageMain.createDiscussion(i_messageID, i_originalMessage);
            console.log(RuqolaDebugCategorySingleton.category, "Create discussion: messageId: ", i_messageID);
        }
    }
    QQC2.MenuItem {
        id: startAThreadItem
        visible: rcAccount.threadsEnabled
        height: visible ? implicitHeight : 0
        icon.name: "mail-reply-sender"
        text: i18n("Reply in Thread")
        onTriggered: {
            messageMain.replyInThread(i_messageID);
            console.log(RuqolaDebugCategorySingleton.category, "Reply in thread ", i_messageID);
        }
    }

    QQC2.MenuItem {
        id: copyMessageItem
        icon.name: "list-add"
        text: i18n("Copy")
        onTriggered: {
            messageMain.copyMessage(i_messageID, i_originalMessage);
            console.log(RuqolaDebugCategorySingleton.category, "Copy", i_messageID, i_originalMessage);
        }
    }

    QQC2.MenuItem {
        id: editMessageItem
        visible: i_username === i_own_username && rcAccount.allowEditingMessages && can_edit_message
        height: visible ? implicitHeight : 0
        icon.name: "entry-edit"
        text: i18n("Edit")
        onTriggered: {
            messageMain.editMessage(i_messageID, i_originalMessage);
            console.log(RuqolaDebugCategorySingleton.category, "Edit", i_messageID, i_originalMessage);
        }
    }
    QQC2.MenuItem {
        icon.name: "mail-reply-all"
        text: i18n("Reply")
        onTriggered: {
            console.log(RuqolaDebugCategorySingleton.category, "Reply to", i_messageID);
            messageMain.replyMessage(i_messageID);
        }
    }
    QQC2.MenuItem {
        id: starredMessageItem
        icon.name: "favorite"
        visible: rcAccount.allowMessageStarringEnabled
        height: visible ? implicitHeight : 0
        text: starred ? i18n("Remove as Favorite") : i18n("Set as Favorite")
        onTriggered: {
            console.log(RuqolaDebugCategorySingleton.category, "Set as favorite", i_messageID);
            messageMain.setFavoriteMessage(i_messageID, !starred);
        }
    }

    QQC2.MenuItem {
        id: pinnedMessageItem
        visible: rcAccount.allowMessagePinningEnabled
        height: visible ? implicitHeight : 0
        icon.name: pinned_message ? "window-unpin" : "window-pin"
        text: pinned_message ? i18n("Unpin Message") : i18n("Pin Message")
        onTriggered: {
            messageMain.pinMessage(i_messageID, !pinned_message);
        }
    }

    QQC2.MenuItem {
        id: deleteMessageItem
        icon.name: "delete"
        visible: i_username === i_own_username && rcAccount.allowMessageDeletingEnabled
        height: visible ? implicitHeight : 0
        text: i18n("Delete")
        onTriggered: {
            messageMain.deleteMessage(i_messageID);
        }
    }
    QQC2.MenuItem {
        id: reportMessageItem
        icon.name: "flag"
        text: i18n("Report Message")
        onTriggered: {
            messageMain.reportMessage(i_messageID);
        }
    }
    QQC2.MenuItem {
        id: ignoreUserItem
        visible: i_username != i_own_username
        height: visible ? implicitHeight : 0
        icon.name: !user_ignored ? "mail-thread-ignored" : ""
        text: user_ignored ? i18n("Unignore") : i18n("Ignore")
        onTriggered: {
            console.log(RuqolaDebugCategorySingleton.category, "Ignore", i_messageID);
            messageMain.ignoreUser(!user_ignored);
        }
    }

    QQC2.MenuItem {
        id: translateMessageItem
        visible: rcAccount.autoTranslateEnabled
        height: visible ? implicitHeight : 0
        text: showTranslatedMessage ? i18n("Original Message") : i18n("Translate Message")
        onTriggered: {
            console.log(RuqolaDebugCategorySingleton.category, "change translate status", i_messageID);
            messageMain.showOriginalOrTranslatedMessage(i_messageID, false) //TODO fix me!
        }
    }
}
