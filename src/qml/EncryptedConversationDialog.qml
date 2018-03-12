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


import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtQuick.Window 2.0
import QtQuick 2.9

Dialog {
    id: deleteRoomDialog

    title: i18n("Encrypted Conversation")

    property string rId: ""
    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    modal: true

    standardButtons: Dialog.Ok | Dialog.Cancel

    Row {
        Label {
            text: i18n("Do you want to accept encrypted conversation?")
            font.bold: true
            font.pointSize: 15
        }
    }
}
