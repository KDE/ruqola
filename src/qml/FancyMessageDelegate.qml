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

import KDE.Ruqola.RuqolaUtils 1.0
import KDE.Ruqola.Message 1.0

import QtQuick.Controls 2.2
import org.kde.kirigami 2.1 as Kirigami
import KDE.Ruqola.ExtraColors 1.0
import QtQuick.Layouts 1.1
import KDE.Ruqola.Ruqola 1.0

Rectangle {

    property string i_messageID
    property string i_messageText
    property string i_username
    property bool i_systemMessage
    property string i_systemMessageType
    property string i_avatar
    property string i_aliasname
    property var i_timestamp
    property var i_messageType
    property var i_urls
    property var i_attachments
    property var i_date

    id: messageMain
    color: RuqolaSingleton.backgroundColor
    implicitHeight: 4*Kirigami.Units.smallSpacing + loaded.implicitHeight
    
    implicitWidth: 150
    
    anchors.bottomMargin: 200
    
    Loader {
        id: loaded
        anchors.topMargin: Kirigami.Units.smallSpacing
        anchors.fill: parent
        
        Component.onCompleted: {
            if (i_messageType === Message.System) {
                if (i_systemMessageType === "jitsi_call_started") {
                    setSource("messages/JitsiVideoMessage.qml",
                              {
                                  i_messageText: i_messageText,
                                  i_username: i_username,
                                  i_aliasname: i_aliasname,
                                  i_timestamp: i_timestamp,
                                  i_systemMessageType: i_systemMessageType,
                                  i_messageID: i_messageID,
                                  i_date: i_date
                              }
                              )
                } else {
                    setSource("messages/SystemMessage.qml",
                              {
                                  i_messageText: i_messageText,
                                  i_username: i_username,
                                  i_aliasname: i_aliasname,
                                  i_timestamp: i_timestamp,
                                  i_systemMessageType: i_systemMessageType,
                                  i_messageID: i_messageID,
                                  i_date: i_date
                              }
                              )
                }
            } else if (i_messageType === Message.NormalText || i_messageType === Message.File) {
                setSource("messages/UserMessage.qml",
                          {
                              i_messageText: i_messageText,
                              i_username: i_username,
                              i_aliasname: i_aliasname,
                              i_timestamp: i_timestamp,
                              i_messageID: i_messageID,
                              i_avatar: i_avatar,
                              i_urls: i_urls,
                              i_attachments: i_attachments,
                              i_date: i_date
                          }
                          )
            } else if (i_messageType === Message.Audio) {
                setSource("messages/AttachmentMessageAudio.qml")
            } else if (i_messageType === Message.Video) {
                setSource("messages/AttachmentMessageVideo.qml")
            } else if (i_messageType === Message.Image) {
                setSource("messages/AttachmentMessageImage.qml",
                          {
                              i_messageText: i_messageText,
                              i_username: i_username,
                              i_aliasname: i_aliasname,
                              i_timestamp: i_timestamp,
                              i_messageID: i_messageID,
                              i_avatar: i_avatar,
                              i_urls: i_urls,
                              i_attachments: i_attachments
                          })

            } else {
                console.log("Unknown message type: " + i_messageType)
            }
        }
    }
    Connections {
        target: loaded.item
        onLinkActivated: {
            if (link.startsWith("ruqola:/room/")) {
                Ruqola.rocketChatAccount().openChannel(link);
                console.log("RUQOLA room: " + link);
            } else if (link.startsWith("ruqola:/user/")) {
                console.log("RUQOKA user: " + link);
                Ruqola.rocketChatAccount().openDirectChat(link);
            } else {
                RuqolaUtils.openUrl(link);
            }
        }
    }

}
