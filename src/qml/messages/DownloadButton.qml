import QtQuick 2.9

import QtQuick.Layouts 1.1

Image {
    id: download
    source: "qrc:/icons/cloud-download.svg"
    signal downloadButtonClicked()

    width: 24
    height: 24
    MouseArea {
        anchors.fill: parent
        onClicked: {
            download.downloadButtonClicked();
        }
    }
}
