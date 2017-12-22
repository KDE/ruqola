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
import "../js/convert.js" as ConvertScript;

import KDE.Ruqola.DebugCategory 1.0

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
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("Click on video file!");
                    if (audioPlayer.source !== "") {
                        if (audioPlayer.playbackState === MediaPlayer.PlayingState) {
                            audioPlayer.pause()
                        } else {
                            audioPlayer.play()
                        }
                    } else {
                        console.log(RuqolaDebugCategorySingleton.category, "Video file no found");
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
        DownloadButton {
            id: download
            onDownloadButtonClicked: {
                //TODO messageMain.downloadAttachment(model.modelData.link)
            }
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
