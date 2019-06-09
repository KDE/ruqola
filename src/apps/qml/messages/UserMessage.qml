/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>
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

import org.kde.kirigami 2.7 as Kirigami
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Layouts 1.12
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

    id: messageMain
    Layout.alignment: Qt.AlignTop
    Loader {
        id: messageMenuLoader
        active: false
        property var posX
        property var posY
        sourceComponent: MessageMenu {
            id: menu
            x: messageMenuLoader.posX
            y: messageMenuLoader.posY
            can_edit_message: i_can_edit_message
            user_ignored : i_user_ignored
            starred: i_starred
            pinned_message: i_pinned
            Component.onCompleted: {
                open()
            }
            onAboutToHide: {
                messageMenuLoader.active = false;
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
                console.log(RuqolaDebugCategorySingleton.category, "show user info not implemented yet")
                //TODO
            }
            visible: !i_groupable
        }
        ColumnLayout {
            Layout.fillHeight: true
            Kirigami.Heading {
                id: usernameLabel

                level: 5
                Layout.alignment: Qt.AlignLeft
                font.bold: true
                text: i_aliasname +  ' ' + i_usernameurl + (i_editedByUserName === "" ? "" : " " + i18n("(edited by %1)", i_editedByUserName))

                height: avatarRect.height
                onLinkActivated: messageMain.linkActivated(link)
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
                visible: !i_groupable

            }

            RowLayout {
                RepeaterRoles {
                    id: repearterRoles
                    model: i_roles
                    Layout.fillWidth: true
                }
            }

            Column {
                id: fullTextColumn

                Layout.fillWidth: true
                QQC2.Label {
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
                Column {
                    id: urlColumn
                    width: parent.width
                    //TODO
                    //Reactivate when we have a parsed url !
                    //see info about bugs

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

                    RepeaterReactions {
                        id: repearterReactions
                        model: i_reactions
                        onDeleteReaction: {
                            messageMain.deleteReaction(i_messageID, emoji)
                        }
                    }
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
                                QQC2.Label {
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
                            QQC2.Label {
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
            ThreadLabel {
                onOpenThread: {
                    console.log(RuqolaDebugCategorySingleton.category, " OPen thread " + i_messageID)
                    messageMain.openThread(i_messageID)
                }
            }
        }
        ReactionsPopup {
            onInsertReaction: {
                messageMain.addReaction(i_messageID, emoji)
            }
        }

        TimestampText {
            id: timestampText
            timestamp: i_timestamp
            visible: !i_groupable
        }
    }
}
