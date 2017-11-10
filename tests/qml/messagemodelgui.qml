import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import KDE.Ruqola.RoomFilterProxyModel 1.0
import org.kde.kirigami 2.1 as Kirigami

Kirigami.ApplicationWindow {
    pageStack.initialPage: [roomsComponent]

    Component {
        id: roomsComponent
        Kirigami.ScrollablePage {
            id: mainWidget
            leftPadding: Kirigami.Units.smallSpacing
            rightPadding: Kirigami.Units.smallSpacing
            topPadding: Kirigami.Units.smallSpacing
            bottomPadding: Kirigami.Units.smallSpacing
            ActiveChat {
                id: activeChat
                model: messageModel
            }
        }
    }
}
