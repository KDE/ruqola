/*
   Copyright (c) 2018-2019 Montel Laurent <montel@kde.org>

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


import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5 as QQC2
import QtQuick.Window 2.2
import QtQuick 2.9
import KDE.Ruqola.FilesForRoomFilterProxyModel 1.0
import org.kde.kirigami 2.7 as Kirigami
import "common"
import "js/message.js" as MessageScript;

QQC2.Dialog {
    id: showFilesInRoomDialog

    title: i18n("Show Files In Room")
    signal downloadFile(string file)
    signal deleteFile(string fileid)

    property QtObject filesModel

    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    modal: true
    standardButtons: QQC2.Dialog.Close

    function initializeAndOpen()
    {
        searchField.text = "";
        open();
    }


    ColumnLayout {
        LineEditWithClearButton {
            id: searchField
            placeholderText: i18n("Search File...")
            Layout.fillWidth: true
            onTextChanged: {
                filesModel.setFilterString(text);
            }
        }
        QQC2.Label {
            text: listview.count === 0 ? i18n("No Attachment found") : i18np("%1 attachment in room", "%1 attachments in room", listview.count)
            Component.onCompleted: {
                font.italic = true
                font.bold = true
            }
        }

        //Add scrollview
        ListView {
            id: listview
            width: 400
            height: 200
            clip: true
            // Scrollars
            QQC2.ScrollIndicator.vertical: QQC2.ScrollIndicator { }
            QQC2.ScrollIndicator.horizontal: QQC2.ScrollIndicator { }

            model: filesModel
            delegate:
                RowLayout {
                width: ListView.view.width
                QQC2.Label {
                    text: filename + '\n' + (description !== "" ? description + '\n' : "") + username + '\n' + MessageScript.displayDateTime(timestamp)
                    wrapMode: QQC2.Label.Wrap
                }
                Item {
                    Layout.fillWidth: true
                }
                DownloadButton {
                    onDownloadButtonClicked: {
                        showFilesInRoomDialog.downloadFile(url)
                    }
                }
                DeleteButton {
                    visible: canbedeleted
                    onDeleteButtonClicked: {
                        deleteFileAttachmentDialog.fileId = fileid;
                        deleteFileAttachmentDialog.open();
                    }
                }
            }
        }
    }
    DeleteFileAttachmentDialog {
        id: deleteFileAttachmentDialog
        onDeleteAttachment: {
            showFilesInRoomDialog.deleteFile(fileId)
        }
    }
}
