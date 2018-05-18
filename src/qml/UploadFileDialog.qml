/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Window 2.0
import QtQuick 2.9
import org.kde.kirigami 2.1 as Kirigami
import QtQuick.Dialogs 1.2

QQC2.Dialog {
    id: uploadFileDialog

    title: i18n("Upload File")

    property string rId: ""
    x: parent.width / 2 - width / 2
    y: parent.height / 2 - height / 2

    modal: true

    signal uploadFile(string description, string messageText, url filename)

    standardButtons: QQC2.Dialog.Ok | QQC2.Dialog.Cancel

    function initializeAndOpen()
    {
        description.text = "";
        message.text = "";
        selectedFileNameField.text = "";
        open();
    }

    ColumnLayout {
        QQC2.TextField {
            id: description
            selectByMouse: true
            placeholderText: i18n("Description")
        }

        QQC2.TextField {
            id: message
            selectByMouse: true
            placeholderText: i18n("Message")
        }

        RowLayout {
            QQC2.TextField {
                id: selectedFileNameField
                selectByMouse: true
                placeholderText: i18n("Select File...")
            }

            Kirigami.Icon {
                source: "folder"
                width: 24
                height: 24
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        selectFileDialog.open()
                    }
                }
            }
        }
        FileDialog {
            id: selectFileDialog
            title: i18n("Please choose a file")
            folder: shortcuts.documents
            selectFolder: false
            selectMultiple: false
            selectExisting: true
            onAccepted: {
                selectedFileNameField.text = fileUrl
            }
        }
    }
    onAccepted: {
        if (selectedFileNameField.text != "") {
            uploadFileDialog.uploadFile(description.text, message.text, selectedFileNameField.text)
        }
    }
}
