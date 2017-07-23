/*
 * <one line to give the program's name and a brief idea of what it does.>
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


import QtQuick 2.0
// import "marked.js" as Markdown
import "js/marked.js" as MarkDown;

Rectangle {

    function getTextFor(type) {

//         console.log(JSON.stringify(markdown));
        if (type == "uj") {
            return qsTr("has joined the channel");
        } else if (type == "ul") {
            return qsTr("has left the channel");
        } else {
            return qsTr("Unknown action!");
        }
    }

    property string i_messageText
    property string i_username
    property bool i_systemMessage
    property string i_systemMessageType

    id: messageMain
    color: "#eeeeee"
    implicitHeight: textLabel.contentHeight


    Text {
        color: i_systemMessage? "#999" : "#555"
        text: i_username
        id: usernameLabel
        width: 80
        horizontalAlignment: Text.AlignRight

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

    Text {
        color: i_systemMessage? "#999" : "#111"
        id: textLabel
        text: i_systemMessage? getTextFor(i_systemMessageType) : MarkDown.md2html(i_messageText)

//         width: parent.width

        anchors.top: parent.top
        anchors.left: usernameLabel.right
        anchors.right: parent.right

        wrapMode:Text.Wrap

        anchors.leftMargin: 5

//         visible: !i_systemMessage
        //         horizontalAlignment: Text.AlignRight
    }


}
