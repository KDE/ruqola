/*
 * Copyright 2017 Laurent Montel <montel@kde.org>
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


import QtQuick 2.0

import QtQuick.Controls 2.2
import org.kde.kirigami 2.1 as Kirigami

import QtQuick.Layouts 1.1
import KDE.Ruqola.ExtraColors 1.0

ColumnLayout {
    id: messageMain

    property string i_messageText
    property string i_username
    property string i_aliasname
    property string i_systemMessageType
    property var i_timestamp
    property string i_date

    signal linkActivated(string link)

    NewDateLabel {
        id: newDateRect
        date: i_date
    }

    Rectangle {
        Layout.alignment: Qt.AlignCenter
        width: textLabel.implicitWidth + 6*Kirigami.Units.smallSpacing
        height: textLabel.height

        color: Kirigami.Theme.disabledTextColor
        radius: 4*Kirigami.Units.smallSpacing

        Kirigami.Label {
            id: textLabel

            color: Kirigami.Theme.textColor
            opacity: 1

            anchors.centerIn: parent
            anchors.leftMargin: Kirigami.Units.smallSpacing
            anchors.rightMargin: Kirigami.Units.smallSpacing

            width: Math.min(implicitWidth, parent.width - Kirigami.Units.largeSpacing)

            text: i18n("Click for video")

            wrapMode: Label.Wrap

            renderType: Text.NativeRendering

            onLinkActivated: messageMain.linkActivated(link)
        }
    }
}
