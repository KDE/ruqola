import QtQuick 2.12
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import QtMultimedia 5.12

Item {
    id: root

    implicitHeight: audioAttachmentLayout.height

    property var attachment: null

    RowLayout {
        id: audioAttachmentLayout

        spacing: 0

        Rectangle {
            Layout.preferredWidth: 2
            Layout.fillHeight: true

            color: "#A0A0A0"
        }

        ColumnLayout {
            Layout.leftMargin: 13
            Layout.fillWidth: true
            spacing: 1


            RowLayout {
                // Layout.preferredHeight: 15

                spacing: 0

                Button {
                    Layout.preferredWidth: audioTitleText.width
                    Layout.preferredHeight: audioTitleText.height
                    background: null
                    Text {
                        id: audioTitleText
                        color: "#13679A"
                        text: root.attachment.title
                        font.pointSize: 12
                    }
                }

                Button {
                    Layout.preferredWidth: 16
                    Layout.preferredHeight: 16
                    Layout.leftMargin: 10

                    background: null
                    Image {
                        sourceSize.width: 16
                        sourceSize.height: 16
                        source: _iconsModelQml.downloadIcon
                    }
                }

                Button {
                    Layout.leftMargin: 5
                    Layout.preferredWidth: 16
                    Layout.preferredHeight: 16

                    background: null
                    Image {
                        sourceSize.width: 16
                        sourceSize.height: 16
                        source: _iconsModelQml.arrowDownIcon
                    }
                }
            }

            Rectangle {
                Layout.preferredWidth: 300
                Layout.preferredHeight: 54

                color: "#F1F3F4"
                radius: 26

                RowLayout {
                    anchors.fill: parent
                    spacing: 0

                    Button {
                        id: audioLayoutPlayButton

                        Layout.alignment: Qt.AlignVCenter
                        Layout.leftMargin: 22
                        Layout.preferredHeight: 14
                        Layout.preferredWidth: 14

                        background: null
                        checkable: false
                        checked: audioContent.playbackState == Audio.PlayingState

                        Image {
                            sourceSize.width: 14
                            sourceSize.height: 14
                            source: parent.checked ? _iconsModelQml.pauseIcon : _iconsModelQml.playSolidIcon
                        }

                        onClicked: {
                            if (audioContent.playbackState == Audio.PlayingState) {
                                audioContent.pause();
                            } else {
                                audioContent.play();
                            }
                        }
                    }

                    Text {
                        id: audioProgressText

                        Layout.leftMargin: 14
                        font.pointSize: 8

                        function updateText() {
                            var duration = Math.floor(audioContent.duration / 1000);
                            var currTime = Math.floor(audioContent.position / 1000);
                            var ret = "";
                            ret += Math.floor(currTime / 60);
                            ret += ":";
                            var currSec = currTime % 60;
                            ret += (currSec < 10) ? ("0" + currSec) : (currSec);
                            ret += " / " + Math.floor(duration / 60);
                            var durSec = duration % 60;
                            ret += ":";
                            ret += (durSec < 10) ? ("0" + durSec) : (durSec);
                            text = ret;
                        }

                        Connections {
                            target: audioContent
                            onDurationChanged: audioProgressText.updateText()
                            onPositionChanged: audioProgressText.updateText()
                        }

                        Component.onCompleted: updateText()
                    }

                    Item {
                        Layout.fillWidth: true

                        Audio {
                            id: audioContent
                            source: _chatScreenController.attachmentUrl(root.attachment.link)
                            notifyInterval: 100
                        }
                    }
                }
            }
        }
    }
}