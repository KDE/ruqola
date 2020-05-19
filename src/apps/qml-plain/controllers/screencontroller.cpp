/*
 * Copyright 2020 Alessandro Ambrosano <alessandro.ambrosano@gmail.com>
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

#include "screencontroller.h"

#include "loginscreencontroller.h"
#include "serverselectorcontroller.h"

#include "accountmanager.h"
#include "ddpapi/ddpclient.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"

namespace RuqolaQmlPlain {

namespace Controllers {

QHash<int, QUrl> ScreenController::s_screenToUrl{
    {(int)Screen::LoginScreen, QUrl(QStringLiteral("qrc:/screen/LoginScreen.qml"))},
    {(int)Screen::ChatScreen, QUrl(QStringLiteral("qrc:/screen/ChatScreen.qml"))},
    {(int)Screen::TestScreen, QUrl(QStringLiteral("qrc:/screen/TestScreen.qml"))}};

ScreenController::ScreenController(
    AccountManager *accountManager, LoginScreenController *loginScreenController,
    ServerSelectorController *serverSelectorController, QObject *parent)
    : QObject(parent)
    , m_accountManager(accountManager)
    , m_loginScreenController(loginScreenController)
    , m_serverSelectorController(serverSelectorController)
{
    handleServerSelectionChange();

    connect(
        m_serverSelectorController, &ServerSelectorController::selectedServerChanged, this,
        &ScreenController::handleServerSelectionChange);
}

ScreenController::Screen ScreenController::currentScreen() const
{
    return m_currentScreen;
}

QUrl ScreenController::loaderUrl() const
{
    return s_screenToUrl[static_cast<int>(m_currentScreen)];
}

void ScreenController::setCurrentScreen(ScreenController::Screen s)
{
    if (m_currentScreen != s) {
        m_currentScreen = s;
        Q_EMIT currentScreenChanged();
    }
}

void ScreenController::handleServerSelectionChange()
{
    if (m_serverSelectorController->addingNewServer() || !m_accountManager->account() ||
        m_accountManager->account()->authToken().isEmpty()) {
        setCurrentScreen(Screen::LoginScreen);
    } else {
        setCurrentScreen(Screen::ChatScreen);
    }
}

} // namespace Controllers

} // namespace RuqolaQmlPlain
