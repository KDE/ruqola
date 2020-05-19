import QtQuick 2.14
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.9

import "../delegates"
import "../components"

Rectangle {
    id: root

    property bool aboutToChangeModel: false

    color: "#FFFFFF"

    // ColumnLayout {
    //     anchors.fill: parent

    ScrollView {
        ScrollBar.vertical.policy: ScrollBar.AlwaysOn
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        clip: true

        width: parent.width
        anchors.top: parent.top
        anchors.bottom: messageLayoutContainer.top

        ListView {
            id: chatListView
            spacing: 0
            clip: true
            anchors.fill: parent
            model: _chatScreenController.messageModel
            boundsBehavior: Flickable.StopAtBounds

            // These properties are used to adjust the scroll position while loading new messages
            property int oldCount: 0
            property bool loadHistoryCalled: false
            property bool positioning: false
            property int currentIndexOnTop: 0
            property int currentIndexOnTopDeltaY: 0
            // If the user scrolled to the bottom of the list, the UI should snap to the bottom when resized,
            // otherwise it should keep the item on top on its current position.
            readonly property bool snapToBottomOnResize: atYEnd

            onContentYChanged: {
                // Changing model causes oscillations in contentY values and it ends up loading history every time
                // using _chatScreenController.aboutToChangeModel to avoid that.
                // Start loading the history when we still have a few messages (10 in this case) so the scrolling
                // can stay fluid while the backend loads new messages
                if (itemAtIndex(10) && contentY <= itemAtIndex(10).y && !loadHistoryCalled && !positioning && !_chatScreenController.aboutToChangeModel) {
                    oldCount = count;
                    _chatScreenController.loadHistory();
                    loadHistoryCalled = true;
                }

                // Keep track of the current item on top while loading new messages
                // so when the new messages are loaded the current first item is snapped on top
                // Keep also track of its distance from the top so when new items are loaded
                // the list doesn't scroll at all instead of snapping the current first item
                // on top.
                // Use the same properties to keep the first item on top with the same delta y
                // while resizing the window
                if (/*loadHistoryCalled && */ itemAt(0, contentY) && !positioning) {
                    currentIndexOnTop = indexAt(0, contentY);
                    currentIndexOnTopDeltaY = contentY - itemAt(0, contentY).y;
                }
            }

            onWidthChanged: {
                positioning = true;
                for (var i = 0; i < 5; i++) {
                    if (snapToBottomOnResize) {
                        positionViewAtIndex(count - 1, ListView.End);
                    }
                    else {
                        positionViewAtIndex(currentIndexOnTop, ListView.Beginning);
                    }
                }
                contentY += currentIndexOnTopDeltaY;
                positioning = false;
            }

            onHeightChanged: {
                if (!snapToBottomOnResize) {
                    return;
                }

                for (var i = 0; i < 5; i++) {
                    positionViewAtIndex(count - 1, ListView.End);
                }
            }

            onCountChanged: {
                if (count === 0) return;

                if (!loadHistoryCalled) {
                    positioning = true;
                    for (var i = 0; i < 5; i++) {
                        positionViewAtIndex(count - 1, ListView.End);
                    }
                    positioning = false;
                }
                else {
                    positioning = true;
                    var newItemIndex = count - oldCount + currentIndexOnTop;
                    for (var i = 0; i < 5; i++) {
                        positionViewAtIndex(newItemIndex, ListView.Beginning);
                    }
                    contentY += currentIndexOnTopDeltaY;
                    // Fix currentIndexOnTop and delta Y in case the user resizes before scrolling again.
                    currentIndexOnTop = indexAt(0, contentY);
                    currentIndexOnTopDeltaY = contentY - itemAt(0, contentY).y;
                    positioning = false;
                    loadHistoryCalled = false;
                }
            }

            // Model update is complete (see QQuickItemView::setModel) so we can reset the guard
            onModelChanged: _chatScreenController.resetAboutToChangeModel()

            delegate: ChatMessageDelegate {
                _model: model
                width: chatListView.width

                // Component.onCompleted: {
                //     for (var x in model) {
                //         console.log(x, model[x]);
                //     }
                // }

                contentPadding: 24
                onReactionDialogRequested: function (_x, _y) {
                    var desiredPos = root.mapFromGlobal(_x, _y);
                    _emojiPopupController.placePopup(desiredPos.x, desiredPos.y,
                            emojiDialog.width, emojiDialog.height,
                            chatListView.width, chatListView.height);
                    _emojiPopupController.startPickForMessageReaction(model.messageID);
                }

                onReactionButtonClicked: function(emojiId) {
                    _chatScreenController.toggleMessageReaction(model.messageID, emojiId);
                }

                onMenuButtonClicked: {
                    // TODO: Move to a message menu popup controller
                    messageMenuPopup.messageId = model.messageID;
                    messageMenuPopup.roomId = model.roomID;
                    messageMenuPopup.open();
                }
            }
        }
    }

    Item {
        id: messageLayoutContainer

        anchors.bottom: parent.bottom

        width: parent.width
        height: messageLayoutRectangle.height + 48

        Rectangle {
            id: messageLayoutRectangle

            border.color: "#CBCED1"
            border.width: 2

            radius: 2

            width: parent.width - 32
            height: messageLayout.height + 24
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            RowLayout {
                id: messageLayout

                width: parent.width
                height: messageText.height
                anchors.verticalCenter: parent.verticalCenter

                Button {
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 20
                    Layout.leftMargin: 14
                    Layout.alignment: Qt.AlignVCenter

                    background: Rectangle {
                        anchors.fill: parent
                        color: "white"
                    }

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: _iconsModelQml.emojiIcon
                        sourceSize.width: 20
                        sourceSize.height: 20
                    }

                    onClicked: {
                        var desiredPos = messageText.mapToGlobal(0, 0);
                        desiredPos = root.mapFromGlobal(desiredPos.x, desiredPos.y);
                        _emojiPopupController.placePopup(desiredPos.x, desiredPos.y,
                            emojiDialog.width, emojiDialog.height,
                            chatListView.width, chatListView.height);
                        _emojiPopupController.startPickForMessageTyping(messageText._textEdit);
                    }
                }

                FancyTextInput {
                    id: messageText

                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter

                    color: "#444444"
                    font.pointSize: 10
                    placeholderText: "Message"
                    placeholderColor: "#CED1D4"

                    function sendMessage() {
                        if (text.length === 0)
                            return;

                        _chatScreenController.sendMessage(text);
                        text = "";
                        messagePreview.close();
                    }
                }


                Button {
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 20
                    Layout.leftMargin: 16
                    Layout.rightMargin: 16
                    Layout.alignment: Qt.AlignVCenter
                    visible: messageText.text.length === 0
                    background: Rectangle {
                        color: "white"
                    }

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: _iconsModelQml.micIcon
                        sourceSize.width: 20
                        sourceSize.height: 20
                    }
                }

                Button {
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 20
                    Layout.rightMargin: 16
                    Layout.alignment: Qt.AlignVCenter
                    visible: messageText.text.length === 0
                    background: Rectangle {
                        color: "white"
                    }

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: _iconsModelQml.plusIcon
                        sourceSize.width: 20
                        sourceSize.height: 20
                    }
                }

                Button {
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 20
                    Layout.rightMargin: 16
                    Layout.alignment: Qt.AlignVCenter
                    visible: messageText.text.length > 0
                    background: Rectangle {
                        color: "white"
                    }

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: _iconsModelQml.eyeIcon
                        sourceSize.width: 20
                        sourceSize.height: 20
                    }

                    onClicked: {
                        if (messagePreview.opened) {
                            messagePreview.close();
                        }
                        else {
                            messagePreview.open();
                        }
                    }
                }

                Button {
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 20
                    Layout.rightMargin: 16
                    Layout.alignment: Qt.AlignVCenter
                    visible: messageText.text.length > 0
                    background: Rectangle {
                        color: "white"
                    }

                    Image {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: _iconsModelQml.sendIcon
                        sourceSize.width: 20
                        sourceSize.height: 20
                    }

                    onClicked: messageText.sendMessage()
                }
            }
        }
    }


    Popup {
        id: messagePreview
        width: messageLayoutContainer.width - 30
        x: messageLayoutContainer.x + 15
        y: messageLayoutContainer.y - height - 10
        closePolicy: Popup.CloseOnEscape | Popup.CloseOn
        // height: textItem.height + 10

        contentItem: Text {
            id: textItem
            textFormat: Text.RichText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width - 10
            text: _chatScreenController.transform(messageText.text)
        }
    }

    MessageMenuPopup {
        id: messageMenuPopup

        // TODO: factor out message menu behaviour in a separate controller
        onRemoveClicked: _chatScreenController.removeMessage(messageMenuPopup.roomId, messageMenuPopup.messageId)
    }

    EmojiDialog {
        id: emojiDialog

        model: _chatScreenController.emojiModel

        Connections {
            target: _emojiPopupController
            onOpen: emojiDialog.open()
            onClose: emojiDialog.close()
        }

        onClosed: _emojiPopupController.reset()

        x: _emojiPopupController.xpos
        y: _emojiPopupController.ypos

        // onOpen: open()

        onEmojiPicked: function(unicodeDisplay, identifier) {
            _emojiPopupController.pick(identifier);
        }
    }
}