
// Skeleton from https://github.com/achipa/outqross_blog.git
// Almost everything has been re-adapted

import QtQuick 2.7
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0

import KDE.Ruqola.UserData 1.0
import KDE.Ruqola.DDPClient 1.0

// import "Log.js" as Log
// import "Data.js" as Data

ApplicationWindow {
    property int margin: 11
    property string statusText

    property list<JSONListModel> todos
    property JSONListModel lists: JSONListModel { }

    property JSONListModel activeRoom: JSONListModel {}
    property JSONListModel userRooms: JSONListModel {}
    
    property string selectedRoomID;
    property bool ready;
    
    id: appid
    title: qsTr("Ruqola")
    width: 640
    height: 480
    visible: true
    
    menuBar: MenuBar {
        Menu {
            title: qsTr("&Main")
            MenuItem {
                text: qsTr("&Log out")
                onTriggered: {
                    UserData.logOut();
//                     loginTab.visible = true;
//                     mainWidget.visible = false;
//                     messageDialog.show(qsTr("Reconnect action triggered"));
                }
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
                shortcut: StandardKey.Quit;
            }
        }
    }

//     Component.onCompleted : {UserData.tryLogin()}//.log(UserData.loggedIn);}
    Login {
        id: loginTab
        visible: (UserData.loginStatus == DDPClient.LoginFailed || UserData.loginStatus == DDPClient.LoggedOut)
        
        anchors.fill:parent
        z: 10
        serverURL: UserData.serverURL
        username: UserData.userName
        onAccepted: {
            UserData.password = loginTab.password;
            UserData.userName = loginTab.username;
            UserData.serverURL = loginTab.serverURL;
//             console.log("")
            UserData.tryLogin();
        }
        
    }
    
//     statusBar: StatusView {
//         RowLayout {
//             Label { text: statusText }
//         }
//     }

    BusyIndicator {
        id: busy
        anchors.centerIn: parent
        visible: UserData.loginStatus == DDPClient.LoggingIn
    }

    Item {
        id: mainWidget
        anchors.fill: parent
        visible: !loginTab.visible
//         visible:true
//         Component.onCompleted :{
//             console.log("debug");
//             console.log(UserData.loginStatus);
//             console.log( DDPClient.LoginFailed);
//             console.log(UserData.loginStatus != DDPClient.LoginFailed);
//         }
        
        ListView {
            id: roomsList
            model: UserData.roomModel()
            width: 100
            visible: true
            
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 10;
            
            delegate:
                Text {
                    property variant internal_id: id
                    text: name
                    font.bold: (selectedRoomID == id)
                    id: room_chooser
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            console.log("Choosing room", room_chooser.internal_id);
                            selectedRoomID = room_chooser.internal_id;
//                             myModel.currentRoom = selectedRoomID;
                            activeChat.model = UserData.getModelForRoom(selectedRoomID);
                            console.log(activeChat.count);
                        }
                    }
                }
        }
        ScrollView {
            anchors.right: parent.right
            anchors.left: roomsList.right
            anchors.top: parent.top
            anchors.bottom: messageLine.top
            verticalScrollBarPolicy: Qt.ScrollBarAlwaysOn
            

            ListView {
                id: activeChat
                
//                 property string activeRoom: selectedRoomID
//                 visibleArea.yPosition: 1.0-heightRatio
                onCountChanged: {
                    console.log("changed")
//                     var newIndex = count - 1 // last index
//                     positionViewAtEnd()
                    positionViewAtIndex(count - 1, ListView.Beginning)

//                     currentIndex = newIndex
                }
//                 Component.onCompleted: positionViewAtEnd()
                Component.onCompleted: positionViewAtIndex(count - 1, ListView.Beginning)


//                 onSelectedRoomIDChanged: { console.log("CHANGED"); activeChat.positionViewAtEnd(); }

//                 model: myModel
                anchors.fill:parent
                visible : count > 0
                
                
                z: -1
                
    //             ScrollBar.vertical: ScrollBar { }
                
                delegate: Message {
                            i_messageText: messageText
                            i_username: username
                            i_systemMessage: systemMessage
                            i_systemMessageType: type
                            width: parent.width
                        }
            }
        }
        TextField {
            id: messageLine
            anchors.right: parent.right
            anchors.left: roomsList.right
            anchors.bottom: parent.bottom
            placeholderText: qsTr("Enter message")
            onAccepted: {
                if (text != "") {
                    UserData.sendMessage(selectedRoomID, text);
                    text = "";
                }
            }
        }
    }
    
    
    onClosing: {
        console.log("Minimizing to systray...");
        hide();
    }
    
    function toggleShow(reason) {
//         console.log ("Showing");
        
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
//         systrayIcon.showMessage("Connected", "We are CONNECTED!");
    }

    
    Timer {
        id: timer
        interval: 3000
        onTriggered: statusText = "";
        repeat: true
    }

    onStatusTextChanged: timer.restart();
}
