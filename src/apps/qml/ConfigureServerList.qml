/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.5 as QQC2
import org.kde.kirigami 2.7 as Kirigami
import KDE.Ruqola.RocketChatAccount 1.0

Kirigami.Page {
    id: configureServer

    implicitHeight: 400
    implicitWidth: 300

    property QtObject accountModel

    signal closeConfigureServer()

    ColumnLayout {
        ListView {
            id: listview
            width: 300;
            height: 200
            clip: true

            model: accountModel
            delegate:
                RowLayout {
                QQC2.Label {
                    text: name
                }
                Kirigami.Icon {
                    //Fix icon ??
                    source: "list-remove"
                    //FIXME icon size
                    height: 22
                    width: 22
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            deleteAccountDialog.accountName = name
                            deleteAccountDialog.open()
                        }
                    }
                }
            }
        }
        Kirigami.Icon {
            //Fix icon ??
            source: "list-add"
            //FIXME icon size
            height: 22
            width: 22
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    createNewAccountDialog.initializeAndOpen()
                }
            }
        }
        Button {
            text: i18n("Close")
            onClicked: {
                configureServer.closeConfigureServer()
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
