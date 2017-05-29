
// Skeleton from https://github.com/achipa/outqross_blog.git
// Almost everything has been re-adapted

import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0
import QtGraphicalEffects 1.0

import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.DDPClient 1.0
import KDE.Ruqola.Notification 1.0


// import "Log.js" as Log
// import "Data.js" as Data

ApplicationWindow {
    property int margin: 11
    property string statusText
    
    property string lightGreen: "#6ab141";
    property string darkGreen: "#00613a";
    
    property string selectedRoomID: "";
    
    id: appid
    title: qsTr("Ruqola")
    width: 800
    height: 600
    visible: true

    Shortcut {
        sequence: StandardKey.Quit
        context: Qt.ApplicationShortcut
        onActivated: Qt.quit()
    }

    Login {
        id: loginTab
        visible: (Ruqola.loginStatus == DDPClient.LoginFailed || Ruqola.loginStatus == DDPClient.LoggedOut)
//        visible: (Ruqola.loginStatus != DDPClient.LoggedIn)
        anchors.fill:parent
        z: 10
        serverURL: Ruqola.serverURL
        username: Ruqola.userName
        onAccepted: {
            Ruqola.password = loginTab.password;
            Ruqola.userName = loginTab.username;
            Ruqola.serverURL = loginTab.serverURL;
            Ruqola.tryLogin();
        }        
    }
    
    BusyIndicator {
        id: busy
        anchors.centerIn: parent
        visible: Ruqola.loginStatus == DDPClient.LoggingIn
    }

    Item {
        id: mainWidget
        anchors.fill: parent
        visible: !loginTab.visible

        Rectangle {
            id: userBox
            anchors.top: parent.top
            width: parent.width
            anchors.left: parent.left
            anchors.right: roomsList.right
            height: 40
            color: darkGreen
            Text {
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                anchors.rightMargin: 10
                anchors.fill: parent
                font.pointSize: 12
                color: "white"
                text: "Hello, " + Ruqola.userName
            }
            
        }
            
        RoomsView {
            anchors.top: userBox.bottom
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.margins: 0            
            width: 200
            height: appid.height

            id: roomsList
            
            model: Ruqola.roomModel()
            visible: parent.visible            
            selectedRoomID: appid.selectedRoomID;
            onRoomSelected: {
                if (roomID == selectedRoomID) {
                    return;
                }
                console.log("Choosing room", roomID);
                appid.selectedRoomID = roomID;
                activeChat.model = Ruqola.getModelForRoom(roomID)
                topicWidget.selectedRoom = Ruqola.getRoom(roomID)
            }
            
            onCountChanged: {
//                 console.log("We have", roomsList.count, "rooms")
            }
            
            LinearGradient {
                id: greenGradient
                anchors.fill: parent
                start: Qt.point(0, 0)
                end: Qt.point(roomsList.width, 0)
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#6ab141" }
                    GradientStop { position: 1.0; color: "#00613a" }
                }
                z: -1;
                
            }
        } //RoomsView
        
        
        Item {

            anchors.right: parent.right
            anchors.left: roomsList.right
            anchors.top: parent.top
            anchors.bottom: input.top
            id: chatView
            Rectangle {
                id: topicWidget
                color: "#fff"

                anchors.top: parent.top
                anchors.right: parent.right
                anchors.left: parent.left
                height: nameLabel.height + topicLabel.height
                property var selectedRoom;
                
                Text {
                    id: nameLabel
                    text: "#" + parent.selectedRoom.name
                    font.pointSize: 18
                    verticalAlignment: Text.AlignVCenter
                    anchors.leftMargin: 20
                    height: 40
                   // height: font.pixelSize + 10

                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    
                }
                
                Text {
                    id: topicLabel
                    text: topicWidget.selectedRoom.topic
                    
                    anchors.top: nameLabel.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    
                    horizontalAlignment: Text.AlignHCenter
                    
                    height: font.pixelSize + 10
                }

            }
            
            ScrollView {
                
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.top: topicWidget.bottom
                anchors.bottom: parent.bottom
                
                verticalScrollBarPolicy: Qt.ScrollBarAlwaysOn
//                 visible: parent.visible && (Ruqola.loginStatus != DDPClient.LoggingIn)
//                visible: !greeter.visible

         
                ListView {
                    id: activeChat
//                     model: Ruqola.getModelForRoom(selectedRoomID)

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
                    anchors.fill:parent
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
            }
        } //Item chatView

        Item {
            anchors.bottom: parent.bottom
            anchors.left: roomsList.right
            anchors.right: parent.right
            id: input
            height: 40

            TextField {
                id: messageLine
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                anchors.right: attachmentsButton.left
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

            Button {
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                anchors.right: parent.right
                width: 50
                id : attachmentsButton
                iconName: "Button"
                text: "Click"
                visible: true
                onClicked: Ruqola.attachmentButtonClicked();
            }

        }//Item input



    }// mainWidget Item
    
    Rectangle {
        z: -10
        anchors.fill: parent
        color: "white"
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
