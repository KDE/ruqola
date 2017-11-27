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
import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.RuqolaUtils 1.0
import KDE.Ruqola.ExtraColors 1.0
import "../js/message.js" as MessageScript;

ColumnLayout {

    property string i_messageText
    property string i_messageID
    property string i_username
    property string i_aliasname
    property string i_avatar
    property var i_timestamp
    property var i_urls
    property var i_attachments
    property var i_date

    signal linkActivated(string link)

    id: messageMain
    Layout.alignment: Qt.AlignTop
    Menu {
        id: menu

        MenuItem {
            enabled: i_username == Ruqola.userName
            contentItem: QQC2.Label {
                text: i18n("Edit")
            }
            onTriggered: {
                console.log("Edit", i_messageID, i_messageText);
                console.log("User", Ruqola.userName, i_username);
            }
        }
        MenuItem {
            contentItem: QQC2.Label {
                text: i18n("Reply")
            }
            onTriggered: {
                console.log("Reply to", i_messageID);
            }
        }
    }


    NewDateLabel {
        id: newDateRect
        date: i_date
    }

    RowLayout {
        Rectangle {
            id: avatarRect

            Layout.alignment: Qt.AlignTop
            Layout.fillHeight: false
            implicitWidth: textLabel.font.pixelSize * 3
            implicitHeight: textLabel.font.pixelSize * 3

            radius: 3

            anchors.rightMargin: 2*Kirigami.Units.smallSpacing

            color: i_avatar !== "" ? "transparent" : MessageScript.stringToColour(i_username)
            Image {
                id: avatarImage

                anchors.fill: parent
                visible: i_avatar !== ""
                source: i_avatar
                fillMode: Image.PreserveAspectFit
            }
            Text {
                id: avatarText

                visible: i_avatar == ""
                anchors.fill: parent
                anchors.margins: Kirigami.Units.smallSpacing

                renderType: Text.QtRendering
                color: "white"

                font.weight: Font.Bold
                font.pointSize: 100
                fontSizeMode: Text.Fit

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                text: {
                    //TODO verify if it works with non latin char.
                    var match = i_aliasname.match(/([a-zA-Z])([a-zA-Z])/);
                    var abbrev = match[1].toUpperCase();
                    if (match.length > 2) {
                        abbrev += match[2].toLowerCase();
                    }
                    return abbrev;
                }
            }
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
                        console.log("clicked");
                        if (mouse.button === Qt.RightButton) {
                            menu.x = mouse.x
                            menu.y = mouse.y
                            menu.open();
                            console.log("Menu opened", mouse.x);
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

        QQC2.Label {
            id: timestampText

            Layout.alignment: Qt.AlignTop | Qt.AlignRight
            text: MessageScript.displayDateTime(i_timestamp)
            opacity: .5

            z: 10
        }
    }
}
