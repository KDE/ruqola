import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12

Popup {
    id: root

    property bool canDeleteMessage: true

    // Used to hold temporarily the message information needed to perform
    // any action on the popup.
    // TODO: move in a C++ controller class
    property var messageId: ""
    property var roomId: ""

    signal reportClicked()
    signal removeClicked()

    implicitWidth: 165
    implicitHeight: buttonLayout.height

    height: buttonLayout.height

    ColumnLayout {
        id: buttonLayout

        width: parent.width
        spacing: 0.0

        Repeater {
            model: [
                {
                    icon: _iconsModelQml.addReactionIcon,
                    text: "Add Reaction"
                },
                {
                    icon: _iconsModelQml.threadIcon,
                    text: "Reply in Thread"
                },
                {
                    icon: _iconsModelQml.chatIcon,
                    text: "Start a Discussion"
                },
                {
                    icon: _iconsModelQml.bellIcon,
                    text: "Follow Message"
                },
                {
                    icon: _iconsModelQml.quoteIcon,
                    text: "Quote"
                },
                {
                    icon: _iconsModelQml.shareIcon,
                    text: "Get Link"
                },
                {
                    icon: _iconsModelQml.copyIcon,
                    text: "Copy"
                },
                {
                    icon: _iconsModelQml.starIcon,
                    text: "Star"
                },
                {
                    icon: _iconsModelQml.jumpIcon,
                    text: "Jump to Message"
                }
            ]

            Button {
                Layout.preferredHeight: 34
                Layout.fillWidth: true

                Image {
                    id: buttonIcon
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    source: modelData.icon
                    sourceSize.width: 16
                    sourceSize.height: 16
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: buttonIcon.right
                    anchors.leftMargin: 8
                    text: modelData.text
                }

                background: Rectangle {
                    anchors.fill: parent
                    color: "white"
                }
            }
        }

        Item {
            Layout.preferredHeight: 34
            Layout.fillWidth: true

            Rectangle {
                width: parent.width
                height: 2
                anchors.verticalCenter: parent.verticalCenter
                color: "#E1E5E8"
            }
        }

        Repeater {
            model: [
                {
                    icon: _iconsModelQml.warningIcon,
                    text: "Report",
                    visible: true,
                    actionTag: 0
                },
                {
                    icon: _iconsModelQml.trashIcon,
                    text: "Delete",
                    visible: canDeleteMessage,
                    actionTag: 1
                }
            ]

            Button {
                Layout.preferredHeight: 34
                Layout.fillWidth: true
                visible: modelData.visible

                Image {
                    id: buttonIcon
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    source: modelData.icon
                    sourceSize.width: 16
                    sourceSize.height: 16
                }

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: buttonIcon.right
                    anchors.leftMargin: 8
                    text: modelData.text
                }

                background: Rectangle {
                    anchors.fill: parent
                    color: "white"
                }

                onClicked: {
                    if (modelData.actionTag === 0) {
                        root.reportClicked();
                    }
                    else if (modelData.actionTag === 1) {
                        root.removeClicked();
                    }
                }
            }
        }        

        Item {
            Layout.preferredHeight: 24
        }
    }
}
