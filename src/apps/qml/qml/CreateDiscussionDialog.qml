/*
   Copyright (c) 2019-2020 Laurent Montel <montel@kde.org>

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

import QtQuick 2.9
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Window 2.2

QQC2.Dialog {
    id: createDiscussionDialog

    title: i18n("Create Discussion")

    property string roomId
    property string messageId
    property string roomName
    property string originalMessage


    signal createNewDiscussion(string parentRoomName, string discussionTitle, string replyMessage, string msgId)

    standardButtons: QQC2.Dialog.Ok | QQC2.Dialog.Cancel

    width: parent.width * 9 / 10
    height: parent.height * 9 / 10
    anchors.centerIn: parent

    modal: true
    focus: true

    function clearAndOpen()
    {
        //discussionName.text = ""
        //TODO search how implement it without call it all the time.
        standardButton(QQC2.Dialog.Ok).text = i18n("Create");
        answer.text = ""
        open();
    }

    contentItem: ColumnLayout {
        QQC2.Label {
            text: i18n("Parent Channel or Group:");
        }
        QQC2.TextField {
            id: parentRoom
            text: roomName
            Layout.fillWidth: true
            selectByMouse: true
            readOnly: true
        }

        QQC2.Label {
            text: i18n("Discussion Name:");
        }
        QQC2.TextField {
            id: discussionName
            selectByMouse: true
            Layout.fillWidth: true
            text: originalMessage
        }

//        //TODO implement it.
//        QQC2.Label {
//            text: i18n("Invite Users:");
//        }
//        QQC2.TextField {
//            id: inviteUser
//            selectByMouse: true
//            Layout.fillWidth: true
//        }

        QQC2.Label {
            text: i18n("Your answer:");
        }
        QQC2.TextField {
            id: answer
            Layout.fillWidth: true
            Layout.fillHeight: true
            selectByMouse: true
            verticalAlignment: TextInput.AlignTop
            wrapMode: TextInput.Wrap
        }

        //Add message + users
    }

    onAccepted: {
        createDiscussionDialog.createNewDiscussion(roomId, discussionName.text, answer.text, messageId)
    }
}

