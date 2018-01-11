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

import QtQuick.Controls 2.2 as QQC2
import org.kde.kirigami 2.1 as Kirigami
import QtMultimedia 5.8
import QtQuick.Layouts 1.1
import "../js/message.js" as MessageScript;
import "../js/convert.js" as ConvertScript;

import KDE.Ruqola.DebugCategory 1.0

MessageBase {
    id: attachmentVideo



    RowLayout {
        AvatarImage {
            id: avatarRect
            avatarurl: i_avatar
            aliasname: i_aliasname
            username: i_username
        }

        Repeater {
            id: repearterAttachments

            model: i_attachments

            MediaPlayer {
                id: videoPlayer

                autoPlay: false
                onPaused: {
                    playerButton.source = "media-playback-start"
                }
                onPlaying: {
                    playerButton.source = "media-playback-pause"
                }
                onStopped: {
                    playerButton.source = "media-playback-start"
                    playerSlider.value=0
                }
                onPositionChanged: {
                    playerSlider.sync = true
                    playerSlider.value = videoPlayer.position / videoPlayer.duration
                    playerSlider.sync = false
                    timeLabel.text = ConvertScript.convertTimeString(videoPlayer.position) + "/" + ConvertScript.convertTimeString(videoPlayer.duration)
                }
                source: rcAccount.attachmentUrl(model.modelData.link)
            }
            RowLayout {
                //Perhaps hidding it by default.
                ColumnLayout {
                    Layout.fillWidth: true
                    Text {
                        text: i18n("File send: %1", model.modelData.title)
                        wrapMode: QQC2.Label.Wrap
                        anchors.leftMargin: Kirigami.Units.smallSpacing
                        anchors.rightMargin: Kirigami.Units.smallSpacing
                    }
                    VideoOutput {
                        id: videoOutput

                        Layout.fillWidth: true
                        source: repearterAttachments.videoPlayer
                        width: 100
                        height: 100
                    }
                    RowLayout {
                        //Verify position.
                        //Add video media
                        Kirigami.Icon {
                            id: playerButton

                            source: "media-playback-start"
                            width: 24
                            height: 24
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    console.log(RuqolaDebugCategorySingleton.category, "Click on video file!");
                                    if (repearterAttachments.videoPlayer.source !== "") {
                                        if (repearterAttachments.videoPlayer.playbackState === MediaPlayer.PlayingState) {
                                            repearterAttachments.videoPlayer.pause()
                                        } else {
                                            repearterAttachments.videoPlayer.play()
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
                    }
                    Text {
                        text: model.modelData.description
                        wrapMode: QQC2.Label.Wrap
                        anchors.leftMargin: Kirigami.Units.smallSpacing
                        anchors.rightMargin: Kirigami.Units.smallSpacing
                    }
                }
            }
        }
        TimestampText {
            id: timestampText
            timestamp: i_timestamp
        }
    }
}
