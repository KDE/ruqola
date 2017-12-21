import QtQuick 2.9

import QtQuick.Controls 2.2 as QQC2
import org.kde.kirigami 2.1 as Kirigami
//We need QtMultimedia module for playing audio file
import QtMultimedia 5.8
import QtQuick.Layouts 1.1

import KDE.Ruqola.DebugCategory 1.0

import "../js/message.js" as MessageScript;

Image {
    id: download
    source: "qrc:/icons/cloud-download.svg"
    signal downloadButtonClicked()
    //TODO customize size
    width: 100
    height: 100
    MouseArea {
        anchors.fill: parent
        onClicked: {
            download.downloadButtonClicked();
        }
    }
}
