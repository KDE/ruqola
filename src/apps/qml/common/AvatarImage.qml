/*
 * Copyright (C) 2017-2019 Laurent Montel <montel@kde.org>
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
import QtQuick.Layouts 1.12
import "../js/message.js" as MessageScript;
import QtQuick.Window 2.2
import KDE.Ruqola.RocketChatAccount 1.0
import KDE.Ruqola.DebugCategory 1.0

Rectangle {
    id: avatarRect
    
    property string avatarurl
    property string aliasname
    property string username

    signal showUserInfo()

    Layout.alignment: Qt.AlignTop | Qt.AlignCenter
    Layout.fillHeight: false
    //Customize it.
    implicitWidth: Kirigami.Units.iconSizes.large
    implicitHeight: implicitWidth
    
    radius: 3
    
    anchors.rightMargin: 2*Kirigami.Units.smallSpacing
    
    color: avatarurl !== "" ? "transparent" : MessageScript.stringToColour(username)
    Image {
        id: avatarImage
        
        anchors.fill: parent
        visible: avatarurl !== ""
        source: avatarurl
        fillMode: Image.PreserveAspectFit
        MouseArea {
            anchors.fill: parent
            onClicked: {
                avatarRect.showUserInfo();
            }
        }
        onStatusChanged: {
            if (avatarImage.status === Image.Error) {
                console.log(RuqolaDebugCategorySingleton.category, "An error occurred when we try to load image");
            }
        }

    }
    QQC2.Label {
        id: avatarText
        
        visible: avatarurl == ""
        anchors.fill: parent
        anchors.margins: Kirigami.Units.smallSpacing
        
        renderType: Text.QtRendering
        color: Kirigami.Theme.backgroundColor

        font.weight: Font.Bold
        font.pointSize: 40
        fontSizeMode: Text.Fit
        
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        text: {
            //TODO verify if it works with non latin char.
            if (aliasname.length > 0) {
                var match = aliasname.match(/([a-zA-Z0-9])([a-zA-Z0-9])/);
                var abbrev = match[1].toUpperCase();
                if (match.length > 2) {
                    abbrev += match[2].toLowerCase();
                }
                return abbrev;
            }
            return "";
        }
    }
    Connections {
        target: appid.rocketChatAccount
        onFileDownloaded: {
            //console.log(" filePath " + filePath + " username: " + username)
            if (filePath === "/avatar/" + username) {
                avatarurl = cacheImageUrl
            }
        }
    }
}
