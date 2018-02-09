import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import KDE.Ruqola.RocketChatAccount 1.0
import KDE.Ruqola.DDPClient 1.0
import org.kde.kirigami 2.1 as Kirigami

TextField {
    id: messageLine
    //TODO add background style.
    
    
    //FIXME add multiline !!!
    inputMethodHints: Qt.ImhMultiLine
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    
    Layout.fillWidth: true
    placeholderText: i18n("Enter message")
    
    onAccepted: {
        if (text != "" && rcAccount.loginStatus === DDPClient.LoggedIn && (selectedRoomID !== "")) {
            if (messageId !== "") {
                if (text !== savePreviousMessage) {
                    rcAccount.updateMessage(selectedRoomID, messageId, text);
                }
                savePreviousMessage = "";
                messageId = "";
            } else {
                rcAccount.sendMessage(selectedRoomID, text);
            }
            text = "";
        }
    }
    onTextChanged: {
        footerItem.textEditing(text)
    }
    Shortcut {
        sequence: "Escape"
        onActivated: {
            clearUnreadMessages();
        }
    }
    Keys.onUpPressed: {
        console.log("move up");
    }
}
