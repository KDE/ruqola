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
import "../js/marked.js" as MarkDown;

import QtQuick.Controls 2.2
import org.kde.kirigami 2.1 as Kirigami

import QtQuick.Layouts 1.1
import KDE.Ruqola.Ruqola 1.0


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
    {regex: /(\*|__)(.*?)\1/g, replacement: '<b>$2</b>'},                  // bold
    {regex: /(_)(.*?)\1/g, replacement: '<i>$2</i>'},                             // emphasis
    {regex: /\~\~(.*?)\~\~/g, replacement: '<del>$1</del>'},                           // del
    {regex: /\:\"(.*?)\"\:/g, replacement: '<q>$1</q>'},                               // quote
    {regex: /`(.*?)`/g, replacement: '<code>$1</code>'},                               // inline code
    {regex: /\n\*(.*)/g, replacement: ulList},                                         // ul lists
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

        var regex2 = new RegExp(/#(\w+)/g);
        result = result.replace(regex2, '<a href=\'ruqola:/room/$1\'>#$1</a>');

        var regex3 = new RegExp(/@(\w+)/g);
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
        for (var i = 0; i < 3; i++) {
            var value = (hash >> (i * 8)) & 0xFF;
            colour += ('00' + value.toString(16)).substr(-2);
        }
        return colour;
    }

    property string i_messageText
    property string i_messageID
    property string i_username
    property bool i_systemMessage
    property string i_systemMessageType
    property string i_avatar
    property var i_timestamp

    
    signal linkActivated(string link)
    
    
    id: messageMain
    color: "#eeeeee"
    implicitHeight: 4*Kirigami.Units.smallSpacing + Math.max(textLabel.implicitHeight+usernameLabel.implicitHeight, avatarRect.implicitHeight)
    
//     implicitWidth: 150
    
    anchors.bottomMargin: 200
    
    Menu {
        id: menu

        MenuItem {
//             enabled: i_username == Ruqola.userName
            contentItem: Kirigami.Label {
                text: "Edit"
                enabled: i_username == Ruqola.userName
            }
            onTriggered: {
                console.log("Edit", i_messageID, i_messageText);
                console.log("User", Ruqola.userName, i_username);
            }
        }
        MenuItem {
            contentItem: Kirigami.Label {
                text: "Reply"
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
//         implicitHeight: textLabel.contentHeight

        spacing: Kirigami.Units.smallSpacing
//         spacing: 12
        
        Rectangle {
            Layout.fillHeight: false

            id: avatarRect
            implicitWidth: 24
            implicitHeight: 24
            
            radius: 3
            
            anchors.rightMargin: 2*Kirigami.Units.smallSpacing
            
            color: stringToColour(i_username)
            
            anchors.top: parent.top
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
                    level: 5
                    id: usernameLabel
                    font.bold: true
                    text: i_username
                    
                    anchors.right: parent.right
                    anchors.left: parent.left
                    height: avatarRect.height
                }
                Kirigami.Label {
                    id: textLabel
                    anchors.right: parent.right
                    anchors.left: parent.left
                    
                    anchors.leftMargin: Kirigami.Units.smallSpacing
                    anchors.rightMargin: Kirigami.Units.smallSpacing
                    
                    text: markdownme(i_messageText)
                    wrapMode: Label.Wrap
                    
                    renderType: Text.NativeRendering
                    
                    onLinkActivated: messageMain.linkActivated(link)



                }
            }
            
           
        }
        Kirigami.Label {
            id: timestampText
            text: Qt.formatTime(new Date(i_timestamp), "hh:mm")
            opacity: .5
            
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.leftMargin: Kirigami.Units.smallSpacing
            
            z: 10
        }
    }
    
//     Component.onCompleted: console.log(i_timestamp)

}
