/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
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

import QtQuick.Controls 2.2
import org.kde.kirigami 2.1 as Kirigami
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Layouts 1.1
import KDE.Ruqola.RuqolaUtils 1.0
import KDE.Ruqola.ExtraColors 1.0
import KDE.Ruqola.DebugCategory 1.0
import "../js/message.js" as MessageScript;
MessageBase {

    property string i_messageText
    property string i_messageID
    property string i_username
    property string i_aliasname
    property string i_avatar
    property var i_timestamp
    property var i_urls
    property var i_attachments
    property string i_own_username

    id: messageMain
    Layout.alignment: Qt.AlignTop
    MessageMenu {
        id: menu
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
                text: i_aliasname + ' @' + i_username

                anchors.right: parent.right
                anchors.left: parent.left
                height: avatarRect.height
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton

                    onClicked: {
                        console.log(RuqolaDebugCategorySingleton.category, "clicked");
                        if (mouse.button === Qt.RightButton) {
                            menu.x = mouse.x
                            menu.y = mouse.y
                            menu.open();
                            console.log(RuqolaDebugCategorySingleton.category, "Menu opened", mouse.x);
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


                    text: MessageScript.markdownme(i_messageText)
                    wrapMode: Label.Wrap

                    onLinkActivated: messageMain.linkActivated(link)
                }
                Column {
                    id: urlColumn
                    width: parent.width
                    Repeater {
                        id: repeaterUrl

                        model: i_urls
                        Text {
                            width: urlColumn.width
                            text: model.modelData.description === ""  ?
                                      MessageScript.markdownme(RuqolaUtils.markdownToRichText(model.modelData.url)) :
                                      MessageScript.markdownme(RuqolaUtils.markdownToRichText(model.modelData.description))
                            wrapMode: Label.Wrap
                            renderType: Text.NativeRendering
                            textFormat: Text.RichText

                            onLinkActivated: messageMain.linkActivated(link)
                        }
                    }
                    Repeater {
                        id: repearterAttachments

                        model: i_attachments
                        Text {
                            width: urlColumn.width
                            text: model.modelData.title
                            wrapMode: Label.Wrap
                            anchors.leftMargin: Kirigami.Units.smallSpacing
                            anchors.rightMargin: Kirigami.Units.smallSpacing
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
