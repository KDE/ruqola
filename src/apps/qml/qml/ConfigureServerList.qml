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
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import org.kde.kirigami 2.7 as Kirigami


Kirigami.Page {
    id: configureServer

    title: i18n("Configure Accounts")

    property QtObject accountModel

    actions.main: Kirigami.Action {
        icon.name: "list-add"
        text: i18n("Add Account")
        onTriggered: createNewAccountDialog.initializeAndOpen()
    }

    actions.right: Kirigami.Action {
        icon.name: "window-close-symbolic"
        text: i18n("Close")
        onTriggered: pageStack.pop()
    }

    ListView {
        id: listview
        anchors.fill: parent

        model: accountModel
        delegate: Kirigami.SwipeListItem {
            supportsMouseEvents: false
            actions: [
                Kirigami.Action {
                    icon.name: "list-remove"
                    text: i18n("Remove")
                    onTriggered: {
                        deleteAccountDialog.accountName = name
                        deleteAccountDialog.open()
                    }
                },
                Kirigami.Action {
                    icon.name: "document-edit"
                    text: i18n("Edit")
                    onTriggered: {
                        // TODO
                    }
                }
            ]

            QQC2.Label {
                text: name
            }
        }
    }

    CreateNewAccountDialog {
        id: createNewAccountDialog
        onCreateNewAccount: {
            appid.accountManager.addAccount(accountName, username, serverUrl);
        }
    }

    DeleteAccountDialog {
        id: deleteAccountDialog
        onAccepted: {
            appid.accountManager.removeAccount(accountName)
        }
    }
}
