/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>
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

import org.kde.kirigami 2.1 as Kirigami
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Layouts 1.1
import KDE.Ruqola.RuqolaUtils 1.0
import KDE.Ruqola.ExtraColors 1.0
import KDE.Ruqola.DebugCategory 1.0
import "../js/message.js" as MessageScript;
import "../common"
MessageBase {

    property string i_messageID
    property var i_urls
    property var i_attachments
    property string i_own_username
    property bool i_can_editing_message
    property bool i_starred

    id: messageMain
    Layout.alignment: Qt.AlignTop
    MessageMenu {
        id: menu
        can_editing_message: i_can_editing_message
        starred: i_starred
    }

    RowLayout {
        AvatarImage {
            id: avatarRect
            avatarurl: i_avatar
            aliasname: i_aliasname
            username: i_username
        }
        ColumnLayout {
            Layout.fillHeight: true
            Kirigami.Heading {
                id: usernameLabel

                level: 5
                font.bold: true
                text: i_aliasname +  ' ' + i_usernameurl + (i_editedByUserName === "" ? "" : " " + i18n("(edited by %1)", i_editedByUserName))

                anchors.right: parent.right
                anchors.left: parent.left
                height: avatarRect.height
                onLinkActivated: messageMain.linkActivated(link)
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
            Column {
                id: fullTextColumn

                Layout.fillWidth: true
                Text {
                    width: parent.width
                    id: textLabel

                    renderType: Text.NativeRendering
                    textFormat: Text.RichText


                    text: i_messageText
                    wrapMode: QQC2.Label.Wrap

                    onLinkActivated: messageMain.linkActivated(link)
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
                Column {
                    id: urlColumn
                    width: parent.width
//                    Repeater {
//                        id: repeaterUrl

//                        model: i_urls
//                        Text {
//                            //Display it only if url != text otherwise it's not necessary
//                            visible: model.modelData.url !== i_originalMessage
//                            width: urlColumn.width
//                            text: model.modelData.description === ""  ?
//                                      RuqolaUtils.markdownToRichText(model.modelData.url) :
//                                      RuqolaUtils.markdownToRichText(model.modelData.description)
//                            wrapMode: QQC2.Label.Wrap
//                            renderType: Text.NativeRendering
//                            textFormat: Text.RichText

//                            onLinkActivated: messageMain.linkActivated(link)
//                        }
//                    }
                    Repeater {
                        id: repearterAttachments

                        model: i_attachments
                        Column {
                            Text {
                                visible: model.modelData.authorName !== ""
                                width: urlColumn.width
                                text: model.modelData.authorName
                                wrapMode: QQC2.Label.Wrap
                                anchors.leftMargin: Kirigami.Units.smallSpacing
                                anchors.rightMargin: Kirigami.Units.smallSpacing
                            }
                            Row {
                                Text {
                                    id: attachmentTitle
                                    renderType: Text.NativeRendering
                                    textFormat: Text.RichText
                                    visible: model.modelData.title !== ""
                                    width: urlColumn.width
                                    text: model.modelData.displayTitle
                                    wrapMode: QQC2.Label.Wrap
                                    anchors.leftMargin: Kirigami.Units.smallSpacing
                                    anchors.rightMargin: Kirigami.Units.smallSpacing
                                    onLinkActivated: {
                                        messageMain.linkActivated(link)
                                    }
                                }
                                DownloadButton {
                                    id: downloadButton
                                    visible: model.modelData.canDownloadAttachment
                                    onDownloadButtonClicked: {
                                        messageMain.downloadAttachment(model.modelData.link)
                                    }
                                }
                                Item {
                                    Layout.fillWidth: true
                                }
                            }
                            Text {
                                visible: model.modelData.description !== ""
                                width: urlColumn.width
                                text: model.modelData.description
                                wrapMode: QQC2.Label.Wrap
                                anchors.leftMargin: Kirigami.Units.smallSpacing
                                anchors.rightMargin: Kirigami.Units.smallSpacing
                            }
                        }
                    }
                }
            }
        }
        TimestampText {
            id: timestampText
            timestamp: i_timestamp
        }
    }
}
