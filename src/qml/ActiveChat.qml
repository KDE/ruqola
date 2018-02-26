/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

import QtQuick 2.9
import org.kde.kirigami 2.1 as Kirigami

ListView {
    id: activeChat
    clip: true

    signal openDirectChannel(string userName)
    signal openChannel(string channel)
    signal jitsiCallConfActivated()
    signal deleteMessage(string messageId)
    signal downloadAttachment(string url)
    signal editMessage(string messageId, string messageStr)
    signal copyMessage(string messageId, string messageStr)
    signal replyMessage(string messageId)
    signal setFavoriteMessage(string messageId, bool starred)
    signal displayImage(url imageUrl, string title)

    property QtObject rcAccount
    property string roomId: ""

    spacing: Kirigami.Units.smallSpacing
    highlightRangeMode: ListView.ApplyRange
    preferredHighlightBegin: currentItem === null ? parent.height : parent.height - currentItem.height
    preferredHighlightEnd: parent.height

    onCountChanged: {
        currentIndex = count - 1
    }

    Component.onCompleted: positionViewAtEnd()
    visible: count > 0
    onDragEnded : {
        if (roomId !== "") {
            rcAccount.loadHistory(roomId)
        }
    }
    delegate: FancyMessageDelegate {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.rightMargin: Kirigami.Units.largeSpacing
        anchors.leftMargin: Kirigami.Units.largeSpacing

        i_originalMessage: originalMessage
        i_messageText: messageConverted
        i_username: username
        i_aliasname: alias
        i_systemMessageType: type
        i_timestamp: timestamp
        i_messageID: messageID
        i_messageType: messagetype
        i_avatar: avatar == "" ? rcAccount.avatarUrl(userID) : avatar
        i_urls: urls
        i_attachments: attachments
        i_date: date
        i_own_username: rcAccount.userName
        i_can_editing_message: canEditingMessage
        i_starred: starred
        i_editedByUserName: editedByUsername

        onOpenChannel: {
            activeChat.openChannel(channel)
        }

        onOpenDirectChannel: {
            activeChat.openDirectChannel(userName)
        }
        onJitsiCallConfActivated: {
            activeChat.jitsiCallConfActivated()
        }
        onDeleteMessage: {
            activeChat.deleteMessage(messageId)
        }
        onDownloadAttachment: {
            activeChat.downloadAttachment(url)
        }
        onEditMessage: {
            activeChat.editMessage(messageId, messageStr)
        }
        onCopyMessage: {
            activeChat.copyMessage(messageId, messageStr)
        }
        onReplyMessage: {
            activeChat.replyMessage(messageId)
        }
        onSetFavoriteMessage: {
            activeChat.setFavoriteMessage(messageId, starred)
        }
        onDisplayImage: {
            activeChat.displayImage(imageUrl, title)
        }
    }
}
