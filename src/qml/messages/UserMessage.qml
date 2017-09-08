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


import QtQuick 2.0
import "../js/marked.js" as MarkDown;

import QtQuick.Controls 2.2
import org.kde.kirigami 2.1 as Kirigami

import QtQuick.Layouts 1.1
import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.RuqolaUtils 1.0


Rectangle {
    /*
 * Author: Johnny Broadway <johnny@johnnybroadway.com>
 * Website: https://gist.github.com/jbroadway/2836900
 * License: MIT
 */
    function slimdown() {

        // Rules
        this.rules =  [
                 {regex: /\n(#+)(.*)/g, replacement: header},                                         // headers
                 {regex: /!\[([^\[]+)\]\(([^\)]+)\)/g, replacement: '<img src=\'$2\' alt=\'$1\'>'}, // image
                 {regex: /\[([^\[]+)\]\(([^\)]+)\)/g, replacement: '<a href=\'$2\'>$1</a>'},        // hyperlink
                 //{regex: /(_)(.*?)\1/g, replacement: '<i>$2</i>'},                             // emphasis
                 {regex: /\~\~(.*?)\~\~/g, replacement: '<del>$1</del>'},                           // del
                 {regex: /\:\"(.*?)\"\:/g, replacement: '<q>$1</q>'},                               // quote
                 {regex: /`(.*?)`/g, replacement: '<code>$1</code>'},                               // inline code
                 //{regex: /\n\*(.*)/g, replacement: ulList},                                         // ul lists
                 {regex: /\n[0-9]+\.(.*)/g, replacement: olList},                                   // ol lists
                 {regex: /\n(&gt;|\>)(.*)/g, replacement: blockquote},                              // blockquotes
                 {regex: /\n-{5,}/g, replacement: '\n<hr />'},                                      // horizontal rule
                 {regex: /\n([^\n]+)\n/g, replacement: para},                                       // add paragraphs
                 {regex: /<\/ul>\s?<ul>/g, replacement: ''},                                        // fix extra ul
                 {regex: /<\/ol>\s?<ol>/g, replacement: ''},                                        // fix extra ol
                 {regex: /<\/blockquote><blockquote>/g, replacement: '\n'}                          // fix extra blockquote
             ];

        // Add a rule.
        this.addRule = function (regex, replacement) {
            regex.global = true;
            regex.multiline = false;
            this.rules.push({regex: regex, replacement: replacement});
        };

        // Render some Markdown into HTML.
        this.render = function (text) {
            text = '\n' + text + '\n';
            this.rules.forEach(function (rule) {
                text = text.replace(rule.regex, rule.replacement);
            });
            return text.trim();
        };

        function para (text, line) {
            debugger;
            var trimmed = line.trim();
            if (/^<\/?(ul|ol|li|h|p|bl)/i.test(trimmed)) {
                return '\n' + line + '\n';
            }
            return '\n<p>' + trimmed + '</p>\n';
        }

        function ulList (text, item) {
            return '\n<ul>\n\t<li>' + item.trim() + '</li>\n</ul>';
        }

        function olList (text, item) {
            return '\n<ol>\n\t<li>' + item.trim() + '</li>\n</ol>';
        }

        function blockquote (text, tmp, item) {
            return '\n<blockquote>' + item.trim() + '</blockquote>';
        }

        function header (text, chars, content) {
            var level = chars.length;
            return '<h' + level + '>' + content.trim() + '</h' + level + '>';
        }
    }

    
    function markdownme(s) {
        //         var md = MarkDown.markdownit();
        //         var result = md.render('# markdown-it rulezz!');
        var sd = new slimdown();
        var result= sd.render(s);
        
        //         var regex = new RegExp(/\[([^\[]+)\]\(([^\)]+)\)/g);
        //         result = s.replace(regex, '<a href=\'$2\'>$1</a>');

        var regex2 = new RegExp(/#(\w+(?:\.\w+)?)/g);
        result = result.replace(regex2, '<a href=\'ruqola:/room/$1\'>#$1</a>');

        var regex3 = new RegExp(/@(\w+(?:\.\w+)?)/g);
        result = result.replace(regex3, '<a href=\'ruqola:/user/$1\'>@$1</a>');

        console.log(result)
        
        //         var regex = new RegExp(/\[([^\[]+)\]\(([^\)]+)\)/g);
        //         var result = s.replace(regex, '<a href=\'$2\'>$1</a>');

        
        return result;
    }
    
    function stringToColour(str) {
        var hash = 0;
        for (var i = 0; i < str.length; i++) {
            hash = str.charCodeAt(i) + ((hash << 5) - hash);
        }
        var colour = '#';
        for (var j = 0; j < 3; j++) {
            var value = (hash >> (j * 8)) & 0xFF;
            colour += ('00' + value.toString(16)).substr(-2);
        }
        return colour;
    }

    function displayDateTime(timestamp)
    {
        return Qt.formatDate(new Date(timestamp), i18n("yyyy-MM-dd")) + "\n" + Qt.formatTime(new Date(timestamp), i18n("hh:mm"));
    }

    property string i_messageText
    property string i_messageID
    property string i_username
    property string i_aliasname
    property string i_avatar
    property var i_timestamp

    
    signal linkActivated(string link)

    
    id: messageMain
    color: "#eeeeee"
    implicitHeight: 4*Kirigami.Units.smallSpacing + Math.max(textLabel.implicitHeight+usernameLabel.implicitHeight, avatarRect.implicitHeight)

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
            
            color: i_avatar !== "" ? "transparent" : stringToColour(i_username)
            
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


                    text: markdownme(RuqolaUtils.markdownToRichText(i_messageText))
                    wrapMode: Label.Wrap
                    
                    onLinkActivated: messageMain.linkActivated(link)
                }
            }
        }
        Kirigami.Label {
            id: timestampText

            text: displayDateTime(i_timestamp)
            opacity: .5

            anchors.top: parent.top
            anchors.right: parent.right
            anchors.leftMargin: Kirigami.Units.smallSpacing

            z: 10
        }
    }
}
