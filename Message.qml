

import QtQuick 2.0
// import "marked.js" as Markdown
import "marked.js" as MarkDown;

Rectangle {
    
    function getTextFor(type) {
        
//         console.log(JSON.stringify(markdown));
        if (type == "uj") {
            return qsTr("has joined the channel");
        } else if (type == "ul") {
            return qsTr("has left the channel");
        } else {
            return qsTr("Unknown action!");
        }
    }   
    
    property string i_messageText
    property string i_username
    property bool i_systemMessage
    property string i_systemMessageType

    id: messageMain
    color: "#eeeeee"
    implicitHeight: textLabel.contentHeight

    
    Text {
        color: i_systemMessage? "#999" : "#555"
        text: i_username
        id: usernameLabel
        width: 80
        horizontalAlignment: Text.AlignRight
        
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }
    
    Text {
        color: i_systemMessage? "#999" : "#111"
        id: textLabel
        text: i_systemMessage? getTextFor(i_systemMessageType) : MarkDown.md2html(i_messageText)
            
        
        
//         width: parent.width
        
        anchors.top: parent.top
        anchors.left: usernameLabel.right
        anchors.right: parent.right
        
        wrapMode:Text.Wrap
        
        anchors.leftMargin: 5

//         visible: !i_systemMessage
        //         horizontalAlignment: Text.AlignRight
    }
    
     
}
