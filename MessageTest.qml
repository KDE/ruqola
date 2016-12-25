
import QtQuick 2.7

Item {
    width: 200
    height: 200
//     Text {
//         text: "If height and width are not explicitly set, Text will attempt to determine how much room is needed and set it accordingly. Unless wrapMode is set, it will always prefer width to height (all text will be placed on a single line)."
//         width: parent.width
//         wrapMode:Text.Wrap
//     }
//     }
    ListView {
        anchors.fill: parent
        model: RoomModel { }
        delegate: Message {
            i_messageText: messageText
            i_username: name
            width: parent.width
        }
    }
    
}
