import QtQuick 2.0

import QtQuick.Controls 2.2
import org.kde.kirigami 2.1 as Kirigami

import QtQuick.Layouts 1.1
import KDE.Ruqola.ExtraColors 1.0

Rectangle {
    id: newDateRect
    property string date
    
    Layout.alignment: Qt.AlignCenter
    visible: date !== ""
    
    implicitHeight: date !== "" ? dateLabel.font.pixelSize : 0
    
    color: RuqolaSingleton.backgroundColor;
    Kirigami.Label {
        id: dateLabel
        text: date
        anchors.centerIn: parent
    }
}
