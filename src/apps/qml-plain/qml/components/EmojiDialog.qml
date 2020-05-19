import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12

Popup {
    id: root

    property var model: null

    signal emojiPicked(string unicodeDisplay, string identifier)

    implicitWidth: 365
    implicitHeight: 300
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
    modal: false
    focus: true

    contentItem: Rectangle {
        anchors.fill: parent
        color: "white"
        radius: 1
        border.color: "grey"
        border.width: 1

        ColumnLayout {
            anchors.fill: parent
            anchors.topMargin: 24
            anchors.leftMargin: 21
            anchors.rightMargin: 10
            anchors.bottomMargin: 28

            Item {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                Rectangle {
                    id: emojiSearchText

                    width: parent.width - 40
                    height: parent.height
                    border.color: "#E1E5E8"
                    border.width: 2
                    radius: 2

                    RowLayout {
                        anchors.fill: parent

                        Button {
                            Layout.leftMargin: 10
                            Layout.alignment: Qt.AlignVCenter
                            Layout.preferredWidth: 12
                            Layout.preferredHeight: 12

                            background: Rectangle {
                                anchors.fill: parent
                                color: "white"
                            }

                            Image {
                                source: _iconsModelQml.magnifierIcon
                                sourceSize.width: 12
                                sourceSize.height: 12
                            }
                        }

                        FancyTextInput {
                            Layout.leftMargin: 20
                            Layout.alignment: Qt.AlignVCenter
                            Layout.preferredHeight: implicitHeight
                            Layout.fillWidth: true
                            Layout.rightMargin: 10
                            font.pointSize: 10
                            placeholderText: "Search"
                            placeholderColor: "#CBCED1"
                        }
                    }
                }

                Rectangle {
                    id: skinToneSelector
                    anchors.left: emojiSearchText.right
                    anchors.leftMargin: 9
                    anchors.verticalCenter: parent.verticalCenter
                    width: 20
                    height: 20
                    radius: 10
                    color: "#FFCF11"
                }
            }

            ListView {
                id: emojiCategoryListView

                Layout.topMargin: 10
                Layout.preferredWidth: 320
                Layout.preferredHeight: 20 + 2 * 8 + 2
                spacing: 0
                orientation: ListView.Horizontal
                model: root.model

                ButtonGroup {
                    id: listViewButtonGroup
                }

                delegate: Button {
                    height: 20 + 2 * 8 + 2
                    width: 32
                    checkable: true
                    ButtonGroup.group: listViewButtonGroup
                    
                    contentItem: Rectangle {
                        anchors.fill: parent
                        anchors.topMargin: 8
                        anchors.bottomMargin: 8
                        anchors.leftMargin: 6
                        anchors.rightMargin: 6
                        color: "white"

                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: model.categoryEmoticon
                        }
                    }

                    background: Rectangle {
                        color: "white"
                        anchors.fill: parent
                    }

                    Rectangle {
                        height: 2
                        width: 32
                        anchors.bottom: parent.bottom
                        color: parent.checked ? "#04436A" : "#F4F4F4"
                    }

                    onClicked: emojiListView.currentIndex = model.index
                }
            }

            ScrollView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                clip: true

                ListView {
                    id: emojiListView
                    anchors.fill: parent
                    model: root.model
                    spacing: 0
                    boundsBehavior: Flickable.StopAtBounds
                    onContentYChanged: emojiCategoryListView.itemAtIndex(indexAt(0, contentY)).checked = true

                    delegate: Item {
                        id: emojiEntryDelegate

                        property var categoryModel: model.categoryModel

                        height: 8 + emojiCategoryText.height + 10 + emojiGridLayout.height + 10
                        width: parent.width
                        Text {
                            id: emojiCategoryText

                            anchors.topMargin: 8
                            anchors.left: parent.left
                            anchors.leftMargin: 4

                            text: model.categoryName
                            font.pointSize: 10
                            color: "#444444"
                        }

                        GridLayout {
                            id: emojiGridLayout
                            anchors.top: emojiCategoryText.bottom
                            anchors.topMargin: 10
                            anchors.left: parent.left
                            columnSpacing: 0
                            rowSpacing: 0

                            Repeater {
                                model: emojiEntryDelegate.categoryModel

                                delegate: Button {
                                    Layout.preferredWidth: 34
                                    Layout.preferredHeight: 34
                                    Layout.column: model.index % 9
                                    Layout.row: model.index / 9

                                    MouseArea {
                                        id: emojiButtonMouseArea
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        z: 1500
                                        onPressed: { mouse.accepted = false; }
                                        onReleased: { mouse.accepted = false; }
                                        onClicked: { mouse.accepted = false; }
                                    }

                                    background: Rectangle {
                                        anchors.fill: parent
                                        radius: 2
                                        color: emojiButtonMouseArea.containsMouse ? "#DDDDDD" : "white"
                                    }

                                    Text {
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        text: model.unicodeDisplay
                                    }

                                    onClicked: root.emojiPicked(model.unicodeDisplay, model.identifier)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}