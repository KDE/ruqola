/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (C) 2017-2019 Laurent Montel <montel@kde.org>
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
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import KDE.Ruqola.RocketChatAccount 1.0
import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.RoomFilterProxyModel 1.0
import KDE.Ruqola.UsersForRoomFilterProxyModel 1.0
import org.kde.kirigami 2.7 as Kirigami
import KDE.Ruqola.StatusModel 1.0

Component {
    id: roomsComponent
    Kirigami.ScrollablePage {
        id: roomsPage
        title: i18n("Rooms")

        actions {
            contextualActions: [
                Kirigami.Action {
                    id: editAction
                    iconName: "list-add"
                    text: i18n("Open room");
                    onTriggered: {
                        searchChannelDialog.initializeAndOpen();
                    }
                },
                Kirigami.Action {
                    iconName: "edit-symbolic"
                    text: i18n("Unread on Top");
                    checkable: true
                    checked: appid.rocketChatAccount.sortUnreadOnTop;
                    onToggled: {
                        appid.rocketChatAccount.setSortUnreadOnTop(checked);
                    }
                },
                Kirigami.Action {
                    iconName: "edit-symbolic"
                    text: i18n("Show Close Icons");
                    checkable: true
                    onToggled: {
                        appid.rocketChatAccount.switchEditingMode(checked);
                    }
                },
                Kirigami.Action {
                    text: i18n("Create New Channel")
                    onTriggered: {
                        createNewChannelDialogLoader.active = true;
                    }
                },
                Kirigami.Action {
                   separator: true
                },
                Kirigami.Action {
                    text: i18n("Server Info")
                    onTriggered: {
                        serverinfodialogLoader.active = true;
                    }
                }

            ]
        }

        // Since we can't have actions at the bottom on mobile, force always toolbar mode
        globalToolBarStyle: Kirigami.ApplicationHeaderStyle.ToolBar
        titleDelegate: LineEditWithClearButton {
            id: searchField
            placeholderText: i18n("Search Room...")
            Layout.fillWidth: true
            onTextChanged: {
                appid.rocketChatAccount.roomFilterProxyModel().setFilterString(text);
            }
            focusSequence: "Ctrl+K"
        }
        background: Rectangle {
            color: Kirigami.Theme.backgroundColor
        }
        footer: QQC2.ToolBar {
            Kirigami.Theme.inherit: false
            Kirigami.Theme.colorSet: Kirigami.Theme.Window
            position: QQC2.ToolBar.Footer
            RowLayout {
                anchors {
                    verticalCenter: parent.verticalCenter
                    left: parent.left
                }
                QQC2.Label {
                    id: comboboxLabel
                    textFormat: Text.PlainText
                    text: i18n("Status:")
                }

                QQC2.ComboBox {
                    id: statusCombobox
                    Layout.alignment: Qt.AlignLeft
                    model: appid.rocketChatAccount.statusModel()
                    //textRole is removed as a workaround for now to draw our own text
                    //textRole: "statusi18n"
                    property variant icon
                    property string text
                    onActivated: {
                        appid.rocketChatAccount.changeDefaultStatus(index)
                    }
                    currentIndex: model.currentStatus

                    delegate: Kirigami.BasicListItem {
                        property bool current: index === statusCombobox.currentIndex
                        separatorVisible: false
                        onCurrentChanged: {
                            if (current) {
                                statusCombobox.text = model.statusi18n
                                statusCombobox.icon = model.icon
                            }
                        }
                        icon: model.icon
                        label: model.statusi18n
                    }
                    //FIXME: QQC2 combobox really, really needs icons support
                    contentItem: RowLayout {
                        Kirigami.Icon {
                            Layout.preferredWidth: Kirigami.Units.iconSizes.smallMedium
                            Layout.fillHeight: true
                            source: statusCombobox.icon
                        }
                        QQC2.Label {
                            text: statusCombobox.text
                            verticalAlignment: Text.AlignVCenter
                            Layout.fillHeight: true
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                    }
                }
            }
        }


        mainItem:
            RoomsView {
            id: roomsList
            editingMode: appid.rocketChatAccount.editingMode
            implicitWidth: Kirigami.Units.gridUnit * 10
            anchors.fill: parent
            model: appid.rocketChatAccount.roomFilterProxyModel()
            selectedRoomID: appid.selectedRoomID;
            onHideRoom: {
                rocketChatAccount.hideRoom(roomID, roomType)
            }
            onLeaveRoom: {
                //TODO move to desktop.qml
                leaveChannelDialogLoader.rid = roomID
                leaveChannelDialogLoader.roomType = roomType
                leaveChannelDialogLoader.active = true
            }

            onRoomSelected: {
                appid.switchToRoom(roomID)
            }
        } //RoomsView
    }
}
