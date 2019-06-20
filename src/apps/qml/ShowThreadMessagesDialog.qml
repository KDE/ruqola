/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Window 2.2
import QtQuick 2.9
import org.kde.kirigami 2.7 as Kirigami

QQC2.Dialog {
    id: showThreadMessagesDialog

    title: i18n("Thread Messages") //Add room name too.

    property QtObject threadMessagesModel

    property string threadMessageId
    width: parent.width * 9 / 10
    height: parent.height * 9 / 10
    anchors.centerIn: parent


    modal: true
    focus: true
    standardButtons: QQC2.Dialog.Close
    contentItem: ColumnLayout {
        ActiveChat {
            Layout.fillWidth: true
            Layout.fillHeight: true
            id: threadMessageListView
            model: threadMessagesModel
            rcAccount: appid.rocketChatAccount
            roomId: appid.selectedRoomID
            clip: true

            QQC2.ScrollBar.vertical: QQC2.ScrollBar { }
        }
        //TODO improve it.
        UserInput {

        }
    }
}
