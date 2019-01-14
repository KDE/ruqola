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
import org.kde.kirigami 2.4 as Kirigami
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.5 as QQC2
import KDE.Ruqola.RocketChatAccount 1.0

ListView {
    id: roomsList
    property string selectedRoomID;
    property bool editingMode: false;

    signal hideRoom(string roomID)
    signal leaveRoom(string roomID)
    signal roomSelected(string roomID)

    section {
        property: "sectionname"
        delegate: Kirigami.AbstractListItem {
            enabled: false
            RowLayout {
                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: Kirigami.Units.smallSpacing
                }
                QQC2.Label {
                    id: sectionLabel
                    text: section
                    Layout.minimumHeight: Math.max(implicitHeight, Kirigami.Units.iconSizes.smallMedium)
                    elide: Text.ElideRight
                    Layout.alignment: Qt.AlignHCenter
                    Component.onCompleted: font.bold = true
                }
            }
        }
    }

    delegate: RoomDelegate {
        d_name: name
        d_unread: unread
        d_userMentions: userMentions
        d_roomID: room_id
        d_type: type
        d_open: open
        d_alert: alert
        d_selected: selectedRoomID == room_id
        d_editingMode: editingMode
        d_icon: channelicon
        
        onRoomSelected : {
            roomsList.roomSelected(roomID)
            applicationWindow().pageStack.currentIndex = 1;
        }
        onHideRoom: {
            roomsList.hideRoom(roomID)
        }
        onLeaveRoom: {
            roomsList.leaveRoom(roomID)
        }
    }
}
