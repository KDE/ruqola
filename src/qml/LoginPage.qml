import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0 as QQC2

import KDE.Ruqola.Ruqola 1.0
import KDE.Ruqola.DDPClient 1.0


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
