/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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
import QtQuick.Controls 2.5 as QQC2
import org.kde.kirigami 2.7 as Kirigami
import "messages"

ListView {
    id: activeChat
    clip: true

    function scrollPageUp() {
        var newContentY = Math.max(contentY - originY - height, 0);
        activeChat.contentY = newContentY + originY;
    }
    function scrollPageDown() {
        var newContentY = Math.min(contentY - originY + height, contentHeight - height);
        activeChat.contentY = newContentY + originY;
    }

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
    signal createDiscussion(string messageId, string originalMessage)
    signal openDiscussion(string discussionRoomId)
    signal openThread(string threadMessageId, string threadPreviewText)
    signal replyInThread(string messageId)
    signal showUserInfo(string userId)
    signal showOriginalOrTranslatedMessage(string messageId, bool showOriginal)
    signal showDisplayAttachment(string messageId, bool displayAttachment)

    property QtObject rcAccount
    property string roomId: ""
    property bool enableEditingMode: false
    property bool wasAtYEnd: true
    property bool useMenuMessage: true

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
    delegate: FancyMessageDelegate {
        width: activeChat.width

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
        i_useMenuMessage: useMenuMessage
        i_showTranslatedMessage: showTranslatedMessage

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
            activeChat.createDiscussion(messageId, originalMessage)
        }
        onOpenDiscussion: {
            activeChat.openDiscussion(discussionRoomId)
        }
        onOpenThread: {
            activeChat.openThread(threadMessageId, threadPreviewText)
        }
        onReplyInThread: {
            activeChat.replyInThread(messageId)
        }
        onShowUserInfo: {
            activeChat.showUserInfo(userID)
        }
        onShowOriginalOrTranslatedMessage: {
            activeChat.showOriginalOrTranslatedMessage(messageId, showOriginal)
        }
        onShowDisplayAttachment: {
            activeChat.showDisplayAttachment(messageId, displayAttachment)
            //TODO
        }
    }

    // always show the scrollbar, also to tell whether we're really looking at the bottom of the list right away
    QQC2.ScrollBar.vertical: QQC2.ScrollBar {
        id: __verticalScrollBar
        policy: QQC2.ScrollBar.AlwaysOn
    }

// FIXME: Disabled this code path since it easily crashes the QML engine when switching quickly between rooms.
//
// See backtrace below
//    #0  __GI_raise (sig=sig@entry=6) at ../sysdeps/unix/sysv/linux/raise.c:50
//    #1  0x00007ffff45fa899 in __GI_abort () at abort.c:79
//    #2  0x00007ffff4c13cd6 in qt_message_fatal (context=..., message=...) at /home/kfunk/devel/src/qt5.12/qtbase/src/corelib/global/qlogging.cpp:1907
//    #3  0x00007ffff4c0fe40 in QMessageLogger::fatal (this=0x7fffffffb420, msg=0x7ffff4fb9ec8 "ASSERT: \"%s\" in file %s, line %d") at /home/kfunk/devel/src/qt5.12/qtbase/src/corelib/global/qlogging.cpp:888
//    #4  0x00007ffff4c0698c in qt_assert (assertion=0x7ffff6f33000 "index >=0 && index < count(group)", file=0x7ffff6f32fb0 "/home/kfunk/devel/src/qt5.12/qtdeclarative/src/qml/util/qqmllistcompositor.cpp", line=365) at /home/kfunk/devel/src/qt5.12/qtbase/src/corelib/global/qglobal.cpp:3212
//    #5  0x00007ffff6aa9872 in QQmlListCompositor::find (this=0x61700009f8e0, group=QQmlListCompositor::Default, index=99) at /home/kfunk/devel/src/qt5.12/qtdeclarative/src/qml/util/qqmllistcompositor.cpp:365
//    #6  0x00007ffff6ea5159 in QQmlDelegateModelPrivate::stringValue (this=0x61700009f800, group=QQmlListCompositor::Default, index=99, name=...) at /home/kfunk/devel/src/qt5.12/qtdeclarative/src/qml/types/qqmldelegatemodel.cpp:1132
//    #7  0x00007ffff6ea544e in QQmlDelegateModel::stringValue (this=0x60300090af00, index=99, name=...) at /home/kfunk/devel/src/qt5.12/qtdeclarative/src/qml/types/qqmldelegatemodel.cpp:1155
//    #8  0x00007ffff75c5867 in QQuickListViewPrivate::updateSections (this=0x61d0001ed280) at /home/kfunk/devel/src/qt5.12/qtdeclarative/src/quick/items/qquicklistview.cpp:1205
//    ...
//
//    section {
//        delegate: NewDateLabel {
//            date: section
//            width: activeChat.width
//        }
//        property: "date"
//        labelPositioning: ViewSection.InlineLabels | ViewSection.CurrentLabelAtStart
//    }
}
