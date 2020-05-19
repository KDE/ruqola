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
class ServerSelectorController;

class ScreenController : public QObject
{
    Q_OBJECT

  public:
    enum Screen : int {
        LoginScreen,
        ChatScreen,
        TestScreen,
    };
    Q_ENUM(Screen);

    Q_PROPERTY(RuqolaQmlPlain::Controllers::ScreenController::Screen currentScreen READ
                   currentScreen WRITE setCurrentScreen NOTIFY currentScreenChanged)
    Q_PROPERTY(QUrl loaderUrl READ loaderUrl NOTIFY currentScreenChanged)

  public:
    explicit ScreenController(
        AccountManager *accountManager, LoginScreenController *loginScreenController,
        ServerSelectorController *serverSelectorController, QObject *parent = nullptr);
    ~ScreenController() override = default;

    Q_REQUIRED_RESULT Screen currentScreen() const;
    Q_REQUIRED_RESULT QUrl loaderUrl() const;

    void setCurrentScreen(Screen s);

  Q_SIGNALS:
    void currentScreenChanged();
    void loaderUrlChanged();

  private:
    Screen m_currentScreen = Screen::LoginScreen;
    AccountManager *m_accountManager = nullptr;
    LoginScreenController *m_loginScreenController = nullptr;
    ServerSelectorController *m_serverSelectorController = nullptr;

    static QHash<int, QUrl> s_screenToUrl;

    void handleServerSelectionChange();
};

} // namespace Controllers
} // namespace RuqolaQmlPlain

Q_DECLARE_METATYPE(RuqolaQmlPlain::Controllers::ScreenController *);
