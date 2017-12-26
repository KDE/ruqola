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
import KDE.Ruqola.DebugCategory 1.0

Kirigami.ApplicationWindow {
    id: appid
    property int margin: 11

    property string selectedRoomID: "";
    property QtObject selectedRoom

    property QtObject model
    property QtObject userModel
    property QtObject rocketChatAccount: Ruqola.rocketChatAccount()
    property string userInputMessageText: "";

    width: Kirigami.Units.gridUnit * 55
    height: Kirigami.Units.gridUnit * 40

    title: i18n("Ruqola")

    pageStack.initialPage: [roomsComponent, mainComponent]
    pageStack.visible: appid.rocketChatAccount.loginStatus === DDPClient.LoggedIn

    globalDrawer: Kirigami.GlobalDrawer {
        drawerOpen: false
        handleVisible: true
        resetMenuOnTriggered: true
        
        topContent: [
            Text {
                text: appid.rocketChatAccount.userName === "" ? "" : i18n("Hello, %1", appid.rocketChatAccount.userName)
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
                    appid.rocketChatAccount.logOut();
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

    ChannelInfoDialog {
        id: channelInfoDialog
        channelName: (appid && appid.selectedRoomID) ? appid.selectedRoomID : ""
        onDeleteRoom: {
            appid.rocketChatAccount.eraseRoom(roomId)
        }
        onModifyChannelSetting: {
            appid.rocketChatAccount.changeChannelSettings(roomId, type, newVal)
        }
    }

    CreateNewChannelDialog {
        id: createNewChannelDialog
        onCreateNewChannel: {
            appid.rocketChatAccount.createNewChannel(name, readOnly, privateRoom, usernames);
        }
    }

    AddUserDialog {
        id: addUserDialog
        onSearchUserName: {
            console.log("Search username" + pattern);
            appid.rocketChatAccount.userAutocomplete(pattern, "");
        }
    }

    OpenDirectChannelDialog {
        id: openDirectChannelDialog
        onOpenDirectChannel: {
            appid.rocketChatAccount.openDirectChannel(userName);
        }
    }

    DeleteMessageDialog {
        id: deleteMessageDialog
        onDeleteMessage: {
            appid.rocketChatAccount.deleteMessage(messageId)
        }
    }

    TakeVideoMessageDialog {
        id: takeVideoMessage
        rocketChatAccount: appid.rocketChatAccount
    }

    BusyIndicator {
        id: busy
        anchors.centerIn: parent
        visible: appid.rocketChatAccount.loginStatus === DDPClient.LoggingIn
    }

    RoomsComponent {
        id: roomsComponent
    }

    MainComponent {
        id: mainComponent
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
