/*
 
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>
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
import KDE.Ruqola.ExtraColors 1.0
import KDE.Ruqola.DebugCategory 1.0
MessageBase {
    id: messageMain

    property string i_systemMessageType

    RowLayout {
        QQC2.Label {
            id: textLabel
            color: Kirigami.Theme.disabledTextColor
            opacity: 1

            Layout.alignment: Qt.AlignLeft
            //anchors.centerIn: parent
            anchors.leftMargin: Kirigami.Units.smallSpacing
            anchors.rightMargin: Kirigami.Units.smallSpacing

            width: Math.min(implicitWidth, parent.width - Kirigami.Units.largeSpacing)

            text: i_messageText

            wrapMode: QQC2.Label.Wrap

            renderType: Text.NativeRendering
            Component.onCompleted: {
                font.italic = true
            }
        }
        DiscussionLabel {
            id: discussionLabel
            onGoToDiscussionRoom: {
                console.log("Click to go to sub room")
            }
        }
        Item {
            Layout.fillWidth: true
        }
        TimestampText {
            id: timestampText
            timestamp: i_timestamp
        }
    }
}
