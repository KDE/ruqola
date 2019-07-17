/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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
import KDE.Ruqola.RocketChatAccount 1.0
QQC2.Menu {
    id: menu
    property bool can_manage_users: true
    property string userId
    property string ownUserId
    property bool hasOwnerRole: true
    property bool hasModeratorRole: true
    property bool hasLeaderRole: true
    property bool userIsIgnored: false
    property bool isAdirectChannel: false
    signal ignoreUser(string userId, bool ignore)
    signal kickUser(string userId)
    signal changeRole(string userId, int type)
    signal openConversation(string userId)
    //Hide it if direct channel

    function updateIgnoreMenuItemText()
    {
        return userIsIgnored ? i18n("Unignore") : i18n("Ignore")
    }
    function updateOwnerMenuItemText()
    {
        return hasOwnerRole ? i18n("Remove as Owner") : i18n("Add as Owner")
    }

    function updateLeaderMenuItemText()
    {
        return hasLeaderRole ? i18n("Remove as Leader") : i18n("Add as Leader")
    }

    function updateModeratorMenuItemText()
    {
        return hasModeratorRole ? i18n("Remove as Moderator") : i18n("Add as Moderator")
    }


    QQC2.MenuItem {
        id: conversationItem
        contentItem: QQC2.Label {
            text: i18n("Conversation")
            textFormat: Text.PlainText
        }
        onTriggered: {
            menu.openConversation(userId)
        }
        visible: !isAdirectChannel
    }
    QQC2.MenuItem {
        id: removeAsOwnerItem
        visible: can_manage_users && !isAdirectChannel
        contentItem: QQC2.Label {
            text: updateOwnerMenuItemText()
        }
        onTriggered: {
            menu.changeRole(userId, hasOwnerRole ? RocketChatAccount.RemoveOwner : RocketChatAccount.AddOwner);
        }
    }
    QQC2.MenuItem {
        id: removeAsLeaderItem
        visible: can_manage_users && !isAdirectChannel
        contentItem: QQC2.Label {
            text: updateLeaderMenuItemText()
        }
        onTriggered: {
            menu.changeRole(userId, hasLeaderRole ? RocketChatAccount.RemoveLeader : RocketChatAccount.AddLeader);
        }
    }
    QQC2.MenuItem {
        id: removeAsModeratorItem
        visible: can_manage_users && !isAdirectChannel
        contentItem: QQC2.Label {
            text: updateModeratorMenuItemText()
        }
        onTriggered: {
            menu.changeRole(userId, hasModeratorRole ? RocketChatAccount.RemoveModerator : RocketChatAccount.AddModerator);
        }
    }
    QQC2.MenuItem {
        id: ignoreItem
        contentItem: QQC2.Label {
            text: updateIgnoreMenuItemText();
        }
        onTriggered: {
            menu.ignoreUser(userId, !userIsIgnored)
        }
        visible: userId != ownUserId  && !isAdirectChannel
    }
    QQC2.MenuItem {
        id: kickItem
        visible: can_manage_users && !isAdirectChannel
        contentItem: QQC2.Label {
            text: i18n("Remove from Room")
            textFormat: Text.PlainText
        }
        onTriggered: {
            menu.kickUser(userId)
        }
    }
    onAboutToShow: {
        ignoreItem.text = updateIgnoreMenuItemText();
        removeAsOwnerItem.text = updateOwnerMenuItemText();
        removeAsLeaderItem.text = updateLeaderMenuItemText();
        removeAsModeratorItem.text = updateModeratorMenuItemText();
    }
}
