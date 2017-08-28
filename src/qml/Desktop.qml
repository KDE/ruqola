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

import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0 as QQC2
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0
import QtGraphicalEffects 1.0

import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.DDPClient 1.0
import org.kde.kirigami 2.1 as Kirigami


Kirigami.ApplicationWindow {
    id: appid
    property int margin: 11
    property string statusText

    property string lightGreen: "#6ab141";
    property string darkGreen: "#00613a";

    property string selectedRoomID: "";
    property QtObject selectedRoom

    property QtObject model

    width: Kirigami.Units.gridUnit * 55
    height: Kirigami.Units.gridUnit * 40

    title: i18n("Ruqola")
    
    header: Kirigami.ApplicationHeader {}

    pageStack.initialPage: [roomsComponent, mainComponent]
    pageStack.visible: Ruqola.loginStatus == DDPClient.LoggedIn

    globalDrawer: Kirigami.GlobalDrawer {
        drawerOpen: false
        handleVisible: true
        resetMenuOnTriggered: true
        
        topContent: [
            Text {
                text: i18n("Hello, %1", Ruqola.userName)
            }
        ]
        
        actions: [
            Kirigami.Action {
                text: i18n("Preferences")
                iconName: "user-available"
                Kirigami.Action {
                    text: i18n("Change theme")
                    iconName: "preferences-desktop-theme"
                }
            },
            Kirigami.Action {
                text: i18n("Log out")
                iconName: "system-log-out"
                onTriggered: {
                    Ruqola.logOut();
                    appid.globalDrawer.drawerOpen = false;
                }
            },
            Kirigami.Action {
                shortcut: StandardKey.Quit
                text: i18n("Quit")
                iconName: "application-exit"
                onTriggered: {
                    Qt.quit();
                }
            }
        ]
    }
    
    LoginPage {
        id: loginTab
    }


    BusyIndicator {
        id: busy
        anchors.centerIn: parent
        visible: Ruqola.loginStatus == DDPClient.LoggingIn
    }

    Component {
        id: roomsComponent
        Kirigami.ScrollablePage {
            id: roomsPage

            actions.main: Kirigami.Action {
                id: editAction
                iconName: "list-add"
                onTriggered: {
                    channelsList.open();
                }
            }
            actions.left: Kirigami.Action {
                iconName: "edit-symbolic"
                checkable: true
                onToggled: {
                    roomsList.editingMode = checked
                    // do stuff
                }
            }
            
            background: Rectangle {
                color: Kirigami.Theme.viewBackgroundColor
            }

            mainItem:
                RoomsView {

                id: roomsList
                implicitWidth: Kirigami.Units.gridUnit * 10
                anchors.fill: parent
                //                     width: parent.width
                //                     height: parent.height
                model: Ruqola.roomModel()
                selectedRoomID: appid.selectedRoomID;
                onRoomSelected: {
                    if (roomID == selectedRoomID) {
                        return;
                    }
                    console.log("Choosing room", roomID);
                    appid.selectedRoomID = roomID;
                    appid.model = Ruqola.getMessageModelForRoom(roomID)
                    appid.selectedRoom = Ruqola.getRoom(roomID)
                }
                
            } //RoomsView
            Kirigami.OverlaySheet {
                id: channelsList

                contentItem: ChannelsList {
                }
            }
        }
    }

    Component {
        id: mainComponent
        
        
        Kirigami.ScrollablePage {

            id: mainWidget
            leftPadding: Kirigami.Units.smallSpacing
            rightPadding: Kirigami.Units.smallSpacing
            topPadding: Kirigami.Units.smallSpacing
            bottomPadding: Kirigami.Units.smallSpacing
            header: Column {

                Kirigami.Heading {
                    text: "#" +appid.selectedRoom.name
                    level: 3
                    font.bold: true
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.leftMargin: 2*Kirigami.Units.smallSpacing
                }
                Kirigami.Label {
                    visible: appid.selectedRoom.topic !== ""
                    text: appid.selectedRoom.topic
                    font.italic: true
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.margins: 2*Kirigami.Units.smallSpacing
                    wrapMode: Label.Wrap
                }
                Kirigami.Label {
                    visible: appid.selectedRoom.announcement !== ""
                    text: appid.selectedRoom.announcement
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.margins: 2*Kirigami.Units.smallSpacing
                    wrapMode: Label.Wrap
                }
                Rectangle {
                    color: Kirigami.Theme.textColor
                    height:1
                    anchors.right: parent.right
                    anchors.left: parent.left
                    opacity: .5
                    visible: appid.selectedRoom
                }
            }

            ListView {
                id: activeChat
                model: appid.model

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
                    i_systemMessage: systemMessage
                    i_systemMessageType: type
                    i_timestamp: timestamp
                    i_messageID: messageID
                }
            }
            footer: UserInput {
                id: footerItem
                onTextEditing: {
                    Ruqola.textEditing(appid.selectedRoomID, str)
                }
                onClearUnreadMessages: {
                    Ruqola.clearUnreadMessages(appid.selectedRoomID)
                }
            }
        }// mainWidget Item
    }

    onClosing: {
        console.log("Minimizing to systray...");
        hide();
    }


    function toggleShow() {
        if (visible) {
            hide();
        } else {
            show();
            raise();
            requestActivate();
        }
    }

    Component.onCompleted: {
        systrayIcon.activated.connect(toggleShow);
        systrayIcon.messageClicked.connect(toggleShow);
    }
}
