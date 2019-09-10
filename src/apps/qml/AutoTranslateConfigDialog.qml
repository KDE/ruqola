/*
   Copyright (c) 2019 Montel Laurent <montel@kde.org>

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

import org.kde.kirigami 2.7 as Kirigami
QQC2.Dialog {
    id: autoTranslateConfigDialog
    property QtObject roomInfo

    title: i18n("Auto-Translate")

    standardButtons: QQC2.Dialog.Close

    width: parent.width * 9 / 10
    height: parent.height * 9 / 10
    anchors.centerIn: parent
    modal: true
    focus: true
    contentItem: GridLayout {
        columns: 2
        QQC2.Label {
            id: labelAutoTranslate
            text: i18n("Automatic Translation:");
        }
        QQC2.Switch {
            id: autoTranslate
            checked: roomInfo === null ? false : roomInfo.autoTranslate
            onClicked: {
                console.log("change autotranslate info");
            }
        }
        QQC2.Label {
            id: labelLAnguage
            text: i18n("Language:");
        }
        AutoTranslateLanguageCombobox {

        }
        Item {
            Layout.fillHeight: true
        }
    }
}
