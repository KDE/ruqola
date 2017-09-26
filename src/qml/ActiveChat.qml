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

import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0 as Controls
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0
import QtGraphicalEffects 1.0

import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.DDPClient 1.0
import KDE.Ruqola.RoomFilterProxyModel 1.0
import org.kde.kirigami 2.1 as Kirigami

/*
    anchors.fill: parent
    TextField {
        id: edit

        placeholderText: i18n("Search...")

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.margins: 10

        onTextChanged: appid.model.setFilterFixedString(text)
    }
    */
ListView {
    id: activeChat

    property QtObject roomModel

    model: roomModel

    onCountChanged: {
        positionViewAtIndex(count - 1, ListView.Beginning)
    }

    Component.onCompleted: positionViewAtIndex(count - 1, ListView.End)
    visible : count > 0

    delegate: FancyMessageDelegate {
        width: parent.width
        i_messageText: messageText
        i_username: username
        i_aliasname: alias
        i_systemMessageType: type
        i_timestamp: timestamp
        i_messageID: messageID
        i_messageType: messagetype
        i_avatar: avatar
        i_urls: urls
        i_attachments: attachments
        i_date: date
    }
}
