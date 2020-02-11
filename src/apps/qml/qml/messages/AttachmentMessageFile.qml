/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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

import QtQuick.Controls 2.5 as QQC2
import QtQuick.Layouts 1.12
import org.kde.kirigami 2.7 as Kirigami

import Ruqola 1.0
import "../common"

UserMessage {
    id: root

    attachments: Repeater {
        id: attachmentsRepeater

        model: i_attachments
        ColumnLayout {
            id: column

            QQC2.Label {
                visible: model.modelData.authorName !== ""
                Layout.fillWidth: true
                text: model.modelData.authorName
                wrapMode: QQC2.Label.Wrap
                anchors.leftMargin: Kirigami.Units.smallSpacing
                anchors.rightMargin: Kirigami.Units.smallSpacing
            }
            RowLayout {
                Layout.fillWidth: true

                ClickableLabel {
                    id: attachmentTitle
                    visible: model.modelData.title !== ""
                    text: model.modelData.displayTitle
                    Layout.fillWidth: true
                    wrapMode: QQC2.Label.Wrap
                    anchors.leftMargin: Kirigami.Units.smallSpacing
                    anchors.rightMargin: Kirigami.Units.smallSpacing
                    onLinkActivated: {
                        messageMain.linkActivated(link)
                    }
                }
                DownloadButton {
                    id: downloadButton
                    visible: model.modelData.canDownloadAttachment
                    onClicked: {
                        messageMain.downloadAttachment(model.modelData.link)
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
            }
            QQC2.Label {
                visible: model.modelData.description !== ""
                Layout.fillWidth: true
                text: model.modelData.description
                wrapMode: QQC2.Label.Wrap
                anchors.leftMargin: Kirigami.Units.smallSpacing
                anchors.rightMargin: Kirigami.Units.smallSpacing
            }
        }
    }
}
