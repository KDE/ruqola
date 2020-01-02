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


import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Window 2.2
import QtQuick 2.9


QQC2.Dialog {
    id: deleteMessageDialog

    title: i18n("Delete Message")

    signal deleteMessage(string messageId)

    property string msgId: ""
    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    modal: true
    focus: true
    standardButtons: QQC2.Dialog.Ok | QQC2.Dialog.Cancel

    Row {
        QQC2.Label {
            text: i18n("Do you want to delete this message?")
            font.bold: true
            font.pointSize: 15
        }
    }

    onAccepted: {
        if (msgId != "")
            deleteMessageDialog.deleteMessage(msgId)
        else
            console.log(RuqolaDebugCategorySingleton.category, "message id is not define")
    }
}
