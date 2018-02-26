/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (C) 2017-2018 Laurent Montel <montel@kde.org>
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
import QtQuick.Controls 2.2 as QQC2

import KDE.Ruqola.RuqolaUtils 1.0
import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.Clipboard 1.0
import KDE.Ruqola.RoomFilterProxyModel 1.0
import org.kde.kirigami 2.1 as Kirigami
import KDE.Ruqola.DebugCategory 1.0

Component {
    id: mainComponent


    Kirigami.Page {

        id: mainWidget
        leftPadding: Kirigami.Units.smallSpacing
        rightPadding: Kirigami.Units.smallSpacing
        topPadding: Kirigami.Units.smallSpacing
        bottomPadding: Kirigami.Units.smallSpacing

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
                Kirigami.Heading {
                    text: "#" + (appid.selectedRoom ? appid.selectedRoom.name : "")
                    level: 3
                    font.bold: true
                }
                Item {
                    Layout.fillWidth: true
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
                            text: i18n("Create New Channel")
                            onTriggered: {
                                createNewChannelDialog.initializeAndOpen()
                            }
                        }
                        RuqolaMenuSeparator {
                        }
                        QQC2.MenuItem {
                            text: i18n("Channel Info")
                            onTriggered: {
                                channelInfoDialog.initializeAndOpen()
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
                            onTriggered: {
                                addUserDialog.roomId = appid.selectedRoomID
                                addUserDialog.initializeAndOpen()
                            }
                        }
                        RuqolaMenuSeparator {
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
                                appid.rocketChatAccount.roomFiles(appid.selectedRoomID);
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
                        Text {
                            text: model.displayname
                            onLinkActivated: {
                                openDirectChannelDialog.username = link;
                                openDirectChannelDialog.open()
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
                console.log(RuqolaDebugCategorySingleton.category, "edit! messageId : " + messageId + " messageStr " + messageStr)
            }
            onCopyMessage: {
                clipboard.text = messageStr
                console.log(RuqolaDebugCategorySingleton.category, "copy! messageId : " + messageId + " messageStr " + messageStr)
            }
            onReplyMessage: {
                console.log(RuqolaDebugCategorySingleton.category, "Not implemented reply message : " + messageId)
            }
            onSetFavoriteMessage: {
                appid.rocketChatAccount.starMessage(messageId, roomId, starred)
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
                displayImageDialog.clearScaleAndOpen();
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
                    appid.rocketChatAccount.openDirectChannel(userName);
                }
            }

            DeleteMessageDialog {
                id: deleteMessageDialog
                onDeleteMessage: {
                    appid.rocketChatAccount.deleteMessage(messageId)
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
                    appid.rocketChatAccount.uploadFile(description, filename)
                }
            }

            ShowFilesInRoomDialog {
                id: showFilesInRoomDialog
                filesModel: appid.filesModel
                onDownloadFile: {
                    downloadFileDialog.fileToSaveUrl = file
                    downloadFileDialog.open()
                }
            }
        }

        footer: UserInput {
            id: userInputMessage
            rcAccount: appid.rocketChatAccount
            visible: appid.selectedRoom && (appid.selectedRoom.readOnly === false) && appid.selectedRoom.blocker === false
            anchors.bottom: mainWidget.bottom
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
    }// mainWidget Item
}
