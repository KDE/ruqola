import QtQuick 2.12

Item {
    id: root

    property alias text: dateText.text
    property int padding: 0

    height: 25

    Rectangle {
        id: leftDateLine
        height: 1
        color: "#EAEAEA"

        anchors.left: parent.left
        anchors.leftMargin: root.padding
        anchors.right: dateTextContainer.left
        anchors.verticalCenter: parent.verticalCenter
    }

    Item {
        id: dateTextContainer
        width: 140
        height: dateText.height
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        Text {
            id: dateText
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }

    Rectangle {
        id: rightDateLine

        height: 1
        color: "#EAEAEA"

        anchors.left: dateTextContainer.right
        anchors.right: parent.right
        anchors.rightMargin: root.padding
        anchors.verticalCenter: parent.verticalCenter
    }
}