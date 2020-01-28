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

import Ruqola 1.0
import QtQuick.Layouts 1.12
import "../common"

MessageBase {
    id: messageMain

    Loader {
        id: messageMenuLoader
        active: false
        property var posX
        property var posY
        sourceComponent :MessageMenu {
            id: menu
            x: messageMenuLoader.posX
            y: messageMenuLoader.posY
            can_edit_message: i_can_edit_message
            starred: i_starred
            Component.onCompleted: {
                open()
            }
            onAboutToHide: {
                messageMenuLoader.active = false
            }
        }
    }

    RowLayout {
        AvatarImage {
            id: avatarRect
            avatarurl: i_avatar
            aliasname: i_aliasname
            username: i_username
            onShowUserInfo: {
                //TODO
            }
        }
        ColumnLayout {
            QQC2.Label {
                id: usernameLabel
                Layout.alignment: Qt.AlignLeft
                font.bold: true
                text: i_aliasname +  ' ' + i_usernameurl + (i_editedByUserName === "" ? "" : " " + i18n("(edited by %1)", i_editedByUserName))

                height: avatarRect.height
                onLinkActivated: messageMain.linkActivated(link)
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton

                    onClicked: {
                        if (i_useMenuMessage) {
                            if (mouse.button === Qt.RightButton) {
                                messageMenuLoader.posX = mouse.x
                                messageMenuLoader.posY = mouse.y
                                if (messageMenuLoader.active)
                                    messageMenuLoader.active = false
                                else
                                    messageMenuLoader.active = true
                            }
                        }
                    }
                }
                visible: !i_groupable
            }
            Repeater {
                id: repearterAttachments

                model: i_attachments
                Row {
                    spacing: Kirigami.Units.smallSpacing
                    Column {
                        QQC2.Label {
                            id: imageTitle
                            text: model.modelData.title === "" ? "" :  model.modelData.imageTitle
                            visible: model.modelData.title !== ""
                            wrapMode: QQC2.Label.NoWrap
                            anchors.leftMargin: Kirigami.Units.smallSpacing
                            anchors.rightMargin: Kirigami.Units.smallSpacing
                            textFormat: Text.RichText
                            onLinkActivated: {
                                messageMain.displayImage(imageUrl.source, model.modelData.title, model.modelData.isAnimatedImage)
                            }
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.RightButton

                                onClicked: {
                                    if (mouse.button === Qt.RightButton) {
                                        messageMenuLoader.posX = mouse.x
                                        messageMenuLoader.posY = mouse.y
                                        if (messageMenuLoader.active)
                                            messageMenuLoader.active = false
                                        else
                                            messageMenuLoader.active = true
                                    }
                                }
                            }
                        }
                        Image {
                            id: imageUrl
                            visible: model.modelData.isAnimatedImage
                            readonly property int imageHeight: model.modelData.imageHeight === -1 ? 200 : Math.min(200, model.modelData.imageHeight)
                            source: rcAccount.attachmentUrl(model.modelData.link)
                            asynchronous: true
                            fillMode: Image.PreserveAspectFit
                            //Don't use really imageWidth otherwise it will be too big
                            width: 200 //model.modelData.imageWidth === -1 ? 200 : model.modelData.imageWidth
                            height: 0
                            sourceSize.width: 200
                            sourceSize.height: 200

                            onStatusChanged: {
                                if(status == Image.Error){
                                    console.log(RuqolaDebugCategorySingleton.category, "Image load error! Trying to reload. " + source)
                                }
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    if(status === Image.Error) {
                                        console.log(RuqolaDebugCategorySingleton.category, "Image not loaded.");
                                    } else {
                                        messageMain.displayImage(imageUrl.source, imageTitle.text, model.modelData.isAnimatedImage)
                                    }
                                }
                            }
                        }
                        AnimatedImage {
                            id: imageAnimatedUrl
                            visible: model.modelData.isAnimatedImage
                            readonly property int imageHeight: model.modelData.imageHeight === -1 ? 200 : Math.min(200, model.modelData.imageHeight)
                            source: rcAccount.attachmentUrl(model.modelData.link)
                            asynchronous: true
                            fillMode: Image.PreserveAspectFit
                            //Don't use really imageWidth otherwise it will be too big
                            width: 200 //model.modelData.imageWidth === -1 ? 200 : model.modelData.imageWidth
                            height: 0

                            onStatusChanged: {
                                if(status == Image.Error){
                                    console.log(RuqolaDebugCategorySingleton.category, "Image load error! Trying to reload. " + source)
                                }
                            }
                            onHeightChanged: {
                                playing = height > 0;
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    if(status === Image.Error) {
                                        console.log(RuqolaDebugCategorySingleton.category, "Image not loaded.");
                                    } else {
                                        messageMain.displayImage(imageAnimatedUrl.source, imageTitle.text, model.modelData.isAnimatedImage)
                                    }
                                }
                            }
                        }
                        QQC2.Label {
                            text: model.modelData.description
                            wrapMode: QQC2.Label.Wrap
                            anchors.leftMargin: Kirigami.Units.smallSpacing
                            anchors.rightMargin: Kirigami.Units.smallSpacing
                            visible: model.modelData.description !== ""
                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.RightButton

                                onClicked: {
                                    if (mouse.button === Qt.RightButton) {
                                        menu.x = mouse.x
                                        menu.y = mouse.y
                                        menu.open();
                                    }
                                }
                            }
                        }
                    }

                    ShowHideButton {
                        targetAnimation: model.modelData.isAnimatedImage ? imageAnimatedUrl : imageUrl
                        defaultHeight: model.modelData.isAnimatedImage ? imageAnimatedUrl.imageHeight : Url.imageHeight
                        onHiddenChanged: {
                            messageMain.showDisplayAttachment(i_messageID, state)
                        }
                    }

                    DownloadButton {
                        id: download
                        onDownloadButtonClicked: {
                            messageMain.downloadAttachment(model.modelData.link)
                        }
                    }
                    Connections {
                        target: rcAccount
                        onFileDownloaded: {
                            //console.log(RuqolaDebugCategorySingleton.category, " IMAGE SUPPORT: " + filePath + " cacheImageUrl :" + cacheImageUrl + " model.modelData.link: " + model.modelData.link)
                            if (filePath === model.modelData.link) {
                                console.log(RuqolaDebugCategorySingleton.category, "Image updated: " + cacheImageUrl)
                                imageUrl.source = cacheImageUrl;
                                imageAnimatedUrl.source = cacheImageUrl;
                            }
                        }
                    }
                }
            }
            RowLayout {
                RepeaterReactions {
                    id: repearterReactions
                    model: i_reactions
                    onDeleteReaction: {
                        messageMain.deleteReaction(i_messageID, emoji)
                    }
                }
            }
        }

        Item {
            Layout.fillWidth: true
        }
        TimestampText {
            id: timestampText
            timestamp: i_timestamp
        }
    }
}
