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


import QtQuick 2.9
import QtQuick.Controls 2.5 as QQC2
import org.kde.kirigami 2.7 as Kirigami

QQC2.TextField {
    id: passwordLineEdit
    property bool displayPassword: false

    selectByMouse: true
    echoMode: displayPassword ? TextInput.Normal : TextInput.Password
    inputMethodHints: Qt.ImhHiddenText
    placeholderText: i18n("Enter Password...")

    Accessible.passwordEdit: true

    Kirigami.Icon {
        source: passwordLineEdit.displayPassword ? "visibility": "hint"
        anchors.right: parent.right
        anchors.top: parent.top
        height: parent.height
        width: parent.height

        MouseArea {
            anchors.fill: parent
            onPressed: {
                passwordLineEdit.displayPassword = true
            }
            onReleased: {
                passwordLineEdit.displayPassword = false
            }
        }
    }
}
