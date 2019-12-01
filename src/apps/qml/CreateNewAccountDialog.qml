/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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
        usernameField.text = ""
        server.text = ""
        open()
    }

    contentItem: GridLayout {
        columns: 2
        QQC2.Label {
            text: i18n("Name:");
        }
        QQC2.TextField {
            id: accountName
            Layout.fillWidth: true
            selectByMouse: true
            placeholderText: i18n("Account Name")
        }
        QQC2.Label {
            text: i18n("Server Url:");
        }
        QQC2.TextField {
            id: server
            Layout.fillWidth: true
            selectByMouse: true
            placeholderText: i18n("Server Url")
        }
        QQC2.Label {
            id: username

            width: parent.width
            text: i18n("Enter your Username:")
        }

        QQC2.TextField {
            id: usernameField
            Layout.fillWidth: true
            selectByMouse: true
            width: parent.width
            placeholderText: i18n("Enter Username")
        }
        Item {
            Layout.fillHeight: true
        }
    }
    onAccepted: {
        createNewAccountDialog.createNewAccount(accountName.text, server.text, usernameField.text)
    }
}
