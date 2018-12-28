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
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Window 2.0

QQC2.Dialog {
    id: displayImageDialog

    title: i18n("Image")

    property url iUrl
    property bool isAnimatedImage: false
    width: parent.width
    height: parent.height

    modal: true
    focus: true
    standardButtons: QQC2.Dialog.Close

    function clearScaleAndOpen()
    {
        image.scale = 1.0
        open();
    }

    Flickable {
        clip: true
        anchors.fill: parent
        contentWidth: image.width
        contentHeight: image.height

        // Scrollars
        QQC2.ScrollIndicator.vertical: QQC2.ScrollIndicator { }
        QQC2.ScrollIndicator.horizontal: QQC2.ScrollIndicator { }

        Image {
            id: image
            visible: !isAnimatedImage
            source: iUrl
            MouseArea {
                anchors.fill: parent
                onWheel: {
                    if (wheel.modifiers & Qt.ControlModifier) {
                        var scaleBefore = image.scale;
                        var delta = wheel.angleDelta.y;
                        if ((image.scale > 0.1 && delta < 0) || (image.scale < 5 && delta > 0)) {
                            image.scale += image.scale * delta / 120 / 10;
                        }
                    }
                }
            }
        }
        AnimatedImage {
            id: imageAnimated
            visible: isAnimatedImage
            source: iUrl
            MouseArea {
                anchors.fill: parent
                onWheel: {
                    if (wheel.modifiers & Qt.ControlModifier) {
                        var scaleBefore = imageAnimated.scale;
                        var delta = wheel.angleDelta.y;
                        if ((imageAnimated.scale > 0.1 && delta < 0) || (imageAnimated.scale < 5 && delta > 0)) {
                            imageAnimated.scale += imageAnimated.scale * delta / 120 / 10;
                        }
                    }
                }
            }
        }
    }
}
