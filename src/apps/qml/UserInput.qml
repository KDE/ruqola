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
import QtQuick.Layouts 1.12

import KDE.Ruqola.RocketChatAccount 1.0
import org.kde.kirigami 2.7 as Kirigami
import "common"
RowLayout {
    id: footerItem
    property QtObject rcAccount
    property alias messageLineText: messageLine.messageLineText
    property alias messageId: messageLine.messageId
    property string threadmessageId
    property string selectedRoomId
    property string selectedThreadMessage
    property QtObject inputCompleterModel

    signal textEditing(string str)
    signal uploadFile()
    signal clearUnreadMessages()
    
    height: 2*Kirigami.Units.largeSpacing
    
    function setOriginalMessage(messageStr)
    {
        messageLine.setOriginalMessage(messageStr)
    }

    Kirigami.Icon {
        id: attachment

        enabled: selectedRoomId !== ""
        source: "document-send-symbolic"
        width: height
        height: messageLine.height
        MouseArea {
            anchors.fill: parent
            onClicked: {
                footerItem.uploadFile()
            }
        }
    }
    MessageLine {
        id: messageLine
        inputCompleterModel: footerItem.inputCompleterModel
        selectedRoomId: footerItem.selectedRoomId
        messageId: footerItem.messageId
        threadmessageId: footerItem.threadmessageId
        selectedThreadMessage: footerItem.selectedThreadMessage
    }
    
    EmoticonMenu {
        id: emoticonMenu
        emojiPopupModel: appid.emojiModel
        width: Kirigami.Units.gridUnit * 20
        height: Kirigami.Units.gridUnit * 15
        x: -width + parent.width
        y: -height - 10
        onInsertEmoticon: {
            messageLine.insertEmoticon(emoti)
        }
    }
    Kirigami.Icon {
        source: "face-smile"
        width: height
        height: messageLine.height
        MouseArea {
            anchors.fill: parent
            onClicked: {
                emoticonMenu.visible ? emoticonMenu.close() : emoticonMenu.open()
            }
        }
    }

    Kirigami.Icon {
        source: messageLine.savePreviousMessage == "" ?  "mail-sent" : "edit-symbolic"
        width: height
        height: messageLine.height
        MouseArea {
            anchors.fill: parent
            onClicked: {
                messageLine.sendMessage()
            }
        }
    }
}
