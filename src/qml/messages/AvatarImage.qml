/*
 * Copyright 2017 Laurent Montel <montel@kde.org>
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

import QtQuick.Controls 2.2
import org.kde.kirigami 2.1 as Kirigami
import QtQuick.Layouts 1.1
import "../js/message.js" as MessageScript;

Rectangle {
    id: avatarRect
    
    Layout.alignment: Qt.AlignTop
    Layout.fillHeight: false
    implicitWidth: textLabel.font.pixelSize * 3
    implicitHeight: textLabel.font.pixelSize * 3
    
    radius: 3
    
    anchors.rightMargin: 2*Kirigami.Units.smallSpacing
    
    color: i_avatar !== "" ? "transparent" : MessageScript.stringToColour(i_username)
    Image {
        id: avatarImage
        
        anchors.fill: parent
        visible: i_avatar !== ""
        source: i_avatar
        fillMode: Image.PreserveAspectFit
    }
    Text {
        id: avatarText
        
        visible: i_avatar == ""
        anchors.fill: parent
        anchors.margins: Kirigami.Units.smallSpacing
        
        renderType: Text.QtRendering
        color: "white"
        
        font.weight: Font.Bold
        font.pointSize: 100
        fontSizeMode: Text.Fit
        
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        
        text: {
            //TODO verify if it works with non latin char.
            var match = i_aliasname.match(/([a-zA-Z])([a-zA-Z])/);
            var abbrev = match[1].toUpperCase();
            if (match.length > 2) {
                abbrev += match[2].toLowerCase();
            }
            return abbrev;
        }
    }
}
