import QtQuick 2.0

Item {
    Loader {
        id: mainWindow
    }

    Component.onCompleted: {
//         if (Qt.platform.os === "android")
//             mainWindow.setSource("Android.qml")
//         else
            mainWindow.setSource("Desktop.qml")
    }

}

