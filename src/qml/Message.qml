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

import QtQuick.Controls 2.2
import org.kde.kirigami 2.1 as Kirigami

Rectangle {

    function getTextFor(type) {

        //         console.log(JSON.stringify(markdown));
        
        if (type === "uj") {
            return qsTr("has joined the channel");
        } else if (type === "ul") {
            return qsTr("has left the channel");
        } else if (type === "room_changed_topic") {
            return qsTr("changed topic to \"%1\"").arg(i_messageText)
        } else if (type === "au") {
            return qsTr("added %1 to the conversation").arg(i_messageText)
        } else if (type === "r") {
            return qsTr("changed room name to \"%1\"").arg(i_messageText)
        } else if (type === "room_changed_description") {
            return qsTr("changed room description to \"%1\"").arg(i_messageText)
        } else {
            console.log("Unkown type for message");
            console.log(type);
            console.log(i_messageText)
            return qsTr("Unknown action!");
        }
    }
    
    //     function timestampConverter(time){
    //         var ret;
    //         var date = new Date(time*1000);// hours part from the timestamp
    //         var hours = date.getHours(); // minutes part from the timestamp
    //         var minutes = date.getMinutes();// seconds part from the timestamp
    //         var seconds = date.getSeconds();
    //         var day;
    //         var num = date.getDate();
    //         var year = date.getFullYear(); // will display time in hh:mm:ss format
    //         var formattedTime = hours + ':' + minutes + ':' + seconds;
    //         var formattedDate = day + " " + num + " " + year;
    //
    //         console.debug("Time : " + formattedTime + " " + formattedDate);
    //     }


    property string i_messageText
    property string i_username
    property bool i_systemMessage
    property string i_systemMessageType
    property string i_avatar
    property var i_timestamp

    id: messageMain
    color: "#eeeeee"
    implicitHeight: textLabel.contentHeight

    
    //     Component.onCompleted: {
    // //         console.log(i_timestamp)
    //         var date = new Date(i_timestamp);
    //         var hours = date.getHours(); // minutes part from the timestamp
    //         var minutes = date.getMinutes();// seconds part from the timestamp
    //         var seconds = date.getSeconds();
    //         var num = date.getDate();
    //         var formattedTime = hours + ':' + minutes + ':' + seconds;
    //
    //         var iso = date.toISOString();
    // //         console.log(d.format("HH"))
    // //         var dd = Date.fromLocaleString(Qt.locale(), d)
    //         console.log(date.toISOString());
    //         console.log(formattedTime);
    //         console.log();
    //
    //     }
    //     implicitWidth: 200
    //     Row {
    //         anchors.fill: parent
    //         spacing: Kirigami.Units.smallSpacing
    //         width: 200
    Label {
        id: timeLabel
        text: "["+(new Date(i_timestamp)).toLocaleTimeString(Locale.ShortFormat)+"]"

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        z:1
    }
    
    Label {
        color: i_systemMessage? "#999" : "#555"
        text: i_username
        id: usernameLabel
        clip: true

        horizontalAlignment: Text.AlignRight

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: timeLabel.left

        width: 100
    }

    Label {
        color: i_systemMessage? "#999" : "#111"
        id: textLabel
        text: i_systemMessage? getTextFor(i_systemMessageType) : MarkDown.md2html(i_messageText)


        anchors.top: parent.top
        anchors.bottom: parent.bottom

        anchors.left: usernameLabel.right
        anchors.right: parent.right

        //         width:

        wrapMode: Label.Wrap

        anchors.leftMargin: 5

        //         visible: !i_systemMessage
        //         horizontalAlignment: Text.AlignRight
    }
    //     }

}
