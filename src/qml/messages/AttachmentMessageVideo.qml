/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

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
import QtQuick 2.9

import QtQuick.Controls 2.2 as QQC2
import org.kde.kirigami 2.1 as Kirigami
import QtMultimedia 5.8
import QtQuick.Layouts 1.1
import "../js/message.js" as MessageScript;

MessageBase {
    id: attachmentVideo

    MediaPlayer {
        id: audioPlayer
        autoPlay: false
        onPaused: {
            playerButton.source = "qrc:/icons/media-playback-start.svg"
        }
        onPlaying: {
            playerButton.source = "qrc:/icons/media-playback-pause.svg"
        }
        onStopped: {
            playerButton.source = "qrc:/icons/media-playback-start.svg"
            playerSlider.value=0
        }
        onPositionChanged: {
            playerSlider.value = audioPlayer.position / audioPlayer.duration
        }
    }


    RowLayout {
        //Add video media
        Image {
            id: playerButton
            source: "qrc:/icons/media-playback-start.svg"
            width: 100
            height: 100
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("Click !");
                    if (audioPlayer.source != "") {
                        if (audioPlayer.playbackState == MediaPlayer.PlayingState) {
                            audioPlayer.pause()
                        } else {
                            audioPlayer.play()
                        }
                    } else {
                        console.log("Video file no found");
                    }
                }
            }
        }
        QQC2.Slider {
            id: playerSlider
            Layout.fillWidth: true

            onValueChanged: {
            }
        }
        Rectangle {
            Layout.alignment: Qt.AlignCenter
            width: textLabel.implicitWidth + 6*Kirigami.Units.smallSpacing
            height: textLabel.height

            color: Kirigami.Theme.disabledTextColor
            radius: 4*Kirigami.Units.smallSpacing

            QQC2.Label {
                id: textLabel

                color: Kirigami.Theme.textColor
                opacity: 1

                anchors.centerIn: parent
                anchors.leftMargin: Kirigami.Units.smallSpacing
                anchors.rightMargin: Kirigami.Units.smallSpacing

                width: Math.min(implicitWidth, parent.width - Kirigami.Units.largeSpacing)

                text: i_username + i18n(" Video Message ");

                wrapMode: QQC2.Label.Wrap

                renderType: Text.NativeRendering
            }
        }
        Item {
            Layout.fillWidth: true
        }
        QQC2.Label {
            id: timestampText

            Layout.alignment: Qt.AlignTop | Qt.AlignRight
            text: MessageScript.displayDateTime(i_timestamp)
            opacity: .5

            z: 10
        }
    }
}
