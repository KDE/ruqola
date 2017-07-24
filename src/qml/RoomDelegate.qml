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
import org.kde.kirigami 2.1 as Kirigami

Kirigami.BasicListItem {
    property int d_unread: 0;
    property string d_name: "roomName";
    property bool d_selected: false;
    property string d_roomID : "";
    
    id: root
    
    signal roomSelected(string roomID)
    
    icon: "irc-channel-active"
    
    visible: !d_name.empty

    label: d_name
    checked: d_selected
    onClicked: {
        root.roomSelected(d_roomID);
    }
    Text {
        visible: false
        anchors.fill: parent
        id: nameLabel
        font.bold: d_unread > 0
        font.pointSize: 11
        text: d_name
        verticalAlignment: Text.AlignVCenter
        anchors.leftMargin: 15
        
        color: d_selected ? "white" : "white" 
        
        Rectangle {
            
            property bool hovered: false;
            
            states: [
                State {
                    name: "inactive"
                    PropertyChanges { target: highlight; opacity: 0 }
                },
                State {
                    name: "selected"
                    PropertyChanges { target: highlight; opacity: .7 }
                },
                State {
                    name: "hovered"
                    PropertyChanges { target: highlight; opacity: .25 }
                }
            ]
            
            id: highlight
//             opacity: .5
//             visible: d_selected
            anchors.fill: parent
            anchors.leftMargin: -10
            anchors.rightMargin: 5
            color: "#6ab141"
            z: -1
            radius: 4
            
            state: {
                if (d_selected) {
                    return "selected";
                } else {
                    if (highlight.hovered) {
                        return "hovered";
                    } else {
                        return "inactive";
                    }
                    
                }
            }
            
            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                onEntered: {
//                     if (highlight.state != "selected") {
                        highlight.hovered = true
//                         highlight.state = "hovered"
//                     }
                }
                onExited: {
//                     if (highlight.state != "selected") {
                        highlight.hovered = false
//                         highlight.state = "inactive"
//                     }
                }
                onClicked: {
                    if (highlight.state == "hovered") {
                        root.roomSelected(d_roomID);
//                         highlight.state = "selected"
                        d_selected: true;
                    }
                    else {
                        root.roomSelected("");
//                         highlight.state = "hovered"
                        d_selected: false;
                        
                    }
                }
                
            } //MouseArea closed
            
        } //Rectangle closed
    } //Text closed

    Rectangle {
        anchors.rightMargin: 15
       // visible: d_unread > 0
        anchors.right: root.right
        anchors.verticalCenter: nameLabel.verticalCenter
//      width: nameLabel.font.pixelSize 
        width: 0.7*nameLabel.height
        height: width
        color: "#6ab141"
visible: false
        radius: width*0.5
        Text {
            anchors.fill: parent
            color: "white"
            text: d_unread
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
    }
//             id: room_chooser
//             
//             MouseArea {
//                 anchors.fill: parent
//                 onClicked: {
//                     console.log("Choosing room", room_chooser.internal_id);
//                     selectedRoomID = room_chooser.internal_id;
//                     
// //                             myModel.currentRoom = selectedRoomID;
//                     
//                     // This line crashes the program when we refine RoomsModel:
// //                     if (Ruqola.loginStatus != DDPClient.LoggingIn)
// //                     activeChat.model = Ruqola.getModelForRoom(selectedRoomID);
//                     
// //                             console.log(activeChat.count);
//                 }
//             }
} // Item closed
