/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (C) 2017-2019 Laurent Montel <montel@kde.org>
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
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.5 as QQC2

import KDE.Ruqola.RuqolaUtils 1.0
import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.Clipboard 1.0
import KDE.Ruqola.RoomFilterProxyModel 1.0
import org.kde.kirigami 2.4 as Kirigami
import KDE.Ruqola.DebugCategory 1.0
import KDE.Ruqola.ReceiveTypingNotificationManager 1.0
import KDE.Ruqola.DebugCategory 1.0

Component {
    id: mainComponent


    Kirigami.Page {

        id: mainWidget
        leftPadding: Kirigami.Units.smallSpacing
        rightPadding: Kirigami.Units.smallSpacing
        topPadding: Kirigami.Units.smallSpacing
        bottomPadding: Kirigami.Units.smallSpacing
        actions {
            contextualActions: [
                Kirigami.Action {
                    id: editAction
                    iconName: "list-add"
                    text: i18n("Open room");
                    onTriggered: {
                        searchChannelDialog.initializeAndOpen();
                    }
                },
                Kirigami.Action {
                    iconName: "edit-symbolic"
                    text: i18n("Edit room");
                    checkable: true
                    onToggled: {
                        appid.rocketChatAccount.switchEditingMode(checked);
                        // do stuff
                    }
                },
                Kirigami.Action {
                    text: i18n("Create New Channel")
                    onTriggered: {
                        createNewChannelDialog.encryptedRoomEnabled = appid.rocketChatAccount.encryptedEnabled()
                        createNewChannelDialog.initializeAndOpen()
                    }
                },
                Kirigami.Action {
                   separator: true
                },
                Kirigami.Action {
                    text: i18n("Server Info")
                    onTriggered: {
                        serverinfodialog.rcAccount = appid.rocketChatAccount
                        serverinfodialog.open();
                    }
                }

            ]
        }

        header: Column {

            RowLayout {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 2*Kirigami.Units.smallSpacing
                visible: appid.selectedRoom

                ToolButton {
                    iconName: "favorite"
                    checkable: true
                    Binding on checked {
                        value: appid.selectedRoom && appid.selectedRoom.favorite
                    }
                    onCheckedChanged: {
                        appid.rocketChatAccount.changeFavorite(appid.selectedRoomID, checked)
                    }
                }
                Kirigami.Icon {
                    source: "encrypted"
                    //FIXME
                    height: 22
                    width: 22
                    visible: appid.selectedRoom && appid.selectedRoom.encrypted
                }
                Kirigami.Heading {
                    text: "#" + (appid.selectedRoom ? appid.selectedRoom.name : "")
                    level: 3
                    font.bold: true
                }
                Item {
                    Layout.fillWidth: true
                }
                ToolButton {
                    id: showNotification
                    iconName: "preferences-desktop-notification"
                    onClicked: {
                        notificationsDialog.roomInfo = appid.selectedRoom
                        notificationsDialog.open()
                    }
                }

                ToolButton {
                    id: showUsersButton
                    iconName: "system-users"
                    checkable: true
                }

                ToolButton {
                    id: searchMessage
                    iconName: "edit-find"
                    onClicked: {
                        searchMessageDialog.roomId = appid.selectedRoomID
                        searchMessageDialog.initializeAndOpen();
                    }
                }
                ToolButton {
                    iconName: "settings-configure"
                    onClicked: menu.open();
                    QQC2.Menu {
                        id: menu
                        y: parent.height

                        QQC2.MenuItem {
                            text: i18n("Channel Info")
                            onTriggered: {
                                var channelType = appid.selectedRoom.channelType;
                                if (channelType === "c" || channelType === "p") {
                                    //Only for debug
//                                    if (channelType === "c") {
//                                        appid.rocketChatAccount.channelInfo(appid.selectedRoom.rid);
//                                    } else {
//                                        appid.rocketChatAccount.groupInfo(appid.selectedRoom.rid);
//                                    }
                                    //For testing
                                    appid.rocketChatAccount.rolesInRoom(appid.selectedRoom.rid, channelType)
                                    channelInfoDialog.roomInfo = appid.selectedRoom
                                    channelInfoDialog.initializeAndOpen()
                                } else if (channelType === "d") {
                                    privateChannelInfoDialog.roomInfo = appid.selectedRoom
                                    privateChannelInfoDialog.initializeAndOpen()
                                } else {
                                    console.log(RuqolaDebugCategorySingleton.category,"channel type " + appid.selectedRoom.channelType)
                                }
                            }
                        }
                        //Hide it if direct channel
                        RuqolaMenuSeparator {
                        }
                        QQC2.MenuItem {
                            text: i18n("Mentions")
                            onTriggered: {
                                appid.rocketChatAccount.channelGetAllUserMentions(appid.selectedRoomID);
                                showMentionsInRoomDialog.initializeAndOpen()
                            }
                        }
                        RuqolaMenuSeparator {
                        }
                        QQC2.MenuItem {
                            text: i18n("Video Chat")
                            onTriggered: {
                                appid.rocketChatAccount.createJitsiConfCall(appid.selectedRoomID);
                            }
                        }
                        RuqolaMenuSeparator {
                        }
                        QQC2.MenuItem {
                            text: i18n("Add User In Room")
                            visible: appid.selectedRoom ? appid.selectedRoom.canBeModify : true
                            onTriggered: {
                                var channelType = appid.selectedRoom.channelType;
                                if (channelType === "c" || channelType === "p") {
                                    addUserDialog.roomInfo = appid.selectedRoom
                                    addUserDialog.roomId = appid.selectedRoomID
                                    addUserDialog.initializeAndOpen()
                                }
                            }
                        }
                        RuqolaMenuSeparator {
                            visible: appid.selectedRoom ? appid.selectedRoom.canBeModify : true
                        }
                        QQC2.MenuItem {
                            text: i18n("Take a Video Message")
                            onTriggered: {
                                takeVideoMessage.open();
                            }
                        }
                        RuqolaMenuSeparator {}
                        QQC2.MenuItem {
                            text: i18n("Load Recent History")
                            onTriggered: {
                                appid.rocketChatAccount.loadHistory(appid.selectedRoomID);
                            }
                        }
                        RuqolaMenuSeparator {
                        }
                        QQC2.MenuItem {
                            text: i18n("Show Files Attachment In Room")
                            onTriggered: {
                                appid.rocketChatAccount.roomFiles(appid.selectedRoomID, appid.selectedRoom.channelType);
                                showFilesInRoomDialog.initializeAndOpen()
                            }
                        }
                    }
                }
            }

            QQC2.Label {
                visible: appid.selectedRoom && (appid.selectedRoom.topic !== "")
                text: appid.selectedRoom ? appid.selectedRoom.topic : ""
                font.italic: true
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.margins: 2*Kirigami.Units.smallSpacing
                wrapMode: QQC2.Label.Wrap
            }
            QQC2.Label {
                visible: appid.selectedRoom && (appid.selectedRoom.announcement !== "")
                text: appid.selectedRoom ? appid.selectedRoom.announcement : ""
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.margins: 2*Kirigami.Units.smallSpacing
                wrapMode: QQC2.Label.Wrap
                onLinkActivated: {
                    RuqolaUtils.openUrl(link);
                }
            }

            Rectangle {
                color: Kirigami.Theme.textColor
                height:1
                anchors.right: parent.right
                anchors.left: parent.left
                opacity: .5
                visible: appid.selectedRoom
            }
            Flow {
                id: topBarUserList
                readonly property bool isActive: showUsersButton.checked
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Kirigami.Units.smallSpacing
                }
                opacity: topBarUserList.isActive ? 1 : 0
                Behavior on opacity { NumberAnimation { duration: 650; easing.type: Easing.InOutQuad } }

                Repeater {
                    model: parent.opacity > 0.5 ? appid.userModel : 0
                    RowLayout {
                        Kirigami.Icon {
                            source: model.iconstatus
                            //FIXME
                            height: 22
                            width: 22
                        }
                        UserMenu {
                            id: userMenu
                            userId: model.userid
                            can_manage_users: appid.selectedRoom.canChangeRoles
                            onKickUser: {
                                appid.rocketChatAccount.kickUser(appid.selectedRoomID, userId, appid.selectedRoom.channelType)
                            }
                            onChangeRole: {
                                appid.rocketChatAccount.changeRoles(appid.selectedRoomID, userId, appid.selectedRoom.channelType, type)
                            }
                            onIgnoreUser: {
                                //TODO
                            }
                            onOpenConversation: {
                                //TODO
                            }
                        }

                        QQC2.Label {
                            text: model.displayname
                            onLinkActivated: {
                                openDirectChannelDialog.username = link;
                                openDirectChannelDialog.open()
                            }
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.RightButton

                                onClicked: {
                                    if (mouse.button === Qt.RightButton) {
                                        userMenu.x = mouse.x
                                        userMenu.y = mouse.y
                                        userMenu.open();
                                    }
                                }
                            }
                        }                        
                    }
                }
                Item {
                    width: parent.width
                    height: topBarUserList.isActive ? 1 : 0
                    Rectangle {
                        height: parent.height
                        width: height > 0 ? parent.width : 0
                        anchors.centerIn: parent
                        Behavior on width { NumberAnimation { duration: 650; easing.type: Easing.InOutQuad } }
                        color: Kirigami.Theme.textColor
                    }
                }
            }
        }

        Clipboard {
            id: clipboard
        }

        ActiveChat {
            id: activeChat
            model: appid.messageModel
            rcAccount: appid.rocketChatAccount
            roomId: appid.selectedRoomID
            anchors.fill: parent
            clip: true

            QQC2.ScrollBar.vertical: QQC2.ScrollBar { }

            onEditMessage: {
                userInputMessage.messageId = messageId;
                userInputMessage.setOriginalMessage(messageStr)
                //console.log(RuqolaDebugCategorySingleton.category, "edit! messageId : " + messageId + " messageStr " + messageStr)
            }
            onCopyMessage: {
                clipboard.text = messageStr
                //console.log(RuqolaDebugCategorySingleton.category, "copy! messageId : " + messageId + " messageStr " + messageStr)
            }
            onReplyMessage: {
                console.log(RuqolaDebugCategorySingleton.category, "Not implemented reply message : " + messageId)
            }
            onSetFavoriteMessage: {
                appid.rocketChatAccount.starMessage(messageId, roomId, starred)
            }
            onIgnoreUser: {
                appid.rocketChatAccount.ignoreUser(roomId, userId, ignored)
            }

            onOpenChannel: {
                openChannelDialog.channelName = channel
                openChannelDialog.open()
            }

            onOpenDirectChannel: {
                openDirectChannelDialog.username = userName;
                openDirectChannelDialog.open()
            }
            onJitsiCallConfActivated: {
                appid.rocketChatAccount.joinJitsiConfCall(roomId)
            }
            onDeleteMessage: {
                deleteMessageDialog.msgId = messageId
                deleteMessageDialog.open()
            }
            onDownloadAttachment: {
                downloadFileDialog.fileToSaveUrl = url
                downloadFileDialog.open()
            }

            onDisplayImage: {
                displayImageDialog.iUrl = imageUrl
                displayImageDialog.title = title
                displayImageDialog.isAnimatedImage = isAnimatedImage
                displayImageDialog.clearScaleAndOpen();
            }
            onDeleteReaction: {
                appid.rocketChatAccount.reactOnMessage(messageId, emoji, false)
            }

            OpenChannelDialog {
                id: openChannelDialog
                onOpenChannel: {
                    appid.rocketChatAccount.openChannel(channelName);
                }
            }

            OpenDirectChannelDialog {
                id: openDirectChannelDialog
                onOpenDirectChannel: {
                    if (appid.rocketChatAccount.userName !== userName) {
                        appid.rocketChatAccount.openDirectChannel(userName);
                    }
                }
            }

            DeleteMessageDialog {
                id: deleteMessageDialog
                onDeleteMessage: {
                    appid.rocketChatAccount.deleteMessage(messageId, appid.selectedRoomID)
                }
            }

            DownloadFileDialog {
                id: downloadFileDialog
                onAccepted: {
                    if (fileUrl != "") {
                        console.log(RuqolaDebugCategorySingleton.category, "You chose: " + fileUrl)
                        appid.rocketChatAccount.downloadFile(fileToSaveUrl, fileUrl)
                    } else {
                        console.log(RuqolaDebugCategorySingleton.category, "No file selected");
                    }
                }
            }
            DisplayImageDialog {
                id: displayImageDialog
            }
            UploadFileDialog {
                id: uploadFileDialog
                onUploadFile: {
                    appid.rocketChatAccount.uploadFile(appid.selectedRoomID, description, messageText, filename)
                }
            }

            ShowMentionsInRoomDialog {
                id: showMentionsInRoomDialog
            }

            ShowFilesInRoomDialog {
                id: showFilesInRoomDialog
                filesModel: appid.filesModel
                onDownloadFile: {
                    downloadFileDialog.fileToSaveUrl = file
                    downloadFileDialog.open()
                }
                onDeleteFile: {
                    appid.rocketChatAccount.deleteFileMessage(appid.selectedRoomID, fileid, appid.selectedRoom.channelType)
                }
            }
            CreateNewChannelDialog {
                id: createNewChannelDialog
                onCreateNewChannel: {
                    rocketChatAccount.createNewChannel(name, readOnly, privateRoom, usernames, encryptedRoom, password);
                }
            }
            ServerInfoDialog {
                id: serverinfodialog
            }
        }

        Keys.onEscapePressed: {
            appid.rocketChatAccount.clearUnreadMessages(appid.selectedRoomID);
        }

        footer:
            ColumnLayout {
            anchors.bottom: mainWidget.bottom
            //anchors.margins: Kirigami.Units.smallSpacing
            QQC2.Label {
                id: channelInfo
                font.bold: true
                anchors {
                    margins: Kirigami.Units.smallSpacing
                }
                Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                visible: appid.selectedRoom && ((appid.selectedRoom.readOnly === true) || (appid.selectedRoom.blocker === true) || (appid.selectedRoom.blocked === true))
                text: appid.selectedRoom ? appid.selectedRoom.roomMessageInfo : ""
            }

            UserInput {
                id: userInputMessage
                rcAccount: appid.rocketChatAccount
                visible: appid.selectedRoom && (appid.selectedRoom.readOnly === false) && (appid.selectedRoom.blocker === false) && (appid.selectedRoom.blocked === false)
                messageLineText: rcAccount.getUserCurrentMessage(appid.selectedRoomID)
                onTextEditing: {
                    rcAccount.textEditing(appid.selectedRoomID, str)
                    appid.userInputMessageText = str;
                }
                onClearUnreadMessages: {
                    rcAccount.clearUnreadMessages(appid.selectedRoomID)
                }
                onUploadFile: {
                    uploadFileDialog.initializeAndOpen()
                }
            }
            QQC2.Label {
                id: typingInfo
                anchors.margins: 2*Kirigami.Units.smallSpacing
                text: ""
            }

            Connections {
                target: appid.rocketChatAccount.receiveTypingNotificationManager()
                onNotificationChanged: {
                    //console.log(RuqolaDebugCategorySingleton.category, "Typing in roomId: " + roomId + " str " + notificationStr);
                    if (appid.selectedRoomID === roomId) {
                        typingInfo.text = notificationStr;
                    }
                }
            }
        }
    }// mainWidget Item
}
