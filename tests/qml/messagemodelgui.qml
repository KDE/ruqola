import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import KDE.Ruqola.RoomFilterProxyModel 1.0
import org.kde.kirigami 2.1 as Kirigami
Kirigami.ApplicationWindow {
    visible: true
    title: "Hello World"

    pageStack.initialPage: [roomsComponent]

    Component {
        id: roomsComponent
        Kirigami.ScrollablePage {
            Column {
                id: form

                anchors.centerIn: parent
                width: 0.8*parent.width
                spacing: 3
                Text {
                    id: loginLabel

                    text: i18n("Ruqola Log in")
                    color: "#555"
                    font.pixelSize: 40
                    horizontalAlignment: Text.AlignHCenter
                    width: parent.width
                }

                Item {
                    id: spacer

                    width: 30
                    height: 30
                }
            }
        }
    }
}
