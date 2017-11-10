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

import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2 as QQC2

import KDE.Ruqola.RuqolaUtils 1.0
import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.DDPClient 1.0
import KDE.Ruqola.RoomFilterProxyModel 1.0
import org.kde.kirigami 2.1 as Kirigami


Kirigami.ApplicationWindow {
    id: appid
    property int margin: 11

    property string lightGreen: "#6ab141";
    property string darkGreen: "#00613a";

    property string selectedRoomID: "";
    property QtObject selectedRoom

    property QtObject model
    property QtObject userModel
    property string userInputMessageText: "";

    width: Kirigami.Units.gridUnit * 55
    height: Kirigami.Units.gridUnit * 40

    title: i18n("Ruqola")

    pageStack.initialPage: [roomsComponent, mainComponent]
    pageStack.visible: Ruqola.rocketChatAccount().loginStatus === DDPClient.LoggedIn

    globalDrawer: Kirigami.GlobalDrawer {
        drawerOpen: false
        handleVisible: true
        resetMenuOnTriggered: true
        
        topContent: [
            Text {
                text: Ruqola.userName == "" ? "" : i18n("Hello, %1", Ruqola.userName)
            }
        ]
        
        actions: [
            Kirigami.Action {
                text: i18n("About")
                iconName: ":/icons/systray.png"
                onTriggered: {
                    aboutDataDialog.open()
                }
            },
            Kirigami.Action {
                text: i18n("Log out")
                iconName: "system-log-out"
                onTriggered: {
                    Ruqola.rocketChatAccount().logOut();
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

    AboutDialog {
        id: aboutDataDialog
        applicationData: Ruqola.applicationData()
        onOpenurl: {
            RuqolaUtils.openUrl(link);
        }
    }

    CreateNewChannelDialog {
        id: createNewChannelDialog
        onCreateNewChannel: {
            Ruqola.rocketChatAccount().createNewChannel(name, readOnly, privateRoom);
        }
    }

    OpenDirectChannelDialog {
        id: openDirectChannelDialog
        onOpenDirectChannel: {
            Ruqola.rocketChatAccount().openDirectChannel(userName);
        }
    }

    BusyIndicator {
        id: busy
        anchors.centerIn: parent
        visible: Ruqola.rocketChatAccount().loginStatus === DDPClient.LoggingIn
    }

    Component {
        id: roomsComponent
        Kirigami.ScrollablePage {
            id: roomsPage

            actions.main: Kirigami.Action {
                id: editAction
                iconName: "list-add"
                text: i18n("Open room");
                onTriggered: {
                    channelsList.open();
                }
            }
            actions.left: Kirigami.Action {
                iconName: "edit-symbolic"
                text: i18n("Edit room");
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
                header: TextField {
                    id: searchField
                    focus: true
                    Layout.minimumHeight: Layout.maximumHeight
                    Layout.maximumHeight: Kirigami.Units.iconSizes.smallMedium + Kirigami.Units.smallSpacing * 2
                    Layout.fillWidth: true
                    width: parent.width
                    placeholderText: i18n("Search...")
                    onTextChanged: {
                        //TODO filter list view
                    }
                }

                id: roomsList
                implicitWidth: Kirigami.Units.gridUnit * 10
                anchors.fill: parent
                model: Ruqola.rocketChatAccount().roomFilterProxyModel()
                selectedRoomID: appid.selectedRoomID;
                onRoomSelected: {
                    if (roomID == selectedRoomID) {
                        return;
                    }
                    Ruqola.rocketChatAccount().setUserCurrentMessage(appid.userInputMessageText, selectedRoomID)
                    console.log("Choosing room", roomID);
                    appid.selectedRoomID = roomID;
                    appid.model = Ruqola.rocketChatAccount().getMessageModelForRoom(roomID)
                    appid.selectedRoom = Ruqola.rocketChatAccount().getRoom(roomID)
                    appid.userModel = Ruqola.rocketChatAccount().getUsersForRoomModel(roomID)
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

                RowLayout {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 2*Kirigami.Units.smallSpacing
                    visible: appid.selectedRoom

                    ToolButton {
                        iconName: "favorite"
                        checkable: true
                        Binding on checked {
                            value: appid.selectedRoom && appid.selectedRoom.favorite
                        }
                        onCheckedChanged: {
                            Ruqola.rocketChatAccount().changeFavorite(appid.selectedRoomID, checked)
                        }
                    }
                    Kirigami.Heading {
                        text: "#" + (appid.selectedRoom ? appid.selectedRoom.name : "")
                        level: 3
                        font.bold: true
                    }
                    Item {
                        Layout.fillWidth: true
                    }
                    ToolButton {
                        iconName: "settings-configure"
                        onClicked: menu.open();
                        QQC2.Menu {
                            id: menu
                            y: parent.height

                            QQC2.MenuItem {
                                text: i18n("Create New Channel")
                                onTriggered: {
                                    createNewChannelDialog.open()
                                }
                            }
                        }
                    }
                }

                QQC2.Label {
                    visible: appid.selectedRoom && (appid.selectedRoom.topic !== "")
                    text: appid.selectedRoom ? appid.selectedRoom.topic : ""
                    font.italic: true
                    anchors.right: parent.right
                    anchors.left: parent.left
                    anchors.margins: 2*Kirigami.Units.smallSpacing
                    wrapMode: Label.Wrap
                }
                QQC2.Label {
                    visible: appid.selectedRoom && (appid.selectedRoom.announcement !== "")
                    text: appid.selectedRoom ? appid.selectedRoom.announcement : ""
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
            ActiveChat {
                id: activeChat
                model: appid.model
                onOpenDirectChannel: {
                    openDirectChannelDialog.username = userName;
                    openDirectChannelDialog.open()
                    console.log("Open direct channel !!! " + userName)
                }
            }
            footer: UserInput {
                id: userInputMessage
                visible: appid.selectedRoom && (appid.selectedRoom.readOnly === false)

                messageLineText: Ruqola.rocketChatAccount().getUserCurrentMessage(appid.selectedRoomID)
                onTextEditing: {
                    Ruqola.rocketChatAccount().textEditing(appid.selectedRoomID, str)
                    appid.userInputMessageText = str;
                }
                onClearUnreadMessages: {
                    Ruqola.rocketChatAccount().clearUnreadMessages(appid.selectedRoomID)
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
        systrayIcon.activateRequested.connect(toggleShow);
    }
}
