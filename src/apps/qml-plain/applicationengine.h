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

#include <QDebug>
#include <QQmlApplicationEngine>

class AccountManager;
class ManagerDataPaths;

class RocketChatMessageItem;
class RocketChatIconsModel;

namespace RuqolaQmlPlain {

namespace Controllers {
class ChatScreenController;
class EmojiPopupController;
class LoginScreenController;
class ScreenController;
class ServerSelectorController;
class SettingsController;
} // namespace Controllers

class ApplicationEngine : public QQmlApplicationEngine
{
    Q_OBJECT

  public:
    explicit ApplicationEngine(QObject *parent = nullptr);
    ~ApplicationEngine() override
    {
        qDebug() << "Destroying appengine";
    }

    bool initialize();

  private:
    ManagerDataPaths *m_managersDataPaths = nullptr;
    AccountManager *m_accountManager = nullptr;
    Controllers::LoginScreenController *m_loginScreenController = nullptr;
    Controllers::ServerSelectorController *m_serverSelectorController = nullptr;
    Controllers::ScreenController *m_screenController = nullptr;
    Controllers::ChatScreenController *m_chatScreenController = nullptr;
    Controllers::SettingsController *m_settingsController = nullptr;
    RocketChatIconsModel *m_rocketChatIconsModel = nullptr;
    Controllers::EmojiPopupController *m_emojiPopupController = nullptr;
};

} // namespace RuqolaQmlPlain
