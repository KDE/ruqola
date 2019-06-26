/*
 * Copyright 2016  Riccardo Iaconelli <riccardo@kde.org>
 * Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License or (at your option) version 3 or any later version
 * accepted by the membership of KDE e.V. (or its successor approved
 * by the membership of KDE e.V.), which shall act as a proxy
 * defined in Section 14 of version 3 of the license.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

import QtQuick 2.9

import KDE.Ruqola.RocketChatAccount 1.0
import KDE.Ruqola.DDPClient 1.0


Login {
    id: loginTab

    anchors.fill: parent
    visible: rcAccount.loginStatus !== DDPClient.LoggedIn
    serverUrl: rcAccount.serverUrl
    username: rcAccount.userName
    originalAccountName: rcAccount.accountName
    password: rcAccount.password
    code: rcAccount.code
    onAccepted: {
        //TODO ?
        //rcAccount.updateAccountSettings(loginTab.accountName, loginTab.password, loginTab.username, loginTab.serverUrl)
        rcAccount.accountName = loginTab.accountName;
        rcAccount.password = loginTab.password;
        rcAccount.code = loginTab.code;
        rcAccount.userName = loginTab.username;
        rcAccount.serverUrl = loginTab.serverUrl;
        rcAccount.tryLogin();
    }
}
