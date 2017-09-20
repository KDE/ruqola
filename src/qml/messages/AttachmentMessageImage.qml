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
import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.RuqolaUtils 1.0
import "../js/message.js" as MessageScript;

Rectangle {

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

    color: "#eeeeee"
    //TODO fixme url height
    implicitHeight: 4*Kirigami.Units.smallSpacing + Math.max(textLabel.implicitHeight+usernameLabel.implicitHeight + 10, avatarRect.implicitHeight)

    anchors.bottomMargin: 200

    Menu {
        id: menu

        MenuItem {
            enabled: i_username == Ruqola.userName
            contentItem: Kirigami.Label {
                text: i18n("Edit")
            }
            onTriggered: {
                console.log("Edit", i_messageID, i_messageText);
                console.log("User", Ruqola.userName, i_username);
            }
        }
        MenuItem {
            contentItem: Kirigami.Label {
                text: i18n("Reply")
            }
            onTriggered: {
                console.log("Reply to", i_messageID);
            }
        }
    }



    GridLayout {
        anchors.topMargin: Kirigami.Units.smallSpacing
        anchors.fill: parent
        anchors.rightMargin: Kirigami.Units.largeSpacing
        anchors.leftMargin: Kirigami.Units.largeSpacing

        columns: 3

        //spacing: Kirigami.Units.smallSpacing


        /*
        Rectangle {
            Layout.columnSpan: 3
            id: newDateRect
            anchors.fill: parent
            Layout.fillWidth: true
            //visible: i_date !== ""
            implicitWidth: i_date.font.pixelSize * 3
            implicitHeight: i_date.font.pixelSize * 3
            anchors.rightMargin: 2*Kirigami.Units.smallSpacing

            color: "red"
            Text {
                text: i_date
            }
        }
        */
        Rectangle {
            id: avatarRect

            Layout.fillHeight: false
            implicitWidth: textLabel.font.pixelSize * 3
            implicitHeight: textLabel.font.pixelSize * 3

            radius: 3

            anchors.rightMargin: 2*Kirigami.Units.smallSpacing

            color: i_avatar !== "" ? "transparent" : MessageScript.stringToColour(i_username)
            anchors.top: parent.top
            //anchors.top: newDateRect.bottom
            Image {
                id: avatarImage

                anchors.fill: parent
                visible: i_avatar !== ""
                source: i_avatar
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
                minimumPointSize: theme.defaultFont.pointSize
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

        Rectangle {
            id: textRect

            Layout.fillWidth: true
            radius: 3
            anchors.top: parent.top
            //anchors.top: newDateRect.bottom
            anchors.bottom: parent.bottom
            anchors.bottomMargin: Kirigami.Units.smallSpacing
            anchors.leftMargin: Kirigami.Units.smallSpacing

            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.RightButton

                onClicked: {
                    console.log("clicked");
                    if (mouse.button == Qt.RightButton) {
                        menu.x = mouse.x
                        menu.y = mouse.y
                        menu.open();
                        console.log("Menu opened", mouse.x);

                    }
                }
            }

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


                    text: MessageScript.markdownme(RuqolaUtils.markdownToRichText(i_messageText))
                    wrapMode: Label.Wrap

                    onLinkActivated: messageMain.linkActivated(link)
                }
                Repeater {
                    id: repeaterUrl

                    model: i_urls
                    Text {
                        text: MessageScript.markdownme(RuqolaUtils.markdownToRichText(model.modelData.url))
                        color: "white"
                        anchors.leftMargin: Kirigami.Units.smallSpacing
                        anchors.rightMargin: Kirigami.Units.smallSpacing
                        anchors.right: parent.right
                        anchors.left: parent.left
                        renderType: Text.NativeRendering
                        textFormat: Text.RichText

                        onLinkActivated: messageMain.linkActivated(link)
                    }
                }
                Repeater {
                    id: repearterAttachments

                    model: i_attachments
                    Row {
                         Text {
                             text: model.modelData.title
                             width: 120
                         }
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
    }
}
/*
import QtQuick 2.0

import QtQuick.Controls 2.2
import org.kde.kirigami 2.1 as Kirigami

import QtQuick.Layouts 1.1
import KDE.Ruqola.ExtraColors 1.0
Rectangle {
    id: attachmentImage

    signal linkActivated(string link)
    color: RuqolaSingleton.backgroundColor;
    implicitHeight: 2*Kirigami.Units.smallSpacing + textLabel.implicitHeight

    implicitWidth: 150

    anchors.bottomMargin: 200

    Rectangle {

        anchors.rightMargin: 2*Kirigami.Units.largeSpacing
        anchors.leftMargin: 2*Kirigami.Units.largeSpacing

        anchors.centerIn: parent

        width: textLabel.implicitWidth + 6*Kirigami.Units.smallSpacing
        height: textLabel.height

        color: Kirigami.Theme.disabledTextColor
        radius: 4*Kirigami.Units.smallSpacing

    }

    Kirigami.Label {
        id: textLabel

        color: Kirigami.Theme.textColor
        opacity: 1

        anchors.centerIn: parent
        anchors.leftMargin: Kirigami.Units.smallSpacing
        anchors.rightMargin: Kirigami.Units.smallSpacing

        width: Math.min(implicitWidth, parent.width - Kirigami.Units.largeSpacing)

        text: i_username + i18n(" Image Message ")

        wrapMode: Label.Wrap

        renderType: Text.NativeRendering

        onLinkActivated: attachmentImage.linkActivated(link)
    }
}
*/
