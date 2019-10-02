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

import QtQuick.Layouts 1.12
ColumnLayout {
    signal linkActivated(string link)
    signal jitsiCallConfActivated()
    signal deleteMessage(string messageId)
    signal reportMessage(string messageId)
    signal copyMessage(string messageId, string messageStr)
    signal editMessage(string messageId, string messageStr)
    signal replyMessage(string messageId)
    signal setFavoriteMessage(string messageId, bool starred)
    signal downloadAttachment(string url)
    signal displayImage(url imageUrl, string title, bool isAnimatedImage)
    signal deleteReaction(string messageId, string emoji)
    signal addReaction(string messageId, string emoji)
    signal ignoreUser(bool ignored)
    signal pinMessage(string messageId, bool pinned)
    signal createDiscussion(string messageId, string originalMessage)
    signal openDiscussion(string discussionRoomId)
    signal openThread(string threadMessageId, string threadPreviewText)
    signal replyInThread(string messageId)
    signal showUserInfo()

    property string i_date
    property string i_username
    property string i_usernameurl
    property string i_aliasname
    property string i_avatar
    property string i_timestamp
    property string i_messageText
    property string i_originalMessage
    property var i_urls
    property var i_attachments
    property var i_reactions
    property string i_roles
    property string i_editedByUserName
    property bool i_starred
    property bool i_can_edit_message
    property bool i_user_ignored
    property bool i_pinned
    property int i_dcount
    property string i_drid
    property int i_tcount
    property string i_tmid
    property bool i_groupable
    property string i_threadPreview
    property bool i_useMenuMessage
    property bool i_showTranslatedMessage


    property QtObject rcAccount
}
