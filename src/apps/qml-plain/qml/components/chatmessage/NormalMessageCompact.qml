import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.9

MessageDelegateComponent {
    id: root

    property var _model: null

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

            RowLayout {
                id: layout
                width: parent.width
                spacing: 0

                Rectangle {
                    Layout.leftMargin: root.padding
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
                            text: "@" + root._model.username
                            color: "#a0a0a0"
                            Layout.preferredWidth: 70
                            elide: Text.ElideRight
                        }

                        Text {
                            id: hourText
                            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                            font.pointSize: 10
                            text: root._model.timestamp
                            color: "#a0a0a0"
                            Layout.preferredWidth: 35
                        }

                        TextEdit {
                            id: messageTextEdit

                            Layout.fillWidth: true
                            font.pointSize: 10
                            text: root._model.messageConverted
                            color: "#444444"
                            wrapMode: Text.WordWrap
                            selectByMouse: true
                            cursorVisible: false
                            textFormat: Text.RichText
                            Keys.onPressed: event.accepted = true
                            onLinkActivated: { /* TODO */ }
                        }

                        RowLayout {
                            id: iconsLayout

                            Layout.preferredWidth: 14
                            spacing: 12
                            Layout.alignment: Qt.AlignTop | Qt.AlignRight

                            Button {
                                Layout.preferredHeight: 14
                                Layout.preferredWidth: 14

                                background: Rectangle {
                                    anchors.fill: parent
                                    color: "transparent"
                                }

                                Image {
                                    source: mouseArea.containsMouse ? _iconsModelQml.menuIcon : ""
                                    sourceSize.width: 14
                                    sourceSize.height: 14
                                }

                                onClicked: {
                                    if (!mouseArea.containsMouse) {
                                        return;
                                    }
                                    var globalPos = mapToGlobal(0, 0);
                                    root.menuButtonClicked(globalPos.x, globalPos.y);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}