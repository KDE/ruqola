import QtQuick 2.9

import QtQuick.Controls 2.2
import org.kde.kirigami 2.1 as Kirigami
import QtQuick.Controls 2.2 as QQC2
import QtQuick.Layouts 1.1
import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.RuqolaUtils 1.0
import KDE.Ruqola.ExtraColors 1.0
import "../js/message.js" as MessageScript;

Rectangle {
    id: avatarRect
    
    Layout.alignment: Qt.AlignTop
    Layout.fillHeight: false
    implicitWidth: textLabel.font.pixelSize * 3
    implicitHeight: textLabel.font.pixelSize * 3
    
    radius: 3
    
    anchors.rightMargin: 2*Kirigami.Units.smallSpacing
    
    color: i_avatar !== "" ? "transparent" : MessageScript.stringToColour(i_username)
    Image {
        id: avatarImage
        
        anchors.fill: parent
        visible: i_avatar !== ""
        source: i_avatar
        fillMode: Image.PreserveAspectFit
    }
    Text {
        id: avatarText
        
        visible: i_avatar == ""
        anchors.fill: parent
        anchors.margins: Kirigami.Units.smallSpacing
        
        renderType: Text.QtRendering
        color: "white"
        
        font.weight: Font.Bold
        font.pointSize: 100
        fontSizeMode: Text.Fit
        
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        
        text: {
            //TODO verify if it works with non latin char.
            var match = i_aliasname.match(/([a-zA-Z])([a-zA-Z])/);
            var abbrev = match[1].toUpperCase();
            if (match.length > 2) {
                abbrev += match[2].toLowerCase();
            }
            return abbrev;
        }
    }
}
