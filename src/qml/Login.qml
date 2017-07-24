/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
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


import QtQuick 2.7
import QtQuick.Controls 1.3
import org.kde.kirigami 2.1 as Kirigami

Kirigami.Page {
    
    property alias username: usernameField.text;
    property alias password: passField.text;
    property alias serverURL: urlField.text;

    signal accepted()
    signal oauthAccepted()

    Keys.onPressed: {
        if (event.key === Qt.Key_Enter) {
            acceptingButton.clicked();
            
        } else if (event.key === StandardKey.Escape) {

        }
    }

    
    id: loginForm
//     color: "#eeeeee"
    implicitHeight: 400
    implicitWidth: 300

    Column {
        id: form
        anchors.centerIn: parent
        width: 0.8*parent.width
        spacing: 3
        Text {
            text: qsTr("Ruqola Log in")
            color: "#555"
            id: loginLabel
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
            text: qsTr("Rocket Chat Server")
            
        }
        TextField {
            id: urlField
//             text: loginForm.serverURL
            width: parent.width
            placeholderText: qsTr("Enter address of the server")
        }
        
        Text {
            id:username
            width: parent.width
            text: qsTr("Enter your username")
            
        }
        TextField {
            width: parent.width
            id: usernameField
            placeholderText: qsTr("Enter username")
        }
        
        Text {
            id: passLabel
            width: parent.width
            text: qsTr("Enter your password")
        }
        
        TextField {
            width: parent.width
            id:passField
            echoMode: TextInput.Password
            inputMethodHints: Qt.ImhHiddenText
            placeholderText: qsTr("Enter password")
        }
        
        Item {
            id: spacer2
            width: 30
            height: 30
        }
        
        Button {
            id: acceptingButton
            width: parent.width
            text: qsTr("Log in")
            enabled: (passField.text && urlField.text && usernameField.text)
            onClicked: loginForm.accepted()
            isDefault: true
        }

        Button {
            id: oauthButton
            width: parent.width
            text: qsTr("Log in with Google Account")
//           enabled: (passField.text && urlField.text && usernameField.text)
            onClicked: loginForm.oauthAccepted()
            visible: false
        }
        
    }
    
//     Component.onCompleted: {
//         acceptingButton.clicked.connect(loginForm.accepted)
//     }
    
     
}
