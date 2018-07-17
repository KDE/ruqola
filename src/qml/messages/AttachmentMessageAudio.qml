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
import QtMultimedia 5.8

import QtQuick.Controls 2.2 as QQC2
import org.kde.kirigami 2.1 as Kirigami
import QtQuick.Layouts 1.1

import KDE.Ruqola.DebugCategory 1.0

import "../js/convert.js" as ConvertScript;
import "../common"

MessageBase {
    id: attachmentAudio


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
                id: audioPlayer
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
                    playerSlider.value = audioPlayer.position / audioPlayer.duration
                    playerSlider.sync = false
                    timeLabel.text = ConvertScript.convertTimeString(audioPlayer.position) + "/" + ConvertScript.convertTimeString(audioPlayer.duration)
                }
                source: rcAccount.attachmentUrl(model.modelData.link)
            }


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

                RowLayout {
                    Kirigami.Icon {
                        id: playerButton
                        source: "media-playback-start"
                        width: 24
                        height: 24
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                console.log(RuqolaDebugCategorySingleton.category, "Click on download audio file");
                                if (repearterAttachments.audioPlayer.source !== "") {
                                    if (repearterAttachments.audioPlayer.playbackState === MediaPlayer.PlayingState) {
                                        repearterAttachments.audioPlayer.pause()
                                    } else {
                                        repearterAttachments.audioPlayer.play()
                                    }
                                } else {
                                    console.log(RuqolaDebugCategorySingleton.category, "Audio file no found");
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
                                audioPlayer.seek(value * audioPlayer.duration)
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
                QQC2.Label {
                    text: model.modelData.description
                    visible: model.modelData.description !== ""
                    wrapMode: QQC2.Label.Wrap
                    anchors.leftMargin: Kirigami.Units.smallSpacing
                    anchors.rightMargin: Kirigami.Units.smallSpacing
                }
            }
        }
        Repeater {
            id: repearterReactions
            model: i_reactions
            Row {
                QQC2.Label {
                    id: numberOfReact
                    renderType: Text.NativeRendering
                    textFormat: Text.RichText
                    text: model.modelData.convertedReactionName
                    wrapMode: QQC2.Label.Wrap
                    anchors.leftMargin: Kirigami.Units.smallSpacing
                    anchors.rightMargin: Kirigami.Units.smallSpacing
                }
            }
        }

        TimestampText {
            id: timestampText
            timestamp: i_timestamp
        }
    }
}
