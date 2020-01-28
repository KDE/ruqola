/*
   Copyright (c) 2018-2020 Laurent Montel <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

import QtQuick 2.9
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Window 2.2
import Ruqola 1.0


import org.kde.kirigami 2.7 as Kirigami

QQC2.Dialog {
    id: searchChannelDialog

    property QtObject searchChannelModel
    signal searchChannel(string pattern)
    signal openChannel(string channelname, string channelid, var channeltype)

    title: i18n("Search Channel")
    standardButtons: QQC2.Dialog.Close

    width: parent.width * 9 / 10
    height: parent.height * 9 / 10
    anchors.centerIn: parent

    modal: true
    focus: true
    function initializeAndOpen()
    {
        channelnametext.text = "";
        channelnametext.forceActiveFocus()
        open();
    }

    contentItem: ColumnLayout {
        LineEditWithClearButton {
            id: channelnametext
            placeholderText: i18n("Search Channel...")
            Layout.fillWidth: true
            onAccepted: {
                searchChannelDialog.searchChannel(channelnametext.text)
            }
        }
        QQC2.Label {
            text: listview.count === 0 ? i18n("No Channel found") : ""
            textFormat: Text.PlainText
            font.italic: true
            font.bold: true
        }
        ListView {
            id: listview
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true

            model: searchChannelModel
            delegate: Kirigami.BasicListItem {
                reserveSpaceForIcon: false
                RowLayout {
                    Layout.fillHeight: true
                    Kirigami.Icon {
                        source: "list-add"
                        //FIXME
                        height: Kirigami.Units.iconSizes.medium
                        width: height
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                searchChannelDialog.openChannel(channelname, channelid, channeltype)
                            }
                        }
                    }
                    Kirigami.Icon {
                        source: iconname
                        height: Kirigami.Units.iconSizes.medium
                        width: height
                    }
                    QQC2.Label {
                        Layout.fillWidth: true
                        text: channelname
                    }
                }
            }
        }
    }
}
