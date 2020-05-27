import QtQuick 2.12
import QtQuick.Controls 2.9
import QtQuick.Layouts 1.12
import QtMultimedia 5.12

Item {
    id: root

    implicitHeight: urlPreviewLayout.height

    property var messageUrl: null

    RowLayout {
        id: urlPreviewLayout

        spacing: 0

        Rectangle {
            Layout.preferredWidth: 2
            Layout.fillHeight: true

            color: "#CFD0CE"
        }

        ColumnLayout {
            Layout.leftMargin: 13
            Layout.fillWidth: true
            spacing: 0

            // ColumnLayout {
            Text {
                Layout.topMargin: 15
                Layout.fillWidth: true

                color: "#AAAAAA"
                text: messageUrl.url
                font.pointSize: 10
            }

            Text {
                Layout.topMargin: 2
                Layout.fillWidth: true

                color: "#13679A"
                text: messageUrl.pageTitle
                font.pointSize: 10
            }

            Text {
                Layout.topMargin: 2
                Layout.fillWidth: true

                color: "#444444"
                text: messageUrl.description
                wrapMode: Text.WordWrap
                font.pointSize: 10
            }
            // }
        }
    }
}
