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
import QtQuick.Layouts 1.12


import org.kde.kirigami 2.7 as Kirigami
import "../common"

Kirigami.Icon {
    id: reactIcon

    property bool showIcon: true

    signal insertReaction(string emoji)
    source: "face-smile"
    width: height
    height: 18
    opacity: {
        if (!showIcon) {
            return 0.0;
        }
        else {
            return mouseArea.containsMouse ? 1.0 : 0.3;
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
        onClicked: {
            if (emoticonMenuLoader.active)
                emoticonMenuLoader.active = false
            else
                emoticonMenuLoader.active = true
        }
    }
    Loader {
        id: emoticonMenuLoader
        active: false
        sourceComponent :EmoticonMenu {
            id: emoticonMenu
            emojiPopupModel: appid.emojiModel
            width: Kirigami.Units.gridUnit * 20
            height: Kirigami.Units.gridUnit * 15
            x: -width + parent.width
            y: -height - 10
            Component.onCompleted: {
                open()
            }
            onInsertEmoticon: {
                reactIcon.insertReaction(emoti)
            }
        }
    }
}
