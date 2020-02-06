/*
 
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>
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
import QtQuick.Controls 2.5 as QQC2
import org.kde.kirigami 2.7 as Kirigami
import Ruqola 1.0

Kirigami.Page {
    id: loginForm
    
    property QtObject rcAccount
    property alias username: usernameField.text;
    property alias password: passField.text;
    property alias twoFactorAuthenticationCode: twoFactorAuthenticationCodeField.text;
    property alias serverUrl: urlField.text;
    property alias accountName: nameField.text;

    property string originalAccountName

    property string lastSocketError

    signal accepted()

    function resetStatus() {
        lastSocketError = ""
    }

    Keys.onReturnPressed: accepted()

    implicitHeight: 400
    implicitWidth: 300

    Column {
        id: form

        anchors.centerIn: parent
        width: 0.8*parent.width
        spacing: 3
        QQC2.Label {
            id: loginLabel

            text: i18n("Rocket.Chat Login")
            textFormat: Text.PlainText
            color: "#555"
            font.pointSize: 40
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
        }
        
        Item {
            id: spacer

            width: 30
            height: 30
        }

        QQC2.Label {
            width: parent.width
            text: i18n("Account Name")
            textFormat: Text.PlainText
        }

        QQC2.TextField {
            id: nameField
            selectByMouse: true
            text: originalAccountName
            readOnly: originalAccountName !== ""
            width: parent.width
            placeholderText: i18n("Enter account name")
            onTextEdited: resetStatus()
        }

        QQC2.Label {
            width: parent.width
            text: i18n("Rocket.Chat Server")
            textFormat: Text.PlainText
        }

        QQC2.TextField {
            id: urlField
            selectByMouse: true
            width: parent.width
            placeholderText: i18n("Enter address of the server")
            onTextEdited: resetStatus()
        }
        
        //

        QQC2.Label {
            width: parent.width
            text: i18n("Authentication Method")
            visible: loginMethodCombobox.visible
            textFormat: Text.PlainText
        }

        QQC2.ComboBox {
            id: loginMethodCombobox
            width: parent.width
            model: appid.rocketChatAccount.loginMethodModel
            visible: count > 1
            textRole: "name"
            currentIndex: model.currentLoginMethod
            onActivated: {
                appid.rocketChatAccount.changeDefaultAuthentication(index)
                resetStatus()
            }

            delegate: QQC2.ItemDelegate {
                width: loginMethodCombobox.width

                contentItem: Text {
                    text: model.name
                    font: loginMethodCombobox.font
                    //TODO fix color.
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }
                highlighted: loginMethodCombobox.highlightedIndex === index
            }
        }



        QQC2.Label {
            id:username

            width: parent.width
            text: i18n("Username")
        }

        QQC2.TextField {
            selectByMouse: true
            width: parent.width
            id: usernameField
            placeholderText: i18n("Enter username")
            onTextEdited: resetStatus()
        }
        
        QQC2.Label {
            id: passLabel

            width: parent.width
            text: i18n("Password")
        }
        
        PasswordLineEdit {
            id: passField
            width: parent.width
            onAccepted: {
                if (acceptingButton.enabled) {
                    acceptingButton.clicked();
                }
            }
            onTextEdited: resetStatus()
        }

        QQC2.Label {
            id: twoFactorAuthenticationCodeLabel

            width: parent.width
            wrapMode: Text.Wrap
            text: i18n("You have enabled second factor authentication. Please enter the generated code or a backup code.")
            color: Kirigami.Theme.negativeTextColor
            font.bold: true
            visible: rcAccount.loginStatus === DDPClient.LoginCodeRequired
        }

        PasswordLineEdit {
            id: twoFactorAuthenticationCodeField
            width: parent.width
            placeholderText: i18n("Two-factor authentication code or backup code")
            visible: rcAccount.loginStatus === DDPClient.LoginCodeRequired
            onAccepted: {
                if (acceptingButton.enabled) {
                    acceptingButton.clicked();
                }
            }
            onTextEdited: resetStatus()
        }

        Item {
            id: spacer2
            width: 30
            height: 30
        }
        
        QQC2.Button {
            id: acceptingButton

            width: parent.width
            text: i18n("Log in")
            enabled: (passField.text && urlField.text && usernameField.text && nameField.text)
            onClicked: loginForm.accepted()
        }        

        Item {
            id: spacer3

            width: 30
            height: 30
        }

        QQC2.Label {
            id: statusLabel
            text: {
                if (rcAccount.loginStatus === DDPClient.LoginFailed)
                    return i18n("Login Failed");
                else if (rcAccount.loginStatus === DDPClient.LoggingIn)
                    return i18n("Logging In...")
                else if (lastSocketError != "")
                    return i18n("Socket error: %1", lastSocketError)
                return ""
            }
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
            color: rcAccount.loginStatus === DDPClient.LoggingIn ? Kirigami.Theme.textColor : Kirigami.Theme.negativeTextColor
            font.bold: true
            visible: rcAccount.loginStatus === DDPClient.LoginFailed ||
                     rcAccount.loginStatus === DDPClient.LoggingIn ||
                     (rcAccount.loginStatus === DDPClient.NotConnected && lastSocketError != "")
        }
        QQC2.Label {
            text: i18n("Installation Problem found. No plugins found here.");
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
            color: Kirigami.Theme.negativeTextColor
            font.bold: true
            visible: rcAccount.loginStatus === DDPClient.FailedToLoginPluginProblem
        }
    }

    Connections {
        target: rcAccount
        onSocketError: lastSocketError = errorString
    }
}
