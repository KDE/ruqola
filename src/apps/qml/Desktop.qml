/*

 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>
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
import org.kde.kirigami 2.4 as Kirigami
import KDE.Ruqola.DebugCategory 1.0
import KDE.Ruqola.Channel 1.0
import KDE.Ruqola.AccountManager 1.0

Kirigami.ApplicationWindow {
    id: appid
    readonly property int margin: 11

    property string selectedRoomID: "";
    property QtObject selectedRoom

    property QtObject messageModel
    property QtObject userModel
    property QtObject filesModel
    property QtObject accountManager: Ruqola.accountManager()
    property QtObject accountManagerModel: accountManager.rocketChatAccountModel()
    property QtObject rocketChatAccount: accountManager.firstAccount()
    property QtObject inputCompleterModel: rocketChatAccount.inputCompleterModel()
    property QtObject searchMessageModel: rocketChatAccount.searchMessageFilterProxyModel()
    property string userInputMessageText: "";

    width: Kirigami.Units.gridUnit * 55
    height: Kirigami.Units.gridUnit * 40

    title: i18n("Ruqola")

    pageStack.initialPage: [roomsComponent, mainComponent]
    pageStack.visible: rocketChatAccount.loginStatus === DDPClient.LoggedIn

    globalDrawer: Kirigami.GlobalDrawer {
        drawerOpen: false
        handleVisible: true
        resetMenuOnTriggered: true
        
        topContent: [
            QQC2.Label {
                text: rocketChatAccount.userName === "" ? "" : i18n("Hello, %1", rocketChatAccount.userName)
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
                    rocketChatAccount.logOut();
                    appid.globalDrawer.drawerOpen = false;
                }
            },
            Kirigami.Action {
                text: i18n("Report a Bug")
                iconName: "tools-report-bug"
                onTriggered: {
                    Qt.openUrlExternally("https://bugs.kde.org/report.cgi");
                }
            },
            Kirigami.Action {
                text: i18n("Configure Account")
                iconName: "settings-configure"
                onTriggered: {
                    configureServerList.visible = true
                    pageStack.visible = false
                }
            },
            Kirigami.Action {
                text: i18n("Handbook")
                iconName: "system-help"
                onTriggered: {
                    rocketChatAccount.openDocumentation();
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
    
    ConfigureServerList {
        id: configureServerList
        accountModel: accountManagerModel
        visible: false
        onCloseConfigureServer: {
            configureServerList.visible = false
            pageStack.visible = true
        }
    }

    LoginPage {
        id: loginTab
        rcAccount: rocketChatAccount
    }

    AboutDialog {
        id: aboutDataDialog
        applicationData: Ruqola.applicationData()
        onOpenurl: {
            RuqolaUtils.openUrl(link);
        }
    }

    PrivateChannelInfoDialog {
        id: privateChannelInfoDialog
        onBlockUser: {
            rocketChatAccount.blockUser(rid, block)
        }
    }

    NotificationOptionsDialog {
        id: notificationsDialog
        rid: (appid && appid.selectedRoomID) ? appid.selectedRoomID : ""
        onModifyNotificationsSetting: {
            rocketChatAccount.changeNotificationsSettings(roomId, type, newVal)
        }
    }

    ChannelInfoDialog {
        id: channelInfoDialog
        channelName: (appid && appid.selectedRoomID) ? appid.selectedRoomID : ""
        onDeleteRoom: {
            rocketChatAccount.eraseRoom(roomId)
        }
        onModifyChannelSetting: {
            rocketChatAccount.changeChannelSettings(roomId, type, newVal, channelType)
        }
    }

    LeaveChannelDialog {
        id: leaveChannelDialog
        onLeaveChannel:  {
            rocketChatAccount.leaveRoom(roomId, channelType)
        }
    }

    AddUserDialog {
        id: addUserDialog
        completerModel: rocketChatAccount.userCompleterFilterModelProxy()
        onSearchUserName: {
            rocketChatAccount.userAutocomplete(pattern, "");
        }
        onAddUser: {
            rocketChatAccount.addUserToRoom(name, rid, channelType)
        }
    }

    ShowSearchMessageDialog {
        id: searchMessageDialog
        searchMessageModel: appid.searchMessageModel
        onSearchMessage: {
            rocketChatAccount.messageSearch(pattern, rid)
        }
        onClosed: {
            rocketChatAccount.clearSearchModel()
        }
    }

    SearchChannelDialog {
        id: searchChannelDialog
        searchChannelModel: rocketChatAccount.searchChannelFilterProxyModel()
        onSearchChannel: {
            rocketChatAccount.channelAndPrivateAutocomplete(pattern);
        }
        onOpenChannel: {
            if (channeltype === Channel.Room) {
                rocketChatAccount.openChannel(channelid)
            } else if (channeltype === Channel.PrivateChannel) {
                if (rocketChatAccount.userName !== channelid) {
                    rocketChatAccount.openDirectChannel(channelid)
                }
            } else {
                console.log(RuqolaDebugCategorySingleton.category, "Unknown open channel type : "  + channeltype + " channelid : "  + channelid + " channelname : "  + channelname)
            }
        }
    }

    TakeVideoMessageDialog {
        id: takeVideoMessage
        rcAccount: rocketChatAccount
    }

    BusyIndicator {
        id: busy
        anchors.centerIn: parent
        visible: rocketChatAccount.loginStatus === DDPClient.LoggingIn
    }

    RoomsComponent {
        id: roomsComponent
    }

    MainComponent {
        id: mainComponent
    }

    ChannelPasswordDialog {
        id: channelPasswordDialog
        onJoinRoom: {
            rocketChatAccount.joinRoom(roomId, password)
        }
    }

    Connections {
        target: rocketChatAccount
        onMissingChannelPassword: {
            channelPasswordDialog.roomId = roomId
            channelPasswordDialog.visible = true
        }
    }


    onClosing: {
        Qt.quit();
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
