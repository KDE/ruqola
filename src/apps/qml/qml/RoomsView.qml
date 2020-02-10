/*
 
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>
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
import org.kde.kirigami 2.10 as Kirigami
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import Ruqola 1.0

ListView {
    id: roomsList
    property string selectedRoomID;
    property bool editingMode: false;

    function syncSelectionFromModel() {
        var modelIndices = model.match(model.index(0, 0), RoomModel.RoomID, selectedRoomID);
        if (modelIndices.length === 0) {
            console.warning("Failed to sync room selection from model");
            return;
        }

        currentIndex = modelIndices[0].row;
    }

    Connections {
        target: model
        onLayoutChanged: syncSelectionFromModel() // happens when the model gets re-sorted
    }

    signal leaveRoom(string roomID, string roomType)
    signal hideRoom(string roomID, string roomType)
    signal roomSelected(string roomID)

    section {
        property: "sectionname"
        delegate: Kirigami.ListSectionHeader {
            text: section
        }
    }

    // If unread are on top, we do not want the view to follow the highlight
    // Otherwise the list view is scrolled when channels are marked as read => feels cumbersome
    highlightFollowsCurrentItem: !appid.rocketChatAccount.sortUnreadOnTop

    delegate: RoomDelegate {
        d_name: fname
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
            roomsList.hideRoom(roomID, roomType)
        }
        onLeaveRoom: {
            roomsList.leaveRoom(roomID, roomType)
        }
    }
}
