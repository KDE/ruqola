/*
 
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>
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
import QtQuick.Controls 2.5
import org.kde.kirigami 2.7 as Kirigami

import QtQuick.Layouts 1.12
import Ruqola 1.0

import "messages/"

Rectangle {
    id: root

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
    property string i_timestamp
    property var i_messageType
    property var i_urls
    property var i_attachments
    property var i_reactions
    property string i_roles
    property string i_date
    property string i_own_username
    property bool i_can_edit_message
    property bool i_starred
    property bool i_user_ignored
    property bool i_pinned
    property int i_dcount
    property string i_drid
    property string i_tmid
    property string i_threadPreview
    property int i_tcount
    property bool i_groupable

    property bool i_useMenuMessage
    property bool i_showTranslatedMessage

    property QtObject rcAccount

    color: RuqolaSingleton.backgroundColor
    implicitHeight: loaded.item ? 2*Kirigami.Units.smallSpacing + loaded.item.implicitHeight : 0
    
    implicitWidth: 150

    signal openDirectChannel(string userName)
    signal openChannel(string channel)
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
    signal showOriginalOrTranslatedMessage(string messageId, bool showOriginal)
    signal showDisplayAttachment(string messageId, bool displayAttachment)

    Component {
        id: jitsiMessageComponent
        JitsiVideoMessage { messageMain: root }
    }
    Component {
        id: systemMessageComponent
        SystemMessage { messageMain: root }
    }
    Component {
        id: userMessageComponent
        UserMessage { messageMain: root }
    }
    Component {
        id: attachmentMessageAudioComponent
        AttachmentMessageAudio { messageMain: root }
    }
    Component {
        id: attachmentMessageVideoComponent
        AttachmentMessageVideo { messageMain: root }
    }
    Component {
        id: attachmentMessageImageComponent
        AttachmentMessageImage { messageMain: root }
    }

    Loader {
        id: loaded
        anchors {
            fill: parent
            margins: Kirigami.Units.largeSpacing
        }

        function getComponent() {
            if (i_messageType === Message.System) {
                if (i_systemMessageType === "jitsi_call_started") {
                    return jitsiMessageComponent;
                } else {
                    return systemMessageComponent;
                }
            } else if (i_messageType === Message.NormalText || i_messageType === Message.File) {
                return userMessageComponent;
            } else if (i_messageType === Message.Audio) {
                return attachmentMessageAudioComponent;
            } else if (i_messageType === Message.Video) {
                return attachmentMessageVideoComponent;
            } else if (i_messageType === Message.Image) {
                return attachmentMessageImageComponent;
            }
            console.warning(RuqolaDebugCategorySingleton.category, "Unknown message type: " + i_messageType)
            return null;
        }

        sourceComponent: getComponent()
    }

    onLinkActivated: {
        var username = RuqolaUtils.extractRoomUserFromUrl(link);
        if (link.startsWith("ruqola:/room/")) {
            root.openChannel(username)
        } else if (link.startsWith("ruqola:/user/")) {
            if (username !== appid.rocketChatAccount.userName) {
                root.openDirectChannel(username)
            }
        } else {
            RuqolaUtils.openUrl(link);
        }
    }
}
