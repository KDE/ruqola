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

#include <QHash>
#include <QObject>
#include <QString>
#include <QUrl>


class AccountManager;

namespace RuqolaQmlPlain {
namespace Controllers {
class LoginScreenController;

class ServerSelectorController : public QObject
{
    Q_OBJECT

  public:
    Q_PROPERTY(int selectedServer READ selectedServer NOTIFY selectedServerChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QUrl currentServerUrl READ currentServerUrl NOTIFY selectedServerChanged)

  public:
    explicit ServerSelectorController(
        AccountManager *accountManager, LoginScreenController *loginScreenController,
        QObject *parent = nullptr);
    ~ServerSelectorController() override = default;

    Q_INVOKABLE void select(int index);
    Q_INVOKABLE QUrl logoUrlForServer(int index);
    Q_INVOKABLE QUrl faviconUrlForServer(int index);

    Q_REQUIRED_RESULT int count() const;
    Q_REQUIRED_RESULT int selectedServer() const;
    Q_REQUIRED_RESULT bool addingNewServer() const;
    Q_REQUIRED_RESULT QUrl currentServerUrl() const;

  Q_SIGNALS:
    void selectedServerChanged();
    void countChanged();

  private:
    AccountManager *m_accountManager = nullptr;
    LoginScreenController *m_loginScreenController = nullptr;
    int m_selectedServer = 0;
    bool m_addingNewServer = false;
};

} // namespace Controllers
} // namespace RuqolaQmlPlain

Q_DECLARE_METATYPE(RuqolaQmlPlain::Controllers::ServerSelectorController *);
