import QtQuick 2.12
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import QtMultimedia 5.14

import "../components/chatmessage"

Item {
    id: root

    // see MesssageModel
    property var _model: null

    property string messageAlias: model.alias
    property string messageUserName: model.username
    property string messageTimestamp: model.timestamp
    property string messageText: model.messageConverted
    property string messageAvatar: _chatScreenController.avatarUrl(model.userID)
    property string messageDate: model.date
    property var messageReactions: model.reactions
    property int messageType: model.messagetype
    property bool showDate: model.dateDiffersFromPrevious

    property real contentPadding: 0
    property bool compact: _settings.compactMode

    implicitHeight: layout.height + daySeparatorLoader.height

    signal reactionDialogRequested(real x, real y)
    signal reactionButtonClicked(string emojiId)
    signal menuButtonClicked(real x, real y)

    Loader {
        id: daySeparatorLoader
        active: root.showDate

        width: parent.width
        height: root.showDate ? 25 : 0

        sourceComponent: DaySeparator {
            id: daySeparator
            text: root.messageDate
            padding: root.contentPadding
        }
    }

    Rectangle {
        width: parent.width
        height: layout.height
        anchors.top: daySeparatorLoader.bottom
        color: (mouseArea.containsMouse || (root.compact && textEditCompactHoverHandler.hovered)) ? "#F3F4F2" : "white"

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            z: 1500
            onPressed: mouse.accepted = false
            onReleased: mouse.accepted = false
            onClicked: mouse.accepted = false
            onPositionChanged: mouse.accepted = false

            RowLayout {
                id: layout
                width: parent.width
                spacing: 0

                Rectangle {
                    Layout.leftMargin: root.contentPadding
                    Layout.topMargin: root.compact ? 4 : 8
                    Layout.bottomMargin: root.compact ? 4 : 8
                    Layout.preferredWidth: root.compact ? 20 : 36
                    Layout.preferredHeight: root.compact ? 20 : 36
                    Layout.alignment: Qt.AlignTop
                    radius: 2
                    color: "red"
                    clip: true

                    Image {
                        source: root.messageAvatar
                        anchors.fill: parent
                    }
                }

                StackLayout {
                    Layout.fillWidth: true
                    Layout.rightMargin: contentPadding
                    Layout.leftMargin: 9

                    currentIndex: root.compact ? 0 : 1
                    Layout.preferredHeight: root.compact ? compactItem.height : fullHeightItem.height

                    Item {
                        id: compactItem

                        Layout.fillWidth: true
                        height: compactLayout.height + 8

                        RowLayout {
                            id: compactLayout

                            anchors.left: parent.left
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            spacing: 5

                            Text {
                                id: userCompact
                                font.pointSize: 10
                                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                                text: "@" + root.messageUserName
                                color: "#a0a0a0"
                                Layout.preferredWidth: 70
                                elide: Text.ElideRight
                            }

                            Text {
                                id: hourCompact
                                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                                font.pointSize: 10
                                text: root.messageTimestamp
                                color: "#a0a0a0"
                                Layout.preferredWidth: 35
                            }

                            TextEdit {
                                id: messageTextEditCompact

                                Layout.fillWidth: true
                                font.pointSize: 10
                                text: root.messageText
                                color: "#444444"
                                wrapMode: Text.WordWrap
                                selectByMouse: true
                                cursorVisible: false
                                textFormat: Text.RichText
                                HoverHandler {
                                    id: textEditCompactHoverHandler
                                }

                                onLinkActivated: { /* TODO */ }
                            }


                            RowLayout {
                                id: compactIconsLayout

                                implicitWidth: 3 * 14 + 2 * 12
                                Layout.preferredWidth: implicitWidth
                                spacing: 12
                                Layout.alignment: Qt.AlignTop | Qt.AlignRight
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
                                        // radius: 7
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
                    }

                    Item {
                        id: fullHeightItem

                        Layout.fillWidth: true
                        height: fullHeightLayout.height + 16
                        
                        ColumnLayout {
                            id: fullHeightLayout

                            width: parent.width
                            anchors.top: parent.top
                            anchors.topMargin: 8

                            RowLayout {
                                Layout.fillWidth: true
                                Text {
                                    font.bold: true
                                    font.pointSize: 10
                                    color: "#444444"
                                    text: root.messageAlias.length ? root.messageAlias : root.messageUserName
                                }
                                Text {
                                    font.pointSize: 10
                                    text: "@" + root.messageUserName
                                    color: "#a0a0a0"
                                }
                                Text {
                                    font.pointSize: 10
                                    text: root.messageTimestamp
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

                                        Layout.fillWidth: true
                                        font.pointSize: 10
                                        text: root.messageText
                                        color: "#444444"
                                        wrapMode: Text.WordWrap
                                        selectByMouse: true
                                        cursorVisible: false
                                        textFormat: Text.RichText
                                        selectionColor: "#3390FF"
                                        selectedTextColor: "#FFFFFF"
                                        visible: root._model.originalMessage.length > 0

                                        Keys.onPressed: event.accepted = true
                                        HoverHandler {
                                            id: textEditHoverHandler
                                        }

                                        onLinkActivated: { /* TODO */ }
                                    }
                                }

                                Repeater {
                                    model: root._model.urls

                                    UrlPreview {
                                        Layout.topMargin: 2
                                        messageUrl: modelData
                                        // Component.onCompleted: {
                                        //     for (var x in model) {
                                        //         console.log(x, model[x]);
                                        //     }
                                        // }
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
                                visible: root.messageReactions && root.messageReactions.length

                                Repeater {
                                    model: root.messageReactions

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
}