/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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
import org.kde.kirigami 2.7 as Kirigami
import "messages"

ListView {
    id: activeChat
    clip: true

    signal openDirectChannel(string userName)
    signal openChannel(string channel)
    signal jitsiCallConfActivated()
    signal deleteMessage(string messageId)
    signal reportMessage(string messageId)
    signal downloadAttachment(string url)
    signal editMessage(string messageId, string messageStr)
    signal copyMessage(string messageId, string messageStr)
    signal replyMessage(string messageId)
    signal setFavoriteMessage(string messageId, bool starred)
    signal displayImage(url imageUrl, string title, bool isAnimatedImage)
    signal deleteReaction(string messageId, string emoji)
    signal addReaction(string messageId, string emoji)
    signal ignoreUser(string userId, bool ignored)
    signal pinMessage(string messageId, bool pinned)
    signal createDiscussion(string messageId)
    signal openDiscussion(string discussionRoomId)
    signal openThread(string threadMessageId)
    signal replyInThread(string messageId)

    property QtObject rcAccount
    property string roomId: ""
    property bool enableEditingMode: false
    property bool wasAtYEnd: true

    spacing: 0//Kirigami.Units.smallSpacing
    highlightRangeMode: ListView.ApplyRange
    preferredHighlightBegin: currentItem === null ? parent.height : parent.height - currentItem.height
    preferredHighlightEnd: parent.height

    onAtYEndChanged: {
        // Remember if we were at the bottom, for when onContentHeight is called
        if (wasAtYEnd != atYEnd) {
            wasAtYEnd = atYEnd;
        }
    }

    onContentHeightChanged: {
        // This is supposed to be emitted only when messages are added or removed.
        // TODO: Apparently it is also emitted when just scrolling up and down, too... Why?
        //       I guess because items are loaded on demand, and implicitHeight depends on loaded.item.implicitHeight?
        //console.log("height=" + contentHeight + " using wasAtYEnd=" + wasAtYEnd + " changingRooms=" + changingRooms);
        if (contentHeight > 0 && (wasAtYEnd)) {
            positionViewAtEnd();
        }
    }

    Component.onCompleted: positionViewAtEnd()
    Connections {
        target: appid.rocketChatAccount
        onSwitchedRooms: {
            positionViewAtEnd();
        }
    }

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
        i_usernameurl: usernameurl
        i_aliasname: alias
        i_systemMessageType: type
        i_timestamp: timestamp
        i_messageID: messageID
        i_messageType: messagetype
        i_avatar: avatar == "" ? rcAccount.avatarUrl(userID) : avatar
        i_urls: urls
        i_attachments: attachments
        i_reactions: reactions
        i_roles: roles
        i_date: date
        i_own_username: rcAccount.userName
        i_can_edit_message: canEditMessage
        i_starred: starred
        i_pinned: pinned
        i_editedByUserName: editedByUsername
        i_user_ignored: userIsIgnored

        i_dcount: discussionCount
        i_drid: discussionRoomId
        i_tcount: threadCount
        i_threadPreview: threadMessagePreview
        i_tmid: threadMessageId
        i_groupable: groupable

        onOpenChannel: {
            activeChat.openChannel(channel)
        }

        onPinMessage: {
            activeChat.pinMessage(messageId, pinned)
        }

        onOpenDirectChannel: {
            if (rcAccount.userName !== userName) {
                activeChat.openDirectChannel(userName)
            }
        }
        onJitsiCallConfActivated: {
            activeChat.jitsiCallConfActivated()
        }
        onDeleteMessage: {
            activeChat.deleteMessage(messageId)
        }
        onReportMessage: {
            activeChat.reportMessage(messageId)
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
            activeChat.displayImage(imageUrl, title, isAnimatedImage)
        }
        onDeleteReaction: {
            activeChat.deleteReaction(messageId, emoji)
        }
        onAddReaction: {
            activeChat.addReaction(messageId, emoji)
        }
        onIgnoreUser: {
            activeChat.ignoreUser(userID, ignored)
        }
        onCreateDiscussion: {
            activeChat.createDiscussion(messageId)
        }
        onOpenDiscussion: {
            activeChat.openDiscussion(discussionRoomId)
        }
        onOpenThread: {
            activeChat.openThread(threadMessageId)
        }
        onReplyInThread: {
            activeChat.replyInThread(messageId)
        }
    }
    section {
        delegate: NewDateLabel {
            date: section
            width: activeChat.width
        }
        property: "date"
        labelPositioning: ViewSection.InlineLabels | ViewSection.CurrentLabelAtStart
    }
}
