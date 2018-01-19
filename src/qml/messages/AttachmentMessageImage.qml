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

import QtQuick.Controls 2.2 as QQC2
import org.kde.kirigami 2.1 as Kirigami

import KDE.Ruqola.DebugCategory 1.0
import QtQuick.Layouts 1.1
import "../js/message.js" as MessageScript;

MessageBase {


    id: messageMain

    RowLayout {
        AvatarImage {
            id: avatarRect
            avatarurl: i_avatar
            aliasname: i_aliasname
            username: i_username
        }

        Repeater {
            id: repearterAttachments

            model: i_attachments
            Row {
                Column {
                    Text {
                        id: imageTitle
                        //TODO make it clickable ?
                        text: model.modelData.title === "" ? "" :  i18n("File send: %1", model.modelData.title)
                        visible: model.modelData.title !== ""
                        wrapMode: QQC2.Label.Wrap
                        anchors.leftMargin: Kirigami.Units.smallSpacing
                        anchors.rightMargin: Kirigami.Units.smallSpacing
                    }
                    Image {
                        id: imageUrl
                        readonly property int imageHeight: model.modelData.imageHeight === -1 ? 200 : model.modelData.imageHeight
                        source: rcAccount.attachmentUrl(model.modelData.link)
                        asynchronous: true
                        fillMode: Image.PreserveAspectFit
                        //TODO customize it.
                        width: model.modelData.imageWidth === -1 ? 200 : model.modelData.imageWidth
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
                                    messageMain.displayImage(imageUrl.source, imageTitle.text)
                                }
                            }
                        }
                    }
                    Text {
                        text: model.modelData.description
                        wrapMode: QQC2.Label.Wrap
                        anchors.leftMargin: Kirigami.Units.smallSpacing
                        anchors.rightMargin: Kirigami.Units.smallSpacing
                        visible: model.modelData.description !== ""
                    }
                }

                ShowHideButton {
                    targetAnimation: imageUrl
                    defaultHeight: imageUrl.imageHeight
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
                        }
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
