
import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.2 as QQC2

//import KDE.Ruqola.Ruqola 1.0
import org.kde.kirigami 2.1 as Kirigami

ApplicationWindow {
    id: window

    width: 600
    height: 800
    visible: true
    title: "About Data Test"

    Label {
        text: "click for show aboutdata"
        anchors.fill: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        MouseArea {
            anchors.fill: parent
            onClicked: {
                aboutDataDialog.open();
            }
        }
    }


    AboutDialog {
        id: aboutDataDialog
    }

    QQC2.Menu {
        id: contactMenu
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        modal: true

        MenuItem {
            text: "AboutData"
            onTriggered: {
            }
        }
    }
}
