/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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

import QtQuick 2.0

import QtQuick.Controls 2.2
import org.kde.kirigami 2.1 as Kirigami
import QtQuick.Layouts 1.1
import KDE.Ruqola.RuqolaUtils 1.0
import "../js/message.js" as MessageScript;

Rectangle {
    id: attachmentFile

    property string i_messageText
    property string i_messageID
    property string i_username
    property string i_aliasname
    property string i_avatar
    property var i_timestamp
    property var i_urls
    property var i_attachments

    color: "#eeeeee"
    implicitHeight: 4*Kirigami.Units.smallSpacing + Math.max(textLabel.implicitHeight+usernameLabel.implicitHeight, avatarRect.implicitHeight)

    anchors.bottomMargin: 200


    RowLayout {

        anchors.topMargin: Kirigami.Units.smallSpacing
        anchors.fill: parent
        anchors.rightMargin: Kirigami.Units.largeSpacing
        anchors.leftMargin: Kirigami.Units.largeSpacing

        spacing: Kirigami.Units.smallSpacing

        Rectangle {
            id: avatarRect

            Layout.fillHeight: false
            implicitWidth: textLabel.font.pixelSize * 3
            implicitHeight: textLabel.font.pixelSize * 3

            radius: 3

            anchors.rightMargin: 2*Kirigami.Units.smallSpacing

            color: i_avatar !== "" ? "transparent" : MessageScript.stringToColour(i_username)

            anchors.top: parent.top
            Image {
                anchors.fill: parent
                visible: i_avatar !== ""
                source: i_avatar
            }
            Text {
                visible: i_avatar == ""
                anchors.fill: parent
                anchors.margins: Kirigami.Units.smallSpacing

                renderType: Text.QtRendering
                color: "white"

                font.weight: Font.Bold
                font.pointSize: 100
                minimumPointSize: theme.defaultFont.pointSize
                fontSizeMode: Text.Fit

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                text: {
                    //TODO verify if it works with non latin char.
                    var match = i_username.match(/([a-zA-Z])([a-zA-Z])/);
                    var abbrev = match[1].toUpperCase();
                    if (match.length > 2) {
                        abbrev += match[2].toLowerCase();
                    }
                    return abbrev;
                }
            }
        }

        Rectangle {
            id: textRect

            Layout.fillWidth: true
            radius: 3
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.bottomMargin: Kirigami.Units.smallSpacing
            anchors.leftMargin: Kirigami.Units.smallSpacing

            Column {
                anchors.leftMargin: Kirigami.Units.smallSpacing
                anchors.rightMargin: Kirigami.Units.smallSpacing
                anchors.fill: parent

                Kirigami.Heading {
                    id: usernameLabel

                    level: 5
                    font.bold: true
                    text: i_aliasname + ' @' + i_username

                    anchors.right: parent.right
                    anchors.left: parent.left
                    height: avatarRect.height
                }
                Text {
                    id: textLabel
                    anchors.right: parent.right
                    anchors.left: parent.left

                    anchors.leftMargin: Kirigami.Units.smallSpacing
                    anchors.rightMargin: Kirigami.Units.smallSpacing
                    renderType: Text.NativeRendering
                    textFormat: Text.RichText


                    text: "Message File " + MessageScript.markdownme(RuqolaUtils.markdownToRichText(i_messageText))
                    wrapMode: Label.Wrap

                    onLinkActivated: attachmentFile.linkActivated(link)
                }
            }
            Repeater {
                model: i_urls
                Row {
                     Text {
                         text: model.modelData.url
                         width: 120
                     }
                }
            }
            Repeater {
                model: i_attachments
                Row {
                     Text {
                         text: model.modelData.title
                         width: 120
                     }
                }
            }
        }
        Kirigami.Label {
            id: timestampText

            text: MessageScript.displayDateTime(i_timestamp)
            opacity: .5

            anchors.top: parent.top
            anchors.right: parent.right
            anchors.leftMargin: Kirigami.Units.smallSpacing

            z: 10
        }
        //TODO add attachments!
    }
}
