/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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
import KDE.Ruqola.RoomWrapper 1.0

QQC2.Dialog {
    id: notificationOptionsDialog

    title: i18n("Notifications")
    standardButtons: QQC2.Dialog.Close

    property QtObject roomInfo

    signal modifyNotificationsSetting(string roomId, int type, var newVal)
    property string rid
    modal: true
    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    GridLayout {
        columns: 2
        QQC2.Label {
            text: i18n("Disable Notifications:");
        }
        QQC2.Switch {
            id: disableNotification
            checked: roomInfo === null ? false : roomInfo.notificationOptions.hideUnreadStatus
            //checked: false
            onClicked: {
                notificationOptionsDialog.modifyNotificationsSetting(rid, RocketChatAccount.DisableNotifications, checked)
            }
        }

        QQC2.Label {
            text: i18n("Hide Unread Room Status:");
        }
        QQC2.Switch {
            id: hideUnreadRoomStatus
            //checked: roomInfo === null ? false : roomInfo.readOnly
            checked: false
            onClicked: {
                notificationOptionsDialog.modifyNotificationsSetting(rid, RocketChatAccount.HideUnreadStatus, checked)
            }
        }

    }
}
