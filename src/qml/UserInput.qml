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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0 as QQC2
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0
import QtGraphicalEffects 1.0

import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.DDPClient 1.0
//import KDE.Ruqola.Notification 1.0

// import "Log.js" as Log
// import "Data.js" as Data

import org.kde.kirigami 2.1 as Kirigami

RowLayout {
    id: footerItem
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: mainWidget.bottom
    
    height: 3*Kirigami.Units.largeSpacing
    
    Kirigami.Icon {
        id: attachment
        enabled: selectedRoomID !== ""
        source: "document-send-symbolic"
        //                     anchors.right: parent.left
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        width: height
        height: messageLine.height/2
        MouseArea {
            anchors.fill: parent
            onClicked: Ruqola.attachmentButtonClicked(selectedRoomID);
        }
    }
    TextField {
        id: messageLine
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        
        Layout.fillWidth: true
        placeholderText: {
            if (Ruqola.loginStatus != DDPClient.LoggedIn || (selectedRoomID == "")){
                qsTr("Please Select a room")
            } else {
                qsTr("Enter message")
            }
        }

        property string type: "text";
        onAccepted: {
            if (text != "" && Ruqola.loginStatus == DDPClient.LoggedIn && !(selectedRoomID == "")) {
                Ruqola.sendMessage(selectedRoomID, text, type);
                text = "";
            }
        }
    }
    
    Kirigami.Icon {
        source: "mail-sent"
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        width: height
    }

    //                 Rectangle {
    //                     anchors.fill: footerItem
    //                     color: Kirigami.Theme.viewBackgroundColor
    //                     z: -1
    //                 }
}
