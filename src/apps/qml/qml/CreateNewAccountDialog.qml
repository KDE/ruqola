/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

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
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Window 2.2

import org.kde.kirigami 2.5 as Kirigami

QQC2.Dialog {
    id: createNewAccountDialog

    signal createNewAccount(string accountName, string serverUrl, string username)

    title: i18n("Create Account")

    standardButtons: QQC2.Dialog.Ok | QQC2.Dialog.Cancel

    width: parent.width * 9 / 10
    height: parent.height * 9 / 10
    anchors.centerIn: parent

    modal: true
    focus: true
    function initializeAndOpen()
    {
        accountName.text = ""
        server.text = ""
        usernameField.text = ""
        open()
    }

    contentItem: Kirigami.FormLayout {
        QQC2.TextField {
            id: accountName
            placeholderText: "Rocket.Chat"
            Kirigami.FormData.label: i18n("Name:")
        }
        QQC2.TextField {
            id: server
            placeholderText: "https://open.rocket.chat"
            Kirigami.FormData.label: i18n("Server Url:")
        }
        QQC2.TextField {
            id: usernameField
            placeholderText: i18n("john.doe")
            Kirigami.FormData.label: i18n("Username:")
        }
    }
    onAccepted: {
        createNewAccountDialog.createNewAccount(accountName.text, server.text, usernameField.text)
    }
}
