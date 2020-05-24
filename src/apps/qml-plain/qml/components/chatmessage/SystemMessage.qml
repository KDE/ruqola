import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.9

MessageDelegateComponent {
    id: root

    property var _model: null
    property var _additionalLeftPadding: 16

    implicitHeight: mouseArea.height

    MouseArea {
        id: mouseArea

        width: parent.width
        height: layoutWrapper.height
        hoverEnabled: true
        z: 1500
        onPressed: mouse.accepted = false
        onReleased: mouse.accepted = false
        onClicked: mouse.accepted = false
        onPositionChanged: mouse.accepted = false

        Rectangle {
            id: layoutWrapper

            width: parent.width
            height: layout.height
            color: mouseArea.containsMouse ? "#F3F4F2" : "white"

            ColumnLayout {
                id: layout

                width: parent.width
                spacing: 0

                RowLayout {
                    id: messageWrapperLayout

                    Layout.fillWidth: true
                    spacing: 0

                    Rectangle {
                        Layout.leftMargin: root.padding + root._additionalLeftPadding
                        Layout.topMargin: 4
                        Layout.bottomMargin: 4
                        Layout.preferredWidth: 20
                        Layout.preferredHeight: 20
                        Layout.alignment: Qt.AlignTop
                        radius: 2
                        color: "white"
                        clip: true

                        Image {
                            source: _chatScreenController.avatarUrl(root._model.userID)
                            anchors.fill: parent
                        }
                    }

                    Item {
                        id: messageWrapper

                        Layout.fillWidth: true
                        Layout.leftMargin: 9
                        Layout.preferredHeight: messageLayout.height + 8
                        Layout.rightMargin: root.padding

                        RowLayout {
                            id: messageLayout

                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 5

                            Text {
                                id: userText
                                font.pointSize: 10
                                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                                text: root._model.username
                                color: "#444444"
                            }

                            Text {
                                id: hourText
                                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                                font.pointSize: 10
                                text: root._model.timestamp
                                color: "#a0a0a0"
                                Layout.preferredWidth: 35
                            }

                            Text {
                                id: messageTextEdit

                                Layout.fillWidth: true
                                font.pointSize: 10
                                font.italic: true
                                text: root._model.messageConverted
                                color: "#a0a0a0"
                                wrapMode: Text.WordWrap
                                // selectByMouse: true
                                // cursorVisible: false
                                textFormat: Text.RichText
                                onLinkActivated: { /* TODO */ }
                            }
                        }
                    }
                }

                Loader {
                    id: jitsiButtonLoader

                    Layout.leftMargin: root.padding + root._additionalLeftPadding
                    Layout.fillWidth: true
                    active: root._model.type == "jitsi_call_started"

                    sourceComponent: Item {
                        height: joinVideoButton.height + 4

                        Button {
                            id: joinVideoButton

                            height: 26
                            width: 96
                            anchors.top: parent.top
                            anchors.horizontalCenter: parent.horizontalCenter

                            MouseArea {
                                id: buttonMouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                onPressed: mouse.accepted = false
                                onReleased: mouse.accepted = false
                                onClicked: mouse.accepted = false
                                onPositionChanged: mouse.accepted = false
                            }

                            background: Rectangle {
                                color: buttonMouseArea.containsMouse ? "#0B3E5C" : "#13679A"
                                radius: 5
                            }

                            Text {
                                color: "white"
                                text: "Click to Join!"
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                font.pointSize: 9
                            }
                        }
                    }
                }
            }
        }
    }
}