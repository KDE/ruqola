/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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
        id: videoPlayer

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
            playerSlider.sync = true
            playerSlider.value = videoPlayer.position / videoPlayer.duration
            playerSlider.sync = false
            timeLabel.text = ConvertScript.convertTimeString(videoPlayer.position) + "/" + ConvertScript.convertTimeString(videoPlayer.duration)
        }
    }


    RowLayout {
        AvatarImage {
            id: avatarRect
            avatarurl: i_avatar
            aliasname: i_aliasname
            username: i_username
        }
        //Add video media
        Image {
            id: playerButton

            source: "qrc:/icons/media-playback-start.svg"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log(RuqolaDebugCategorySingleton.category, "Click on video file!");
                    if (videoPlayer.source !== "") {
                        if (videoPlayer.playbackState === MediaPlayer.PlayingState) {
                            videoPlayer.pause()
                        } else {
                            videoPlayer.play()
                        }
                    } else {
                        console.log(RuqolaDebugCategorySingleton.category, "Video file no found");
                    }
                }
            }
        }
        //Verify position.
        //Perhaps hidding it by default.
        VideoOutput {
            id: videoOutput

            Layout.fillWidth: true
            source: videoPlayer
        }

        QQC2.Slider {
            id: playerSlider

            Layout.fillWidth: true

            property bool sync: false

            onValueChanged: {
                if (!sync) {
                    videoPlayer.seek(value * videoPlayer.duration)
                }
            }
        }
        QQC2.Label {
            id: timeLabel
            text: "00:00/00:00"
        }

        DownloadButton {
            id: download
            onDownloadButtonClicked: {
                //TODO messageMain.downloadAttachment(model.modelData.link)
            }
        }
        TimestampText {
            id: timestampText
            timestamp: i_timestamp
        }
    }
}
