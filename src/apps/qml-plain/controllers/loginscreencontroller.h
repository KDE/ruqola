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

#pragma once

#include <QObject>

class AccountManager;
class RocketChatAccount;

namespace RuqolaQmlPlain {

namespace Controllers {

class LoginScreenControllerEnumContainer : public QObject
{
    Q_OBJECT

  public:
    enum LoginMode { PickAServer, AuthenticateOnServer };
    Q_ENUM(LoginMode);

    explicit LoginScreenControllerEnumContainer(QObject *parent = nullptr)
        : QObject(parent){};
    ~LoginScreenControllerEnumContainer() override = default;
};

class LoginScreenController : public QObject
{
    Q_OBJECT

    using LoginEnum = LoginScreenControllerEnumContainer;

  public:
    Q_PROPERTY(RuqolaQmlPlain::Controllers::LoginScreenControllerEnumContainer::LoginMode loginMode
                   READ loginMode NOTIFY loginModeChanged);

  public:
    explicit LoginScreenController(AccountManager *accountManager, QObject *parent = nullptr);
    ~LoginScreenController() override = default;

    Q_INVOKABLE void addServer(const QString &serverUrl);
    Q_INVOKABLE void authenticate(const QString &user, const QString &pass);

    Q_REQUIRED_RESULT LoginEnum::LoginMode loginMode() const;
    void setLoginMode(int mode);

  Q_SIGNALS:
    void loginModeChanged();
    void newServerAdded();

  private:
    AccountManager *m_accountManager = nullptr;
    LoginEnum::LoginMode m_loginMode = LoginEnum::AuthenticateOnServer;
    QString m_tempUrl;
    RocketChatAccount *m_tempAccount = nullptr;
};

} // namespace Controllers

} // namespace RuqolaQmlPlain
