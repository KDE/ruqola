/*
 * Copyright (C) 2019 Laurent Montel <montel@kde.org>
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
import org.kde.kirigami 2.7 as Kirigami
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Layouts 1.12
import Ruqola 1.0


Kirigami.BasicListItem {
    reserveSpaceForIcon: false
    reserveSpaceForLabel: false
    property int i_numberofmessages
    property string i_lastmessage
    property string i_discussionid
    property string i_description
    property string i_timestamp

    signal openDiscussion(string messageDiscussionId)

    ColumnLayout {
        QQC2.Label {
            id: timestamp
            Layout.alignment: Qt.AlignTop
            text: i_timestamp
            opacity: .5
        }

        QQC2.Label {
            text: i_description
            elide: Text.ElideRight
            wrapMode: QQC2.Label.Wrap
            Component.onCompleted: {
                font.italic = true
            }
        }
        RowLayout {
            QQC2.Label {
                text: i18np("1 message", "%1 messages", i_numberofmessages)
                elide: Text.ElideRight
                textFormat: Text.PlainText
                wrapMode: QQC2.Label.Wrap
                Component.onCompleted: {
                    font.bold = true
                }
            }
            QQC2.Label {
                id: lastMessageText
                visible: i_numberofmessages > 0
                Layout.alignment: Qt.AlignTop | Qt.AlignRight
                text: i_lastmessage
                opacity: .5
            }
        }
        QQC2.Label {
            text: i18n("Open Discussion")
            elide: Text.ElideRight
            wrapMode: QQC2.Label.Wrap
            textFormat: Text.PlainText
            color: Kirigami.Theme.negativeTextColor
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    showDiscussionsInRoomDialog.openDiscussion(i_discussionid)
                }
            }
        }
    }
}
