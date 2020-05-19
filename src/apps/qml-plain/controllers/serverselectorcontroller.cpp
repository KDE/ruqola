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

#include "controllers/serverselectorcontroller.h"

#include "controllers/loginscreencontroller.h"

#include "accountmanager.h"
#include "model/rocketchataccountmodel.h"
#include "rocketchataccount.h"
#include "ruqolaserverconfig.h"

#include <QUrl>

namespace RuqolaQmlPlain {
namespace Controllers {

ServerSelectorController::ServerSelectorController(
    AccountManager *accountManager, LoginScreenController *loginScreenController, QObject *parent)
    : QObject(parent)
    , m_accountManager(accountManager)
    , m_loginScreenController(loginScreenController)
{
    connect(
        m_loginScreenController, &LoginScreenController::newServerAdded, this,
        &ServerSelectorController::countChanged);
}

void ServerSelectorController::select(int index)
{
    m_addingNewServer = false;
    m_selectedServer = index;
    // Adding a new account
    if (index == m_accountManager->rocketChatAccountModel()->rowCount({})) {
        m_loginScreenController->setLoginMode(
            LoginScreenControllerEnumContainer::LoginMode::PickAServer);
    } else {
        m_accountManager->setCurrentAccountIndex(index);
        m_loginScreenController->setLoginMode(
            LoginScreenControllerEnumContainer::LoginMode::AuthenticateOnServer);
    }
    Q_EMIT selectedServerChanged();
}

QUrl ServerSelectorController::logoUrlForServer(int idx)
{
    auto account = m_accountManager->accountByIndex(idx);
    QUrl logoUrl;

    QString urlStr = account->serverUrl();
    urlStr += QStringLiteral("/") + account->ruqolaServerConfig()->logoUrl();

    logoUrl.setUrl(QStringLiteral("https://") + urlStr);
    return logoUrl;
}

QUrl ServerSelectorController::faviconUrlForServer(int idx)
{
    auto account = m_accountManager->accountByIndex(idx);
    QUrl faviconUrl;

    QString urlStr = account->serverUrl();
    urlStr += QStringLiteral("/") + account->ruqolaServerConfig()->faviconUrl();

    faviconUrl.setUrl(QStringLiteral("https://") + urlStr);
    return faviconUrl;
}

int ServerSelectorController::count() const
{
    return m_accountManager->rocketChatAccountModel()->rowCount({});
}

int ServerSelectorController::selectedServer() const
{
    return m_selectedServer;
}

bool ServerSelectorController::addingNewServer() const
{
    return m_selectedServer == count();
}

QUrl ServerSelectorController::currentServerUrl() const
{
    QUrl url;
    url.setUrl(QStringLiteral("https://") + m_accountManager->account()->serverUrl());
    return url;
}

} // namespace Controllers
} // namespace RuqolaQmlPlain
