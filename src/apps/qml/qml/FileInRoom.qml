/*
 * Copyright (C) 2019-2020 Laurent Montel <montel@kde.org>
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


import "common"

Kirigami.BasicListItem {
    reserveSpaceForIcon: false
    reserveSpaceForLabel: false
    property string i_filename
    property string i_description
    property string i_username
    property string i_timestamp
    property string i_url
    property bool i_complete

    RowLayout {
        width: filesInRoomDelegateItem.width
        ColumnLayout {
            QQC2.Label {
                text: i_filename
                textFormat: Text.PlainText
                elide: Text.ElideRight
                wrapMode: QQC2.Label.Wrap
                Layout.fillWidth: true
            }
            QQC2.Label {
                text: i_description
                Layout.fillWidth: true
                visible: i_description !== ""
                wrapMode: QQC2.Label.Wrap
                elide: Text.ElideRight
                font.italic: true
            }
            QQC2.Label {
                text: i_username
                wrapMode: QQC2.Label.NoWrap
                elide: Text.ElideRight
                color: Kirigami.Theme.disabledTextColor
                Layout.fillWidth: true
                font.italic: true
            }
            QQC2.Label {
                text: i_timestamp
                textFormat: Text.PlainText
                wrapMode: QQC2.Label.NoWrap
                color: Kirigami.Theme.disabledTextColor
                Layout.fillWidth: true
                font.italic: true
            }
        }
        Item {
            Layout.fillWidth: true
        }
        DownloadButton {
            visible: i_complete
            onClicked: {
                showFilesInRoomDialog.downloadFile(i_url)
            }
        }
        DeleteButton {
            visible: canbedeleted
            onClicked: {
                deleteFileAttachmentDialog.fileId = fileid;
                deleteFileAttachmentDialog.open();
            }
        }
    }
}
