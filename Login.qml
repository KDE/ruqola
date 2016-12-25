

import QtQuick 2.7
import QtQuick.Controls 1.3
Item {
    
    property alias username: usernameField.text;
    property alias password: passField.text;
    property alias serverURL: urlField.text;

    signal accepted()
    
    id: loginForm
//     color: "#eeeeee"
    implicitHeight: 400
    implicitWidth: 300

    Column {
        id: form
        anchors.centerIn: parent
        width: 0.8*parent.width
        spacing: 3
        Text {
            text: "Ruqola Log in"
            color: "#555"
            id: loginLabel
            font.pixelSize: 20
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
        }
        
        Item {
            id: spacer
            width: 30
            height: 30
        }
        
        Text {
            width: parent.width
            text:"Rocket Chat Server"
            
        }
        TextField {
            id: urlField
//             text: loginForm.serverURL
            width: parent.width
            placeholderText: qsTr("Enter address of the server")
        }
        
        Text {
            id:username
            width: parent.width
            text:"Enter your username"
            
        }
        TextField {
            width: parent.width
            id: usernameField
            placeholderText: qsTr("Enter username")
        }
        
        Text {
            id: passLabel
            width: parent.width
            text:"Enter your password" 
        }
        
        TextField {
            width: parent.width
            id:passField
            echoMode: TextInput.Password
            inputMethodHints: Qt.ImhHiddenText
            placeholderText: qsTr("Enter password")
        }
        
        Item {
            id: spacer2
            width: 30
            height: 30
        }
        
        Button {
            id: acceptingButton
            width: parent.width
            text: qsTr("Log in")
            enabled: (passField.text && urlField.text && usernameField.text)
            onClicked: loginForm.accepted()
        }
        
    }
    
//     Component.onCompleted: {
//         acceptingButton.clicked.connect(loginForm.accepted)
//     }
    
     
}
