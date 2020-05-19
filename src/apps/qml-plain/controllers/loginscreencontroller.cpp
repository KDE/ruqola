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

#include "loginscreencontroller.h"

#include "accountmanager.h"
#include "rocketchataccount.h"

#include <QUuid>

namespace RuqolaQmlPlain {

namespace Controllers {

using LoginEnum = LoginScreenControllerEnumContainer;

LoginScreenController::LoginScreenController(AccountManager *accountManager, QObject *parent)
    : QObject(parent)
    , m_accountManager(accountManager)
{
    m_loginMode =
        accountManager->account() ? LoginEnum::AuthenticateOnServer : LoginEnum::PickAServer;
}

LoginEnum::LoginMode LoginScreenController::loginMode() const
{
    return m_loginMode;
}

void LoginScreenController::setLoginMode(int mode)
{
    auto loginMode = static_cast<LoginEnum::LoginMode>(mode);
    if (loginMode != m_loginMode) {
        m_loginMode = loginMode;
        Q_EMIT loginModeChanged();
    }
}

void LoginScreenController::addServer(const QString &serverUrl)
{
    m_accountManager->addAccount(serverUrl, QString(), serverUrl, true);
    m_accountManager->setCurrentAccount(serverUrl);
    setLoginMode(LoginEnum::AuthenticateOnServer);
    Q_EMIT newServerAdded();
}

void LoginScreenController::authenticate(const QString &user, const QString &pass)
{
    m_accountManager->account()->setUserName(user);
    m_accountManager->account()->setPassword(pass);
    m_accountManager->account()->tryLogin();
}

} // namespace Controllers

} // namespace RuqolaQmlPlain
