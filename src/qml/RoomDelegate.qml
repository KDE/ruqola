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
import org.kde.kirigami 2.1 as Kirigami

import QtQuick.Layouts 1.1

Kirigami.BasicListItem {
    id: root

    property int d_unread: 0;
    property string d_name: "roomName";
    property bool d_selected: false;
    property string d_roomID : "";
    property string d_type: "";
    property bool d_open: false;
    property bool d_alert: false;
    
    property bool d_editingMode: false;
    
    
    signal roomSelected(string roomID)
    signal leaveRoom(string roomID)
    signal hideRoom(string roomID)
    
    icon: d_type == "c" ? "irc-channel-active" : "user-avaliable"
    
    visible: !d_name.empty && d_open

    font.bold: (d_unread > 0) || d_alert
    label: d_name
    checked: d_selected
    onClicked: {
        root.roomSelected(d_roomID);
    }
    
    Row {

        
        id: editingActions
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        width: 50
        spacing: Kirigami.Units.smallSpacing
        Kirigami.Label {
            id: unreadMessage

            visible: !editingMode && d_unread > 0
            height: parent.height
            width: height
            font.bold: true
            text: i18n("(%1)", d_unread)
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
        }
        Kirigami.Icon {
            id: hideAction

            visible: editingMode // && root.hover ?
            source: "hide_table_row"
            height: parent.height
            width: height
            opacity: d_selected ? 1 : .5
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.active = true;
                onExited: parent.active = false;
                onClicked: {
                    root.hideRoom(d_roomID)
                }
            }
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
        }
        
        Kirigami.Icon {
            id: quitAction

            //We can leave only channel not private chat
            visible: editingMode && d_type == "c"
            source: "dialog-close"
            height: parent.height
            width: height
            opacity: d_selected ? 1 : .5
            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: parent.active = true;
                onExited: parent.active = false;
                onClicked: {
                    root.leaveRoom(d_roomID)
                }
            }
            Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
        }
    }
} // Item closed
