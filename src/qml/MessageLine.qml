/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (C) 2017-2018 Laurent Montel <montel@kde.org>
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
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Layouts 1.1

import KDE.Ruqola.RocketChatAccount 1.0
import KDE.Ruqola.InputCompleterModel 1.0
import KDE.Ruqola.DDPClient 1.0
import org.kde.kirigami 2.1 as Kirigami

ColumnLayout {
    property alias messageLineText: messageLine.text
    property string savePreviousMessage

    function setOriginalMessage(messageStr)
    {
        messageLine.text = messageStr
        savePreviousMessage = messageStr
        messageLine.selectAll()
    }

    TextField {
        id: messageLine
        //TODO add background style.


        //FIXME add multiline !!!
        inputMethodHints: Qt.ImhMultiLine

        Layout.fillWidth: true
        placeholderText: i18n("Enter message")
        onTextChanged: {
            //Reactivate when autocompletion is ok.
            /*
            if (listView.count > 0) {
                showPopupCompleting()
            } else {
                popup.close()
            }
            */
            footerItem.textEditing(text)
        }
        onAccepted: {
            if (text != "" && rcAccount.loginStatus === DDPClient.LoggedIn && (selectedRoomID !== "")) {
                if (messageId !== "") {
                    if (text !== savePreviousMessage) {
                        rcAccount.updateMessage(selectedRoomID, messageId, text);
                    }
                    savePreviousMessage = "";
                    messageId = "";
                } else {
                    rcAccount.sendMessage(selectedRoomID, text);
                }
                text = "";
            }
        }
        Keys.onEscapePressed: {
            clearUnreadMessages();
        }
        QQC2.Popup {
            id: popup
            x: 0
            y: /*messageLine.height*/-100 //TODO customize it.
            padding: 0
            width: messageLine.width
            contentHeight: rect.height
            visible: listView.count > 0

            Rectangle {
                id: rect

                anchors.top: popup.top
                anchors.left: popup.left

                height: /*listView.contentHeight*/100 //TODO customize it.
                width: popup.width

                ListView {
                    id: listView
                    height: /*contentHeight*/100 //TODO customize it.
                    width: parent.width
                    interactive: true
                    clip: true
                    model: rcAccount.inputCompleterModel()
                    delegate:
                        RowLayout {
                        Text {
                            text: display
                        }
                    }
                }
            }
        }
        function showPopupCompleting() {
            if (!popup.visible) {
                popup.open()
                listView.currentIndex = -1
            }
        }
    }

    Keys.onDownPressed: {
        inputTextCompleter.completionListView.incrementCurrentIndex()
    }
    Keys.onUpPressed: {
        inputTextCompleter.completionListView.decrementCurrentIndex()
    }
}
