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
        height: layoutRect.height
        hoverEnabled: true
        z: 1500
        onPressed: mouse.accepted = false
        onReleased: mouse.accepted = false
        onClicked: mouse.accepted = false
        onPositionChanged: mouse.accepted = false

        Rectangle {
            id: layoutRect

            width: parent.width
            height: layout.height
            color: mouseArea.containsMouse ? "#F3F4F2" : "white"

            RowLayout {
                id: layout
                width: parent.width
                spacing: 0

                Rectangle {
                    id: avatar

                    Layout.leftMargin: root.padding
                    Layout.topMargin: 8
                    Layout.bottomMargin: 8
                    Layout.preferredWidth: 36
                    Layout.preferredHeight: 36
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

                    Layout.leftMargin: 9
                    Layout.rightMargin: root.padding
                    Layout.fillWidth: true
                    Layout.preferredHeight: messageLayout.height + 16
                    
                    ColumnLayout {
                        id: messageLayout

                        width: parent.width
                        anchors.top: parent.top
                        anchors.topMargin: 8

                        RowLayout {
                            Layout.fillWidth: true
                            Text {
                                font.bold: true
                                font.pointSize: 10
                                color: "#444444"
                                text: root._model.alias.length ? root._model.alias : root._model.username
                            }
                            Text {
                                font.pointSize: 10
                                text: "@" + root._model.username
                                color: "#a0a0a0"
                            }
                            Text {
                                font.pointSize: 10
                                text: root._model.timestamp
                                color: "#a0a0a0"
                            }

                            Item {
                                Layout.fillWidth: true
                            }

                            RowLayout {
                                Layout.preferredWidth: 3 * 14 + 2 * 12
                                spacing: 12
                                // TODO: handle mouse through a listview proxy QObject
                                visible: mouseArea.containsMouse

                                Button {
                                    Layout.preferredHeight: 14
                                    Layout.preferredWidth: 14

                                    background: Rectangle {
                                        anchors.fill: parent
                                        color: "transparent"
                                    }

                                    Image {
                                        source: _iconsModelQml.addReactionIcon
                                        sourceSize.width: 14
                                        sourceSize.height: 14
                                    }

                                    onClicked: {
                                        var globalPos = mapToGlobal(0, 0);
                                        reactionDialogRequested(globalPos.x, globalPos.y);
                                    }
                                }

                                Button {
                                    Layout.preferredHeight: 14
                                    Layout.preferredWidth: 14

                                    background: Rectangle {
                                        anchors.fill: parent
                                        color: "transparent"
                                    }

                                    Image {
                                        source: _iconsModelQml.threadIcon
                                        sourceSize.width: 14
                                        sourceSize.height: 14
                                    }
                                }

                                Button {
                                    Layout.preferredHeight: 14
                                    Layout.preferredWidth: 14

                                    background: Rectangle {
                                        anchors.fill: parent
                                        color: "transparent"
                                    }

                                    Image {
                                        source: _iconsModelQml.menuIcon
                                        sourceSize.width: 14
                                        sourceSize.height: 14
                                    }

                                    onClicked: {
                                        var globalPos = mapToGlobal(0, 0);
                                        root.menuButtonClicked(globalPos.x, globalPos.y);
                                    }
                                }
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true

                            spacing: 0

                            Loader {
                                id: textMessageLoader

                                Layout.fillWidth: true

                                active: root._model.originalMessage.length > 0

                                sourceComponent: TextEdit {
                                    id: messageTextEdit

                                    font.pointSize: 10
                                    text: root._model.messageConverted
                                    color: "#444444"
                                    wrapMode: Text.WordWrap
                                    selectByMouse: true
                                    cursorVisible: false
                                    textFormat: Text.RichText
                                    selectionColor: "#3390FF"
                                    selectedTextColor: "#FFFFFF"
                                    visible: root._model.originalMessage.length > 0

                                    Keys.onPressed: event.accepted = true
                                    // HoverHandler {
                                    //     id: textEditHoverHandler
                                    // }

                                    onLinkActivated: { /* TODO */ }
                                }
                            }

                            Repeater {
                                model: root._model.urls

                                UrlPreview {
                                    Layout.topMargin: 2
                                    messageUrl: modelData
                                }
                            }

                            Loader {
                                Layout.fillWidth: true
                                Layout.topMargin: (active && textMessageLoader.active) ? 5 : 0

                                active: {
                                    return root._model.messagetype == 3 // File
                                            || root._model.messagetype == 4 // video
                                            || root._model.messagetype == 5 // audio
                                            || root._model.messagetype == 6 // image
                                }

                                sourceComponent: {
                                    switch (root._model.messagetype) {
                                        case 2: // normal text
                                            return null;
                                        case 3: // file
                                            return null; // not implemented yet
                                        case 4: // video
                                            return videoAttachmentComponent;
                                        case 5: // audio
                                            return audioAttachmentComponent;
                                        case 6: // image
                                            return imageAttachmentComponent; // not implemented yet
                                        default:
                                            return null;
                                    }
                                }

                                Component {
                                    id: audioAttachmentComponent

                                    AudioAttachment {
                                        attachment: root._model.attachments[0]
                                    }
                                }

                                Component {
                                    id: videoAttachmentComponent

                                    VideoAttachment {
                                        attachment: root._model.attachments[0]
                                    }
                                }

                                Component {
                                    id: imageAttachmentComponent

                                    ImageAttachment {
                                        attachment: root._model.attachments[0]
                                    }
                                }
                            }
                        }

                        RowLayout {
                            visible: root._model.reactions && root._model.reactions.length

                            Repeater {
                                model: root._model.reactions

                                Button {
                                    Layout.preferredHeight: 22
                                    Layout.preferredWidth: 35

                                    background: Rectangle {
                                        anchors.fill: parent
                                        border.width: 1
                                        radius: 4
                                        // TODO: add focus colors
                                        border.color: "#E7E7E7"
                                    }
                                    
                                    Text {
                                        anchors.left: parent.left
                                        anchors.leftMargin: 5
                                        anchors.verticalCenter: parent.verticalCenter

                                        text: modelData.convertedReactionName
                                    }

                                    Text {
                                        anchors.right: parent.right
                                        anchors.rightMargin: 5
                                        anchors.verticalCenter: parent.verticalCenter

                                        text: modelData.count
                                        color: "#ABABAB"
                                    }

                                    onClicked: root.reactionButtonClicked(modelData.reactionName)
                                }
                            }

                            Button {
                                Layout.preferredHeight: 22
                                Layout.preferredWidth: 25

                                background: Rectangle {
                                    anchors.fill: parent
                                    border.width: 1
                                    radius: 4
                                    // TODO: add focus colors
                                    border.color: "#E7E7E7"
                                }

                                Text {
                                    anchors.right: parent.right
                                    anchors.rightMargin: 5
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.verticalCenter: parent.verticalCenter

                                    text: "+"
                                    color: "#ABABAB"
                                }

                                onClicked: {
                                    var globalPos = mapToGlobal(0, 0);
                                    reactionDialogRequested(globalPos.x, globalPos.y);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}