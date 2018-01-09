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

    property var i_urls
    property var i_attachments

    id: messageMain

    RowLayout {
        AvatarImage {
            id: avatarRect
            avatarurl: i_avatar
            aliasname: i_aliasname
            username: i_username
        }

        Item {
            Layout.fillWidth: true
        }
        Repeater {
            id: repearterAttachments

            model: i_attachments
            Row {
                Column {
                    Text {
                        text: model.modelData.title
                        wrapMode: QQC2.Label.Wrap
                        anchors.leftMargin: Kirigami.Units.smallSpacing
                        anchors.rightMargin: Kirigami.Units.smallSpacing
                    }
                    Image {
                        id: imageUrl
                        source: rcAccount.attachmentUrl(model.modelData.link)
                        asynchronous: true
                        //TODO customize it.
                        width: 200
                        height: 200
                        onStatusChanged: {
                            if(status == Image.Error){
                                console.log(RuqolaDebugCategorySingleton.category, "Image load error! Trying to reload. ")
                            }
                        }
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
                        console.log(RuqolaDebugCategorySingleton.category, " IMAGE SUPPORT (const QString &imageUrl, const QString &cacheImageUrl); " + filePath + " cacheIma :" + cacheImageUrl + "model.modelData.link " + model.modelData.link)
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
