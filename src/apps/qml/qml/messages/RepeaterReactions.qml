/*
 * Copyright (C) 2018-2020 Laurent Montel <montel@kde.org>
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
Repeater {
    id: repearterReactions
    signal deleteReaction(string emoji)
    Rectangle {
        radius: 5
        width: row.width + 2 * Kirigami.Units.smallSpacing
        height: row.height + 2 * Kirigami.Units.smallSpacing
        border.color: Kirigami.Theme.linkBackgroundColor

        RowLayout {
            id: row
            anchors {
                centerIn: parent
                margins: Kirigami.Units.smallSpacing
            }
            spacing: Kirigami.Units.smallSpacing

            AnimatedImage {
                id: imageAnimated
                visible: model.modelData.isAnimatedImage
                source: model.modelData.isAnimatedImage ? model.modelData.convertedReactionName : ""
                //Verify it
                Layout.preferredWidth: 20
                Layout.preferredHeight: height
                asynchronous: true
            }
            QQC2.Label {
                id: reactionsType
                visible: !model.modelData.isAnimatedImage
                textFormat: Text.RichText
                text: model.modelData.convertedReactionName
                wrapMode: QQC2.Label.NoWrap
                font.pixelSize: 8
            }
            QQC2.Label {
                id: count
                Layout.fillHeight: true
                text: model.modelData.count
                visible: model.modelData.count  > 0
                wrapMode: QQC2.Label.NoWrap
                font.italic: true
                font.pixelSize: 9
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            acceptedButtons: Qt.RightButton | Qt.LeftButton
            cursorShape: containsMouse ? Qt.PointingHandCursor : Qt.ArrowCursor
            hoverEnabled: true
            onClicked: {
                repearterReactions.deleteReaction(model.modelData.reactionName);
            }
        }

        QQC2.ToolTip.visible: mouseArea.containsMouse
        QQC2.ToolTip.text: model.modelData.convertedUsersNameAtToolTip
    }
}
