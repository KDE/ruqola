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


// Skeleton from https://github.com/achipa/outqross_blog.git
// Almost everything has been re-adapted

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
//import KDE.Ruqola.Notification 1.0

// import "Log.js" as Log
// import "Data.js" as Data



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

    title: qsTr("Ruqola")
    
    header: Kirigami.ApplicationHeader {}

    pageStack.initialPage: [roomsComponent, mainComponent]
    pageStack.visible: Ruqola.loginStatus == DDPClient.LoggedIn

//     Keys.onShortcutOverride: event.accepted = (event.key === StandardKey.Quit)

//     Shortcut {
//         sequence: StandardKey.Quit
//         context: Qt.ApplicationShortcut
//         onActivated: {
//             console.log("QUITTING (trying)");
//             Qt.quit();
//         }
//     }

    globalDrawer: Kirigami.GlobalDrawer {
        drawerOpen: false
        handleVisible: true
        resetMenuOnTriggered: true
        
        topContent: [
            Text {
                text: qsTr("Hello, %1").arg(Ruqola.userName)
            }
        ]
        
        actions: [
           Kirigami.Action {
               text: "Preferences"
               iconName: "user-available"
               Kirigami.Action {
                       text: "action 1"
               }
               Kirigami.Action {
                       text: "action 2"
               }
               Kirigami.Action {
                       text: "action 3"
               }
           },
           Kirigami.Action {
               text: "Log out"
               iconName: "system-log-out"
               onTriggered: {
                   Ruqola.logOut();
                   appid.globalDrawer.drawerOpen = false;
               }
           },
           Kirigami.Action {
                shortcut: "Ctrl+Q"  //StandardKey.Quit
                text: "Quit"
                iconName: "application-exit"
                onTriggered: {
                    Qt.quit();
                }
           }
        ]
    }
    
    Login {
        id: loginTab
        anchors.fill: parent
        visible: Ruqola.loginStatus != DDPClient.LoggedIn
        serverURL: Ruqola.serverURL
        username: Ruqola.userName
        onAccepted: {
            Ruqola.password = loginTab.password;
            Ruqola.userName = loginTab.username;
            Ruqola.serverURL = loginTab.serverURL;
//            DDPClient.loginType = Password;
            Ruqola.tryLogin();
        }
    }


    BusyIndicator {
        id: busy
//         indeterminate: true
        anchors.centerIn: parent
        visible: Ruqola.loginStatus == DDPClient.LoggingIn
    }

    Component {
        id: roomsComponent
        Kirigami.ScrollablePage {
            background: Rectangle {
                color: Kirigami.Theme.viewBackgroundColor
            }
            RoomsView {
                id: roomsList
                implicitWidth: Kirigami.Units.gridUnit * 10
                anchors.fill: parent

                model: Ruqola.roomModel()
                selectedRoomID: appid.selectedRoomID;
                onRoomSelected: {
                    if (roomID == selectedRoomID) {
                        return;
                    }
                    console.log("Choosing room", roomID);
                    appid.selectedRoomID = roomID;
                    appid.model = Ruqola.getModelForRoom(roomID)
                    appid.selectedRoom = Ruqola.getRoom(roomID)
                }

                onCountChanged: {
    //                 console.log("We have", roomsList.count, "rooms")
                }

            } //RoomsView
        }
    }

    Component {
        id: mainComponent
        
        
        Kirigami.ScrollablePage {
            
//         anchors.right: parent.right
//         anchors.top: parent.top
//         anchors.bottom: parent.bottom
//         anchors.left: roomsComponent.right
//             anchors.fill: parent

            id: mainWidget
            leftPadding: Kirigami.Units.smallSpacing
            rightPadding: Kirigami.Units.smallSpacing
            topPadding: Kirigami.Units.smallSpacing
            bottomPadding: Kirigami.Units.smallSpacing
            title: "#" + appid.selectedRoom.name

            ListView {
                id: activeChat
                model: appid.model
//                     model: Ruqola.getModelForRoom(selectedRoomID)

                anchors.fill:parent
                
                
                onCountChanged: {
//                     console.log("changed")
//                     var newIndex = count - 1 // last index
//                     positionViewAtEnd()
                    positionViewAtIndex(count - 1, ListView.Beginning)
//                     currentIndex = newIndex
                }
//                 Component.onCompleted: positionViewAtEnd()
                Component.onCompleted: positionViewAtIndex(count - 1, ListView.Beginning)


//                 onSelectedRoomIDChanged: { console.log("CHANGED"); activeChat.positionViewAtEnd(); }

//                 model: myModel
                visible : count > 0


                z: -1

    //             ScrollBar.vertical: ScrollBar { }

                delegate: Message {
                            i_messageText: messageText
                            i_username: username
                            i_systemMessage: systemMessage
                            i_systemMessageType: type
                            //width: parent.width
                        }
            }

            footer: Item {
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                id: input
                height: 40

                TextField {
                    id: messageLine
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top
                    anchors.right: emoticonsButton.left
                    placeholderText: if (Ruqola.loginStatus != DDPClient.LoggedIn || (selectedRoomID=="")){
                                        qsTr("Please Select a room")
                                    }
                                    else{
                                        qsTr("Enter message")
                                    }

    //                height: 2.7*font.pixelSize
                    property string type: "text";
                    onAccepted: {
                        if (text != "" && Ruqola.loginStatus == DDPClient.LoggedIn && !(selectedRoomID=="")) {
                            Ruqola.sendMessage(selectedRoomID, text, type);
                            text = "";
                        }
                    }
                }

                Button  {
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top
                    anchors.right: attachmentsButton.left
                    width: 50
                    id : emoticonsButton
                    iconName: "emoticonsButton"
                    iconSource: "qrc:/Emoticon.png"
                    visible: true
                }

                Button {
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top
                    anchors.right: parent.right
                    width: 50
                    id : attachmentsButton
                    iconName: "attachmentsButton"
                    iconSource: "qrc:/icons/attach-button.jpg"
                    visible: true
                    onClicked: Ruqola.attachmentButtonClicked();
                }

            }//Item input

        }// mainWidget Item
    }

//    Image {
//        id: receivedImage
//        source:" "
//        width: 60
//        height: 80
//        fillMode: Image.PreserveAspectFit
////        visible: //only when an image is recieved from server
//        sourceSize.width: 1024
//        sourceSize.height: 1024
//    }


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
//        roomsList.model = Ruqola.roomModel();

//        timer.start();
//        timer.fire();
    }

/*
    Timer {
        id: timer
        interval: 1000
        onTriggered: {
//             console.log("FIRE");
            switch (Ruqola.loginStatus) {
                case Ruqola.NotConnected:
                    statusText = qsTr("Not connected.");
                    break;
                case Ruqola.LoggedIn:
                        statusText = qsTr("Connected to " + Ruqola.serverURL);
                        break;

            }
        }
        repeat: true
    }*/

//    onStatusTextChanged: timer.restart();
}
