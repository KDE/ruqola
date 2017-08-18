import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0 as QQC2
import QtQuick.Controls.Styles 1.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import Qt.labs.settings 1.0
import QtGraphicalEffects 1.0

import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.DDPClient 1.0
//import KDE.Ruqola.Notification 1.0

// import "Log.js" as Log
// import "Data.js" as Data

import org.kde.kirigami 2.1 as Kirigami


Login {
    id: loginTab
    anchors.fill: parent
    visible: Ruqola.loginStatus != DDPClient.LoggedIn
    serverURL: Ruqola.serverURL
    username: Ruqola.userName
    onAccepted: {
        Ruqola.password = loginTab.password;
        Ruqola.userName = loginTab.username;
        Ruqola.serverURL = loginTab.serverURL;
        Ruqola.tryLogin();
    }
}
