/*
 
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


import QtQuick 2.9
import QtQuick.Controls 1.3
import org.kde.kirigami 2.1 as Kirigami
import KDE.Ruqola.DDPClient 1.0
import KDE.Ruqola.RocketChatAccount 1.0
Kirigami.Page {
    id: loginForm
    
    property alias username: usernameField.text;
    property alias password: passField.text;
    property alias serverUrl: urlField.text;
    property alias accountName: nameField.text;

    signal accepted()

    

    implicitHeight: 400
    implicitWidth: 300

    Column {
        id: form

        anchors.centerIn: parent
        width: 0.8*parent.width
        spacing: 3
        Text {
            id: loginLabel

            text: i18n("Ruqola Log in")
            color: "#555"
            font.pixelSize: 40
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
        }
        
        Item {
            id: spacer

            width: 30
            height: 30
        }

        Text {
            width: parent.width
            text: i18n("Account Name")
        }

        TextField {
            id: nameField
            readOnly: true
            width: parent.width
            placeholderText: i18n("Enter Account Name")
        }

        Text {
            width: parent.width
            text: i18n("Rocket Chat Server")    
        }

        TextField {
            id: urlField

            width: parent.width
            placeholderText: i18n("Enter address of the server")
        }
        
        Text {
            id:username

            width: parent.width
            text: i18n("Enter your username")
        }

        TextField {
            width: parent.width
            id: usernameField
            placeholderText: i18n("Enter Username")
        }
        
        Text {
            id: passLabel

            width: parent.width
            text: i18n("Enter your password")
        }
        
        TextField {
            width: parent.width
            id:passField
            echoMode: TextInput.Password
            inputMethodHints: Qt.ImhHiddenText
            placeholderText: i18n("Enter password")
            onAccepted: {
                if (acceptingButton.enabled) {
                    acceptingButton.clicked();
                }
            }
        }

        Item {
            id: spacer2
            width: 30
            height: 30
        }
        
        Button {
            id: acceptingButton

            width: parent.width
            text: i18n("Log in")
            enabled: (passField.text && urlField.text && usernameField.text)
            onClicked: loginForm.accepted()
            isDefault: true
        }        

        Item {
            id: spacer3

            width: 30
            height: 30
        }

        Text {
            text: i18n("Login Failed");
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
            color: "#FF0000"
            font.bold: true
            visible: rcAccount.loginStatus === DDPClient.LoginFailed
        }
    }
}
