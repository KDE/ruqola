
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
        model:             
            ListModel {
                ListElement {
                    name: "luca.toma"
                    messageText: "Hey! I released the new WikiToLearn version"
                    timestamp: 1476277459413
                }
                ListElement {
                    name: "m_bona"
                    messageText: "This is a very very very long message.This is a very very very long message.This is a very very very long message.This is a very very very long message.This is a very very very long messageThis is a very very very long message.This is a very very very long message.This is a very very very long message."
                    timestamp: 1498134201519
                }
                ListElement {
                    name: "tunale"
                    messageText: "CourseEditor rocks!"
                    timestamp: 1484277459413
                }
            }

        delegate: FancyMessageDelegate {
            i_messageText: messageText
            i_username: name
            i_timestamp: timestamp
            width: parent.width
        }
    }
    
}
