/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Window 2.2
import QtQuick 2.9
import org.kde.kirigami 2.7 as Kirigami
import Ruqola 1.0

QQC2.Dialog {
    id: showThreadMessagesDialog


    property QtObject threadMessagesModel

    property string threadMessageId
    property string threadPreviewText

    title: threadPreviewText === "" ? "" : i18n("Thread Messages: %1", threadPreviewText)
    width: parent.width * 9 / 10
    height: parent.height * 9 / 10
    anchors.centerIn: parent


    modal: true
    focus: true
    standardButtons: QQC2.Dialog.Close
    contentItem: ColumnLayout {
        ActiveChat {
            id: threadMessageListView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: threadMessagesModel
            rcAccount: appid.rocketChatAccount
            roomId: appid.selectedRoomID
            clip: true
            useMenuMessage: true

            onPinMessage: {
                appid.rocketChatAccount.pinMessage(messageId, pinned)
            }

            onEditMessage: {
//                userInputMessage.messageId = messageId;
//                userInputMessage.setOriginalMessage(messageStr)
                //console.log(RuqolaDebugCategorySingleton.category, "edit! messageId : " + messageId + " messageStr " + messageStr)
            }
            onCopyMessage: {
                clipboard.text = messageStr
                console.log(RuqolaDebugCategorySingleton.category, "copy! messageId : " + messageId + " messageStr " + messageStr)
            }
            onReplyMessage: {
//                console.log(RuqolaDebugCategorySingleton.category, "Not implemented reply message : " + messageId)
//                userInputMessage.messageId = messageId;
            }
            onSetFavoriteMessage: {
                appid.rocketChatAccount.starMessage(messageId, starred)
            }
            onIgnoreUser: {
//                appid.rocketChatAccount.ignoreUser(roomId, userId, ignored)
            }
            onCreateDiscussion: {
//                createDiscussionDialogLoader.roomId = roomId
//                createDiscussionDialogLoader.messageId = messageId
//                createDiscussionDialogLoader.active = true
                //TODO add message text too
            }

            onOpenChannel: {
//                openChannelDialogLoader.channelName = channel
//                openChannelDialogLoader.active = true;
            }

            onOpenDirectChannel: {
//                openDirectChannelDialog.username = userName;
//                openDirectChannelDialog.open()
            }
            onJitsiCallConfActivated: {
//                appid.rocketChatAccount.joinJitsiConfCall(roomId)
            }
            onReportMessage: {
//                reportMessageDialogLoader.messageId = messageId
//                reportMessageDialogLoader.active = true
            }

            onDeleteMessage: {
//                deleteMessageDialogLoader.messageId = messageId
//                deleteMessageDialogLoader.active = true
            }
            onDownloadAttachment: {
//                downloadFileDialog.fileToSaveUrl = url
//                downloadFileDialog.open()
            }

            onDisplayImage: {
//                displayImageDialog.iUrl = imageUrl
//                displayImageDialog.title = title
//                displayImageDialog.isAnimatedImage = isAnimatedImage
//                displayImageDialog.clearScaleAndOpen();
            }
            onDeleteReaction: {
                appid.rocketChatAccount.reactOnMessage(messageId, emoji, false)
            }
            onAddReaction: {
                appid.rocketChatAccount.reactOnMessage(messageId, emoji, true)
            }

            onOpenThread: {
//                appid.rocketChatAccount.getThreadMessages(threadMessageId)
//                showThreadMessageDialogLoader.threadMessageId = threadMessageId;
//                showThreadMessageDialogLoader.active = true
            }

            onOpenDiscussion: {
                //console.log(RuqolaDebugCategorySingleton.category, "Open discussion " + discussionRoomId)
//                appid.switchToRoom(discussionRoomId)
            }
            onReplyInThread:  {
                //console.log(RuqolaDebugCategorySingleton.category,"reply in thread " + messageId)
//                userInputMessage.threadmessageId = messageId;
            }
        }
        //TODO improve it.
        UserInput {
            inputTextManager: appid.rocketChatAccount.inputThreadMessageTextManager

            rcAccount: appid.rocketChatAccount
            visible: appid.selectedRoom && (appid.selectedRoom.roomMessageInfo.length === 0)
            messageLineText: rcAccount.getUserCurrentMessage(appid.selectedRoomID)
            selectedRoomId: appid.selectedRoomID
            selectedThreadMessage: threadMessageId


            onUploadFile: {
                uploadFileDialogLoader.active = true
            }
        }
    }
}
