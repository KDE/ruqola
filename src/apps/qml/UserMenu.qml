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

import org.kde.kirigami 2.4 as Kirigami
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Layouts 1.1
import KDE.Ruqola.DebugCategory 1.0
QQC2.Menu {
    id: menu
    property bool can_manage_users: false
    property string userId

    signal ignoreUser(string userId)
    signal kickUser(string userId)

    QQC2.MenuItem {
        id: conversationItem
        contentItem: QQC2.Label {
            text: i18n("Conversation")
        }
        onTriggered: {
        }
    }
    QQC2.MenuItem {
        id: removeAsOwnerItem
        visible: can_manage_users
        contentItem: QQC2.Label {
            text: i18n("Remove as Owner")
        }
        onTriggered: {
        }
    }
    QQC2.MenuItem {
        id: removeAsLeaderItem
        visible: can_manage_users
        contentItem: QQC2.Label {
            text: i18n("Remove as Leader")
        }
        onTriggered: {
        }
    }
    QQC2.MenuItem {
        id: removeAsModeratorItem
        visible: can_manage_users
        contentItem: QQC2.Label {
            text: i18n("Remove as Moderator")
        }
        onTriggered: {
        }
    }
    QQC2.MenuItem {
        id: ignoreItem
        contentItem: QQC2.Label {
            text: i18n("Ignore")
        }
        onTriggered: {
        }
    }
    QQC2.MenuItem {
        id: kickItem
        visible: can_manage_users
        contentItem: QQC2.Label {
            text: i18n("Remove from Room")
        }
        onTriggered: {
        }
    }
}
