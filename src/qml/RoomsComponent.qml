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
            model: appid.rocketChatAccount.roomFilterProxyModel()
            selectedRoomID: appid.selectedRoomID;
            onRoomSelected: {
                if (roomID == selectedRoomID) {
                    return;
                }
                appid.rocketChatAccount.setUserCurrentMessage(appid.userInputMessageText, selectedRoomID)
                //console.log("Choosing room", roomID);
                appid.selectedRoomID = roomID;
                appid.model = appid.rocketChatAccount.getMessageModelForRoom(roomID)
                appid.selectedRoom = appid.rocketChatAccount.getRoom(roomID)
                appid.userModel = appid.rocketChatAccount.getUsersForRoomModel(roomID)
            }
            
        } //RoomsView
        Kirigami.OverlaySheet {
            id: channelsList
            
            contentItem: ChannelsList {
            }
        }
    }
}
