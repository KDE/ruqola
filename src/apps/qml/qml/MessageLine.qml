/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (C) 2017-2020 Laurent Montel <montel@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


import QtQuick 2.9
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Layouts 1.12
import Ruqola 1.0

import org.kde.kirigami 2.7 as Kirigami

ColumnLayout {
    id: messageLineItem
    property alias messageLineText: messageLine.text
    property string savePreviousMessage
    readonly property int popupheight: 100
    property string selectedRoomId
    property string messageId
    property string threadmessageId
    property string selectedThreadMessage

    property QtObject inputCompleterModel

    function sendMessage()
    {
        var text = messageLine.text;
        if (text === "")
            return;
        if (appid.rocketChatAccount.loginStatus !== DDPClient.LoggedIn)
            return;

        if (messageLineItem.selectedThreadMessage !== "") {
            //console.log("In thread message" + messageLineItem.selectedThreadMessage + messageLineItem.selectedRoomId)
            appid.rocketChatAccount.replyOnThread(messageLineItem.selectedRoomId, messageLineItem.selectedThreadMessage, text);
        } else if (messageLineItem.selectedRoomId !== "") {
            //Modify text.
            if (messageId !== "") {
                //Reply against message
                if (savePreviousMessage == "") {
                    //console.log("Previous message  empty")
                    appid.rocketChatAccount.sendMessage(messageLineItem.selectedRoomId, text, messageId);
                } else if (text !== savePreviousMessage) {
                    //console.log("Previous message  empty text different")
                    appid.rocketChatAccount.updateMessage(messageLineItem.selectedRoomId, messageId, text);
                }
            } else if (threadmessageId !== "") { //Reply in thread
                appid.rocketChatAccount.replyOnThread(messageLineItem.selectedRoomId, threadmessageId, text);
            } else {
                appid.rocketChatAccount.sendMessage(messageLineItem.selectedRoomId, text);
            }
        }

        //clear all element
        messageLine.text = "";
        threadmessageId = "";
        savePreviousMessage = "";
    }

    function setOriginalMessage(messageStr)
    {
        messageLine.text = messageStr
        savePreviousMessage = messageStr
        messageLine.selectAll()
    }

    function insertEmoticon(emotiStr)
    {
        messageLine.insert(messageLine.cursorPosition, emotiStr)
    }

    QQC2.TextField {
        id: messageLine
        selectByMouse: true
        //FIXME add multiline !!!
        inputMethodHints: Qt.ImhMultiLine

        Layout.fillWidth: true
        placeholderText: i18n("Enter message...")
        onTextChanged: {
            appid.rocketChatAccount.setInputTextChanged(text, cursorPosition);
            if (listView.count > 0) {
                showPopupCompleting()
            } else {
                popup.close()
            }
            footerItem.textEditing(text)
        }
        Keys.onDownPressed: {
            listView.incrementCurrentIndex()
        }
        Keys.onUpPressed: {
            listView.decrementCurrentIndex()
        }
        Keys.onTabPressed: {
            if (listView.currentItem) {
                textSelected(listView.currentItem.myData.completername)
            }
        }
        Keys.onEnterPressed: enterOrReturnPressed(event)
        Keys.onReturnPressed: enterOrReturnPressed(event)

        function enterOrReturnPressed(event) {
            if (event.modifiers & Qt.ShiftModifier) {
                event.accepted = false;
                return; // composing a multi-line message
            }

            if (listView.currentItem) {
                textSelected(listView.currentItem.myData.completername)
            } else {
                messageLineItem.sendMessage()
            }
        }

        QQC2.Popup {
            id: popup

            x: 0
            height: listView.delegateHeight * listView.count
            y: -height - 10
            padding: 0
            width: messageLine.width
            contentHeight: rect.height
            visible: listView.count > 0

            Rectangle {
                id: rect

                anchors.top: popup.top
                anchors.left: popup.left

                height: popup.height
                width: popup.width

                ListView {
                    id: listView

                    readonly property int delegateHeight: count > 0 ? contentItem.children[0].height : 0

                    height: popup.height
                    width: parent.width
                    interactive: true
                    clip: true
                    model: inputCompleterModel
                    delegate: Kirigami.BasicListItem {
                        readonly property variant myData: model

                        icon: model.iconname

                        label: model.displayname
                        onClicked: {
                            listView.currentIndex = model.index
                            textSelected(model.completername)
                        }
                        highlighted: focus && ListView.isCurrentItem
                    }
                    QQC2.ScrollIndicator.vertical: QQC2.ScrollIndicator { }
                }
            }
        }
        function showPopupCompleting() {
            if (!popup.visible) {
                popup.open()
                listView.currentIndex = 0
            }
        }
    }

    function textSelected(completerName) {
        if (listView.currentItem) {
            messageLine.text = appid.rocketChatAccount.replaceWord(completerName + " ", messageLine.text, messageLine.cursorPosition)
        }
        popup.close()
    }
    Keys.onEscapePressed: {
        if (popup.visible) {
            popup.close()
            //Clear modified message
        } else if (savePreviousMessage !== "" || threadmessageId !== "") {
            savePreviousMessage = "";
            messageId = "";
            threadmessageId = "";
            messageLine.text = "";
        } else {
            clearUnreadMessages();
        }
    }
}
