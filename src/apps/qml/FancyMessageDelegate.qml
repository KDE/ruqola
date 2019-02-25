/*
 
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>
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

import KDE.Ruqola.RuqolaUtils 1.0
import KDE.Ruqola.Message 1.0

import QtQuick.Controls 2.2
import org.kde.kirigami 2.4 as Kirigami
import KDE.Ruqola.ExtraColors 1.0
import QtQuick.Layouts 1.1
import KDE.Ruqola.RocketChatAccount 1.0
import KDE.Ruqola.DebugCategory 1.0
Rectangle {

    id: messageMain

    property string i_messageID
    property string i_originalMessage
    property string i_messageText
    property string i_username
    property string i_usernameurl
    property bool i_systemMessage
    property string i_systemMessageType
    property string i_avatar
    property string i_aliasname
    property string i_editedByUserName
    property var i_timestamp
    property var i_messageType
    property var i_urls
    property var i_attachments
    property var i_reactions
    property var i_roles
    property string i_date
    property string i_own_username
    property bool i_can_editing_message
    property bool i_starred
    property bool i_user_ignored
    property bool i_pinned

    color: RuqolaSingleton.backgroundColor
    implicitHeight: 4*Kirigami.Units.smallSpacing + loaded.item.implicitHeight
    
    implicitWidth: 150
    
    signal openDirectChannel(string userName)
    signal openChannel(string channel)
    signal jitsiCallConfActivated()
    signal deleteMessage(string messageId)
    signal downloadAttachment(string url)
    signal editMessage(string messageId, string messageStr)
    signal copyMessage(string messageId, string messageStr)
    signal replyMessage(string messageId)
    signal setFavoriteMessage(string messageId, bool starred)
    signal displayImage(url imageUrl, string title, bool isAnimatedImage)
    signal deleteReaction(string messageId, string emoji)
    signal ignoreUser(bool ignored)
    signal pinMessage(string messageId, bool pinned)

    Loader {
        id: loaded
        anchors.fill: parent

        Component.onCompleted: {
            if (i_messageType === Message.System) {
                if (i_systemMessageType === "jitsi_call_started") {
                    console.log(RuqolaDebugCategorySingleton.category, "Jitsi");
                    setSource("messages/JitsiVideoMessage.qml",
                              {
                                  i_messageText: i_messageText,
                                  i_username: i_username,
                                  i_aliasname: i_aliasname,
                                  i_timestamp: i_timestamp,
                                  i_systemMessageType: i_systemMessageType,
                                  i_messageID: i_messageID,
                                  i_avatar: i_avatar,
                                  i_date: i_date,
                                  rcAccount: appid.rocketChatAccount
                              }
                              )
                } else {
                    console.log(RuqolaDebugCategorySingleton.category, "System Message");
                    setSource("messages/SystemMessage.qml",
                              {
                                  i_messageText: i_messageText,
                                  i_username: i_username,
                                  i_aliasname: i_aliasname,
                                  i_timestamp: i_timestamp,
                                  i_systemMessageType: i_systemMessageType,
                                  i_messageID: i_messageID,
                                  i_date: i_date,
                                  rcAccount: appid.rocketChatAccount
                              }
                              )
                }
            } else if (i_messageType === Message.NormalText || i_messageType === Message.File) {
                console.log(RuqolaDebugCategorySingleton.category, "User Message");
                setSource("messages/UserMessage.qml",
                          {
                              i_originalMessage: i_originalMessage,
                              i_messageText: i_messageText,
                              i_username: i_username,
                              i_usernameurl: i_usernameurl,
                              i_aliasname: i_aliasname,
                              i_timestamp: i_timestamp,
                              i_messageID: i_messageID,
                              i_avatar: i_avatar,
                              i_urls: i_urls,
                              i_reactions: i_reactions,
                              i_roles: i_roles,
                              i_attachments: i_attachments,
                              i_date: i_date,
                              i_own_username: i_own_username,
                              rcAccount: appid.rocketChatAccount,
                              i_can_editing_message: i_can_editing_message,
                              i_editedByUserName: i_editedByUserName,
                              i_starred: i_starred,
                              i_pinned: i_pinned,
                              i_user_ignored : i_user_ignored
                          }
                          )
            } else if (i_messageType === Message.Audio) {
                console.log(RuqolaDebugCategorySingleton.category, "Audio");
                setSource("messages/AttachmentMessageAudio.qml",
                          {
                              i_messageText: i_messageText,
                              i_username: i_username,
                              i_usernameurl: i_usernameurl,
                              i_aliasname: i_aliasname,
                              i_timestamp: i_timestamp,
                              i_messageID: i_messageID,
                              i_avatar: i_avatar,
                              i_reactions: i_reactions,
                              i_roles: i_roles,
                              i_urls: i_urls,
                              i_attachments: i_attachments,
                              i_date: i_date,
                              i_editedByUserName: i_editedByUserName,
                              rcAccount: appid.rocketChatAccount,
                              i_editedByUserName: i_editedByUserName,
                              i_starred: i_starred,
                              i_pinned: i_pinned
                          })
            } else if (i_messageType === Message.Video) {
                console.log(RuqolaDebugCategorySingleton.category, "Video");
                setSource("messages/AttachmentMessageVideo.qml",
                          {
                              i_messageText: i_messageText,
                              i_username: i_username,
                              i_usernameurl: i_usernameurl,
                              i_aliasname: i_aliasname,
                              i_timestamp: i_timestamp,
                              i_messageID: i_messageID,
                              i_avatar: i_avatar,
                              i_reactions: i_reactions,
                              i_roles: i_roles,
                              i_urls: i_urls,
                              i_attachments: i_attachments,
                              i_date: i_date,
                              i_editedByUserName: i_editedByUserName,
                              rcAccount: appid.rocketChatAccount,
                              i_editedByUserName: i_editedByUserName,
                              i_starred: i_starred,
                              i_pinned: i_pinned
                          })
            } else if (i_messageType === Message.Image) {
                console.log(RuqolaDebugCategorySingleton.category, "Image");
                setSource("messages/AttachmentMessageImage.qml",
                          {
                              i_messageText: i_messageText,
                              i_username: i_username,
                              i_usernameurl: i_usernameurl,
                              i_aliasname: i_aliasname,
                              i_timestamp: i_timestamp,
                              i_messageID: i_messageID,
                              i_avatar: i_avatar,
                              i_reactions: i_reactions,
                              i_roles: i_roles,
                              i_urls: i_urls,
                              i_attachments: i_attachments,
                              i_date: i_date,
                              i_editedByUserName: i_editedByUserName,
                              rcAccount: appid.rocketChatAccount,
                              i_editedByUserName: i_editedByUserName,
                              i_starred: i_starred,
                              i_pinned: i_pinned
                          })

            } else {
                console.log(RuqolaDebugCategorySingleton.category, "Unknown message type: " + i_messageType)
            }
        }
    }
    Connections {
        target: loaded.item
        onLinkActivated: {
            var username = RuqolaUtils.extractRoomUserFromUrl(link);
            if (link.startsWith("ruqola:/room/")) {
                messageMain.openChannel(username)
            } else if (link.startsWith("ruqola:/user/")) {
                if (username !== appid.rocketChatAccount.userName) {
                    messageMain.openDirectChannel(username)
                }
            } else {
                RuqolaUtils.openUrl(link);
            }
        }
        onJitsiCallConfActivated: {
            messageMain.jitsiCallConfActivated()
        }
        onDeleteMessage: {
            messageMain.deleteMessage(messageId)
        }
        onDownloadAttachment: {
            messageMain.downloadAttachment(url)
        }
        onEditMessage: {
            console.log(RuqolaDebugCategorySingleton.category, "i_messageText " + i_messageText);
            messageMain.editMessage(messageId, messageStr)
        }
        onCopyMessage: {
            console.log(RuqolaDebugCategorySingleton.category, "i_messageText " + i_messageText);
            messageMain.copyMessage(messageId, messageStr)
        }
        onReplyMessage: {
            messageMain.replyMessage(messageId)
        }
        onSetFavoriteMessage: {
            messageMain.setFavoriteMessage(messageId, starred)
        }
        onDisplayImage: {
            messageMain.displayImage(imageUrl, title, isAnimatedImage)
        }
        onDeleteReaction: {
            messageMain.deleteReaction(messageId, emoji)
        }
        onIgnoreUser: {
            messageMain.ignoreUser(ignored)
        }
    }
}
