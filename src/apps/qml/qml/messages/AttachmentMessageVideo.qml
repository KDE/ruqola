/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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

import QtQuick.Controls 2.5 as QQC2
import org.kde.kirigami 2.7 as Kirigami
import QtMultimedia 5.8
import QtQuick.Layouts 1.12
import Ruqola 1.0
import "../js/convert.js" as ConvertScript;
import "../common"

UserMessage {
    id: attachmentVideo

    MediaPlayer {
        id: videoPlayer

        autoPlay: false

        //It doesn't work. Perhaps we need to remove it
        function preview()
        {
            if (status === MediaPlayer.Loaded && playbackState === MediaPlayer.StoppedState) {
                seek(duration/2)
            }
        }

        onStatusChanged: {
            preview();
        }
        onPlaybackStateChanged: {
            preview();
        }
    }


    attachments: Repeater {
        id: repearterAttachments

        model: i_attachments

        RowLayout {
            ColumnLayout {
                Layout.fillWidth: true
                QQC2.Label {
                    //TODO remove duplicate code
                    text: model.modelData.title === "" ? "" :  i18n("File Uploaded: %1", model.modelData.title)
                    visible: model.modelData.title !== ""
                    wrapMode: QQC2.Label.Wrap
                    anchors.leftMargin: Kirigami.Units.smallSpacing
                    anchors.rightMargin: Kirigami.Units.smallSpacing
                }
                VideoOutput {
                    id: videoOutput

                    property int videoHeight: 100
                    Layout.fillWidth: true
                    source: videoPlayer
                    width: 100
                    height: 0
                }
                RowLayout {
                    //Add video media
                    Kirigami.Icon {
                        id: playerButton

                        source: "media-playback-start"
                        width: height
                        height: Kirigami.Units.iconSizes.medium
                        MouseArea {
                            anchors.fill: parent
                            readonly property url link: rcAccount.attachmentUrl(model.modelData.link)
                            onLinkChanged: {
                                videoPlayer.source = link
                            }
                            onClicked: {
                                console.log(RuqolaDebugCategorySingleton.category, "Click on video file!");

                                if (videoPlayer.playbackState === MediaPlayer.PlayingState) {
                                    videoPlayer.pause()
                                    playerButton.source = "media-playback-start"
                                } else {
                                    videoPlayer.play()
                                    playerButton.source = "media-playback-pause"
                                }
                                //TODO stop ?
                                if (videoPlayer.error !== MediaPlayer.NoError) {
                                    console.log(RuqolaDebugCategorySingleton.category, "Video file no found");
                                }
                            }
                        }
                    }

                    QQC2.Slider {
                        id: playerSlider
                        enabled: videoPlayer.playbackState === MediaPlayer.PlayingState
                        Layout.fillWidth: true

                        from: 0
                        to: videoPlayer.duration
                        value: videoPlayer.position

                        onMoved: {
                            videoPlayer.seek(value)
                        }

                    }
                    QQC2.Label {
                        id: timeLabel
                        text: ConvertScript.convertTimeString(playerSlider.value) + "/" + ConvertScript.convertTimeString(playerSlider.to)//"00:00/00:00"
                    }

                    DownloadButton {
                        id: download
                        onDownloadButtonClicked: {
                            messageMain.downloadAttachment(videoPlayer.source)
                        }
                    }
                    ShowHideButton {
                        targetAnimation: videoOutput
                        defaultHeight: videoOutput.videoHeight
                    }
                }
                QQC2.Label {
                    text: model.modelData.description
                    visible: model.modelData.description !== ""
                    wrapMode: QQC2.Label.Wrap
                    anchors.leftMargin: Kirigami.Units.smallSpacing
                    anchors.rightMargin: Kirigami.Units.smallSpacing
                }
            }
        }
    }
}
