/*
 
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


import QtQuick 2.0

import QtQuick.Controls 2.2
import org.kde.kirigami 2.1 as Kirigami

import QtQuick.Layouts 1.1

Rectangle {

    function getTextFor(type) {
        
        if (type === "uj") {
            return i18n("has joined the channel");
        } else if (type === "ul") {
            return i18n("has left the channel");
        } else if (type === "room_changed_topic") {
            return i18n("changed topic to: <i>%1</i>").arg(i_messageText)
        } else if (type === "au") {
            return i18n("added %1 to the conversation").arg(i_messageText)
        } else if (type === "r") {
            return i18n("changed room name to <a href=\"ruqola:/room/%1\">#%1</a>").arg(i_messageText)
        } else if (type === "ru") {
            return i18n("removed user %1").arg(i_messageText)
        } else if (type === "room_changed_description") {
            return i18n("changed room description to %1").arg(i_messageText)
        }  else if (type === "room_changed_privacy") {
            return i18n("changed room privacy to %1").arg(i_messageText)
        } else {
            console.log("Unkown type for message");
            console.log(type);
            console.log(i_messageText)
            return i18n("Unknown action!");
        }
    }

    property string i_messageText
    property string i_username
    property string i_aliasname
    property string i_systemMessageType
    property var i_timestamp

    signal linkActivated(string link)
    
    id: messageMain
    color: "#eeeeee"
    implicitHeight: 2*Kirigami.Units.smallSpacing + textLabel.implicitHeight
    
    implicitWidth: 150
    
    anchors.bottomMargin: 200
    
    Rectangle {
        
        anchors.rightMargin: 2*Kirigami.Units.largeSpacing
        anchors.leftMargin: 2*Kirigami.Units.largeSpacing
        
        anchors.centerIn: parent
        
        width: textLabel.implicitWidth + 6*Kirigami.Units.smallSpacing
        height: textLabel.height
        
        color: Kirigami.Theme.disabledTextColor
        radius: 4*Kirigami.Units.smallSpacing
        
    }
    
    Kirigami.Label {
        id: textLabel

        color: Kirigami.Theme.textColor
        opacity: 1
        
        anchors.centerIn: parent
        anchors.leftMargin: Kirigami.Units.smallSpacing
        anchors.rightMargin: Kirigami.Units.smallSpacing
        
        width: Math.min(implicitWidth, parent.width - Kirigami.Units.largeSpacing)
        
        text: i_username + " " + getTextFor(i_systemMessageType)
            
        wrapMode: Label.Wrap
        
        renderType: Text.NativeRendering
        
        onLinkActivated: messageMain.linkActivated(link)
    }
}
