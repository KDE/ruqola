import QtQuick 2.12
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import QtMultimedia 5.12

Item {
    id: root

    implicitHeight: videoAttachmentLayout.height

    property var attachment: null

    RowLayout {
        id: videoAttachmentLayout

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
                spacing: 0

                Button {
                    Layout.preferredWidth: videoTitleText.width
                    Layout.preferredHeight: videoTitleText.height
                    background: null
                    Text {
                        id: videoTitleText
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

            Video {
                id: videoMessageContent

                Layout.preferredWidth: 480
                Layout.preferredHeight: 270
                width: 480
                height: 270

                source: _chatScreenController.attachmentUrl(root.attachment.link)
                Component.onCompleted: {
                    play();
                    pause();
                    seek(0);
                }

                onSourceChanged: {
                    play();
                    pause();
                    seek(0);
                }

                // Video overlay
                Item {
                    anchors.fill: parent
                    MouseArea {
                        id: videoMouseArea

                        anchors.fill: parent
                        z: 1600
                        hoverEnabled: true
                        onPressed: mouse.accepted = false
                        onReleased: mouse.accepted = false
                        onClicked: mouse.accepted = false

                        Rectangle {
                            anchors.fill: parent
                            visible: videoMouseArea.containsMouse || (videoMessageContent.playbackState != MediaPlayer.PlayingState)

                            color: "#11000000"
                            Button {
                                width: 68
                                height: 68

                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter

                                background: Rectangle {
                                    anchors.fill: parent
                                    color: "white"
                                    radius: 68/2
                                }

                                Image {
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    sourceSize.width: 20
                                    sourceSize.height: 20
                                    source: videoMessageContent.playbackState != MediaPlayer.PlayingState ? _iconsModelQml.playSolidIcon : _iconsModelQml.pauseIcon
                                }

                                onClicked: {
                                    if (videoMessageContent.playbackState == MediaPlayer.PlayingState) {
                                        videoMessageContent.pause();
                                    }
                                    else {
                                        videoMessageContent.play();
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
