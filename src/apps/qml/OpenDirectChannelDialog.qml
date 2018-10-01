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
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Window 2.0
import QtQuick.Layouts 1.3

QQC2.Dialog {
    id: openDirectChannelDialog
    property string username: "username"

    signal openDirectChannel(string userName)

    standardButtons: QQC2.Dialog.Ok | QQC2.Dialog.Cancel

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    modal: true
    focus: true
    Row {
        QQC2.Label {
            text: i18n("Open Conversation with \"%1\"?", username)
            font.bold: true
            font.pointSize: 15
            wrapMode: QQC2.Label.Wrap
        }
    }

    onAccepted: {
        openDirectChannelDialog.openDirectChannel(username)
    }
}
