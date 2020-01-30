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

import org.kde.kirigami 2.7 as Kirigami
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Layouts 1.12
import Ruqola 1.0

import "../common"

MessageBase {
    id: root

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
            showTranslatedMessage: i_showTranslatedMessage
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
                messageMain.showUserInfo(i_own_username)
            }
            visible: !i_groupable
        }
        ColumnLayout {
            Layout.fillHeight: true
            spacing: Kirigami.Units.smallSpacing / 2 // reduce spacing a little
            GridLayout {
                rowSpacing: 0
                columnSpacing: Kirigami.Units.smallSpacing
                columns: compactViewMode ? -1 : 2 // user name label + roles info in one row

                QQC2.Label {
                    id: usernameLabel
                    Layout.columnSpan: rolesInfo.visible ? 1 : 2
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    font.bold: true
                    text: i_aliasname +  ' ' + i_usernameurl + (i_editedByUserName === "" ? "" : " " + i18n("(edited by %1)", i_editedByUserName))

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
                Kirigami.Icon {
                    id: rolesInfo
                    Layout.alignment: Qt.AlignTop
                    source: "documentinfo"
                    width: height
                    height: 18
                    visible: i_roles.length > 0
                    opacity: rolesInfoMA.containsMouse ? 1.0 : 0.6
                    MouseArea {
                        id: rolesInfoMA
                        hoverEnabled: true
                        anchors.fill: parent
                    }
                    QQC2.ToolTip.visible: rolesInfoMA.containsMouse
                    QQC2.ToolTip.text: i_roles
                }

                QQC2.Label {
                    id: threadPreview

                    // TODO: I think the whole thread preview item needs to be visually redesigned...
                    /// no eliding possible with rich text, cf. QTBUG-16567, fake it
                    /// not ideal, see: https://stackoverflow.com/a/29923358
                    function elidedText(s, length) {
                        var elidedText = s.substring(0, length)
                            if (s.length > length)
                                elidedText += "..."
                        return elidedText
                    }

                    Layout.fillWidth: !compactViewMode
                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.columnSpan: 2
                    visible: i_threadPreview.length > 0
                    textFormat: Text.RichText
                    color: "red" //Convert to kirigami color
                    font.pointSize: textLabel.font.pointSize - 1
                    text: compactViewMode ? elidedText(i_threadPreview, 30) : i_threadPreview
                    wrapMode: compactViewMode ? Text.NoWrap : Text.Wrap
                    MouseArea {
                        anchors.fill: parent
                        acceptedButtons: Qt.RightButton | Qt.LeftButton

                        onClicked: {
                            //console.log("open thread "  + i_tmid)
                            messageMain.openThread(i_tmid, i_threadPreview)
                        }
                    }
                }
                QQC2.Label {
                    id: textLabel
                    Layout.fillWidth: true

                    textFormat: Text.RichText


                    text: i_messageText
                    wrapMode: QQC2.Label.Wrap

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
                }

                ColumnLayout {
                    id: urlColumn
                    Layout.fillWidth: true
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
                    //                            textFormat: Text.RichText

                    //                            onLinkActivated: messageMain.linkActivated(link)
                    //                        }
                    //                    }

                    RowLayout {
                        Layout.fillWidth: true
                        RepeaterReactions {
                            id: repearterReactions
                            model: i_reactions
                            onAddReaction: {
                                messageMain.addReaction(i_messageID, emoji)
                            }
                            onDeleteReaction: {
                                messageMain.deleteReaction(i_messageID, emoji)
                            }
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
                Layout.fillWidth: true
                onOpenThread: {
                    console.log(RuqolaDebugCategorySingleton.category, " OPen thread " + i_messageID)
                    messageMain.openThread(i_messageID, i_messageText)
                }
            }
        }
        ReactionsPopup {
            visible: i_useMenuMessage
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
