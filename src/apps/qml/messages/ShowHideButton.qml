/*
 * Copyright (C) 2018 Laurent Montel <montel@kde.org>
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

import QtQuick.Controls 2.2 as QQC2
import org.kde.kirigami 2.4 as Kirigami

import QtQuick.Layouts 1.1

Kirigami.Icon {
    property QtObject targetAnimation
    property int defaultHeight: 200
    property bool isHidden: true

    source: isHidden ? "hint" : "visibility"
    width: 24
    height: 24
    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (isHidden) {
                isHidden = false
                imageAnimationShow.running = true;
            } else {
                isHidden = true
                imageAnimationHide.running = true;
            }
        }
    }
    NumberAnimation {
        id: imageAnimationHide
        target: targetAnimation
        property: "height"
        duration: 500
        easing.type: Easing.InOutQuad
        from: defaultHeight
        to: 0
    }
    NumberAnimation {
        id: imageAnimationShow
        target: targetAnimation
        property: "height"
        duration: 500
        easing.type: Easing.InOutQuad
        from: 0
        to: defaultHeight
    }
}
