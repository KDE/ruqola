import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12

Item {
    id: root

    signal clicked()

    width: parent.width
    height: 34

    property string roomName: ""
    property string roomType: ""
    property string roomId: ""
    property int roomUnread: 0
    property bool roomAlert: false
    property string avatarURL: ""
    property var avatarsModel: null

    Repeater {
        model: _chatScreenController.roomAvatarsModel
        Item {
            Component.onCompleted: root.avatarsModel = model
        }
    }

    Button {
        id: btn
        contentItem: Item {
            anchors.fill: parent
            RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 20
                Rectangle {
                    Layout.preferredWidth: 27
                    Layout.preferredHeight: 27
                    Layout.alignment: Qt.AlignVCenter
                    radius: 2
                    color: "blue"
                    clip: true

                    Image {
                        source: {
                            if (roomType === "d") {
                                return root.avatarURL;
                            }
                            else {
                                return root.avatarsModel[roomName];
                            }
                        }
                        anchors.fill: parent
                    }
                }
                Item {
                    Layout.preferredWidth: 25
                    Rectangle {
                        width: 6
                        height: 6
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        radius: 3
                        color: "green"
                        visible: roomType === "d"
                    }
                }
                Text {
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter

                    font.pointSize: 10
                    font.bold: roomUnread != 0
                    color: (roomUnread || roomAlert) ? "white" : "#9EA2A8"
                    text: {
                        if (roomType === "c") {
                            return "#" + roomName;
                        } else {
                            return roomName;
                        }
                    }
                }
            }
        }

        anchors.fill: parent
        checked: roomId == _chatScreenController.currentRoom
        onClicked: root.clicked()
        background: Rectangle {
            anchors.fill: parent
            visible: btn.checked || btn.hovered
            radius: 2
            color: btn.checked ? "#414852" : "#1F2329"
        }

        Rectangle {
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 26
            visible: roomAlert && roomUnread !== 0
            width: 18
            height: 18
            radius: 9
            color: "#414852"
            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: roomUnread
                color: "white"
            }
        }
    }
}