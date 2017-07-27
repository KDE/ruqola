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


import QtQuick 2.0
// import "marked.js" as Markdown
// import "js/marked.js" as MarkDown;

import QtQuick.Controls 2.2
import org.kde.kirigami 2.1 as Kirigami

import QtQuick.Layouts 1.1

Rectangle {

    function getTextFor(type) {
        
        if (type === "uj") {
            return qsTr("has joined the channel");
        } else if (type === "ul") {
            return qsTr("has left the channel");
        } else if (type === "room_changed_topic") {
            return qsTr("changed topic to \"%1\"").arg(i_messageText)
        } else if (type === "au") {
            return qsTr("added %1 to the conversation").arg(i_messageText)
        } else if (type === "r") {
            return qsTr("changed room name to \"%1\"").arg(i_messageText)
        } else if (type === "room_changed_description") {
            return qsTr("changed room description to \"%1\"").arg(i_messageText)
        } else {
            console.log("Unkown type for message");
            console.log(type);
            console.log(i_messageText)
            return qsTr("Unknown action!");
        }
    }

    property string i_messageText
    property string i_username
    property bool i_systemMessage
    property string i_systemMessageType
    property var i_timestamp

    id: messageMain
    color: "#eeeeee"
//     implicitHeight: textLabel.contentHeight
    implicitHeight: 4*Kirigami.Units.smallSpacing + Math.max(textLabel.implicitHeight+usernameLabel.implicitHeight, avatarRect.implicitHeight)
    
    implicitWidth: 150
    
    anchors.bottomMargin: 200
    
//     anchors.margins: 50

    RowLayout {
        
        anchors.topMargin: Kirigami.Units.smallSpacing
        anchors.fill: parent
//         implicitHeight: textLabel.contentHeight

        spacing: Kirigami.Units.smallSpacing
//         spacing: 12
        
        Rectangle {
            Layout.fillHeight: false

            id: avatarRect
            implicitWidth: 24
            implicitHeight: 24
            
            anchors.margins: Kirigami.Units.smallSpacing
            
            color: "gray"
            anchors.top: parent.top
        }
        
        Rectangle {
            id: textRect
            
            Layout.fillWidth: true
//             radius: 4
//             color: "#eeeeee"
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.bottomMargin:  Kirigami.Units.smallSpacing
//             height: textLabel.implicitHeight + usernameLabel.implicitHeight
            
            Column {
//             implicitWidth: 100
                anchors.fill:parent 
                Kirigami.Heading {
                    level: 5
                    id: usernameLabel
                    font.bold: true
                    text: i_username
    //                 anchors.top: parent.top
                    anchors.right: parent.right
                    anchors.left: parent.left
                }
                Kirigami.Label {
                    id: textLabel
    //                 anchors.top: usernameLabel.bottom
                    anchors.right: parent.right
                    anchors.left: parent.left
    //                 anchors.bottom: parent.bottom
                    text: getTextFor(i_systemMessageType)
                    
                    wrapMode: Label.Wrap
                }

//                 Rectangle {
//                                 color: "red"
//     //                 anchors.top: textLabel.bottom
//                     anchors.right: parent.right
//                     anchors.left: parent.left
//     //                 anchors.bottom: parent.bottom
//                     implicitHeight: 2*Kirigami.Units.smallSpacing
//                 }
            }
        }

//         Kirigami.Label {
//             id: timeLabel
//             text: "["+(new Date(i_timestamp)).toLocaleTimeString(Locale.ShortFormat)+"]"
// 
//             anchors.top: parent.top
//             anchors.bottom: parent.bottom
//             
//             color: Kirigami.Theme.textColor
//             opacity: .5
//             
//             z:1
//         }
//     }
//     
   

//     Label {
//         color: i_systemMessage? "#999" : "#555"
//         text: i_username
//         id: usernameLabel
//         clip: true
//         
//         horizontalAlignment: Text.AlignRight
// 
//         anchors.top: parent.top
//         anchors.bottom: parent.bottom
//         anchors.left: timeLabel.left
//         
//         width: 100
//     }
// 
//     Label {
//         color: i_systemMessage? "#999" : "#111"
//         id: textLabel
//         text: i_systemMessage? getTextFor(i_systemMessageType) : MarkDown.md2html(i_messageText)
// 
// 
//         anchors.top: parent.top
//         anchors.bottom: parent.bottom
//         
//         anchors.left: usernameLabel.right
//         anchors.right: parent.right
//         
//         wrapMode: Label.Wrap
// 
//         anchors.leftMargin: 5
// 
    }

}
