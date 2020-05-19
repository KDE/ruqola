import QtQuick 2.12
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import QtMultimedia 5.14

Item {
    id: root

    implicitHeight: imageAttachmentLayout.height

    property var attachment: null

    RowLayout {
        id: imageAttachmentLayout

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

            Image {
                Layout.preferredHeight: 200

                sourceSize.height: 200
                fillMode: Image.PreserveAspectFit
                source: _chatScreenController.attachmentUrl(root.attachment.link)
            }
        }
    }
}
