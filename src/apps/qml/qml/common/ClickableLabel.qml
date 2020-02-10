/*
   Copyright (c) 2020 Kevin Funk <kfunk@kde.org>

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

/**
 * A rich text label that is suitable for use on Desktop
 *
 * Features:
 * - Modifies the cursor shape when a link in the label is hovered
 * - Handles right mouse button pressed and emits a contextMenuRequested signal
 */
QQC2.Label {
    id: root

    property alias mouseArea: mouseArea

    signal contextMenuRequested(var mouse)

    textFormat: Text.RichText

    MouseArea {
        id: mouseArea

        anchors.fill: parent

        acceptedButtons: Qt.RightButton
        cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor

        onClicked: {
            if (mouse.button === Qt.RightButton) {
                root.contextMenuRequested(mouse)
            }
        }
    }
}
