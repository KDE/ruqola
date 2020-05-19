import QtQuick 2.9
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12

Button {
    id: root

    property url imageUrl: ""

    checkable: true

    background: Item {
        anchors.verticalCenter: parent.verticalCenter

        Rectangle {
            id: leftRect

            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            color: root.checked ? "white" : "#aacacf"
            width: 3
            height: 8

            NumberAnimation on height {
                id: leftRectExpand

                running: false
                from: 8
                to: 36
                duration: 50
            }

            NumberAnimation on height {
                id: leftRectShrink

                running: false
                from: 36
                to: 8
                duration: 50
            }
        }

        Rectangle {
            id: rightRect

            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            color: root.checked ? "white" : "#aacacf"
            width: 6
            radius: 3
            height: 8

            NumberAnimation on height {
                id: rightRectExpand

                running: false
                from: 8
                to: 36
                duration: 50
            }

            NumberAnimation on height {
                id: rightRectShrink

                running: false
                from: 36
                to: 8
                duration: 50
            }
        }
    }

    onCheckedChanged: {
        if (checked) {
            leftRectExpand.start();
            rightRectExpand.start();
        }
        else {
            leftRectShrink.start();
            rightRectShrink.start();
        }
    }

    Image {
        anchors.fill: parent
        anchors.leftMargin: 5
        anchors.topMargin: 5
        anchors.bottomMargin: 5

        anchors.verticalCenter: parent.verticalCenter

        fillMode: Image.PreserveAspectFit
        source: root.imageUrl
    }
}
