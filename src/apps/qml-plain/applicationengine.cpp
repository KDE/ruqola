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

#include "applicationengine.h"

#include "controllers/screencontroller.h"
#include "ruqolaqmlplain_debug.h"

#include <QDebug>
#include <QLoggingCategory>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>

// ruqola qml controllers
#include "controllers/chatscreencontroller.h"
#include "controllers/emojipopupcontroller.h"
#include "controllers/loginscreencontroller.h"
#include "controllers/screencontroller.h"
#include "controllers/serverselectorcontroller.h"
#include "controllers/settingscontroller.h"

// Core
#include "accountmanager.h"
#include "emoticons/emojiqmlmodel.h"
#include "managerdatapaths.h"
#include "model/messagemodel.h"
#include "model/rocketchataccountmodel.h"
#include "model/roomfilterproxymodel.h"

#include "models/rocketchaticonsmodel.h"
#include "models/roomavatarsmodel.h"

namespace RuqolaQmlPlain {

ApplicationEngine::ApplicationEngine(QObject *parent)
    : QQmlApplicationEngine(parent)
    , m_managersDataPaths(ManagerDataPaths::self())
    , m_accountManager(new AccountManager(this, false))
    , m_loginScreenController(new Controllers::LoginScreenController(m_accountManager, this))
    , m_serverSelectorController(new Controllers::ServerSelectorController(
          m_accountManager, m_loginScreenController, this))
    , m_screenController(new Controllers::ScreenController(
          m_accountManager, m_loginScreenController, m_serverSelectorController, this))
    , m_chatScreenController(new Controllers::ChatScreenController(m_accountManager, this))
    , m_settingsController(new Controllers::SettingsController(this))
    , m_rocketChatIconsModel(new RocketChatIconsModel(this))
    , m_emojiPopupController(new Controllers::EmojiPopupController(m_accountManager, this))
{
}

bool ApplicationEngine::initialize()
{
    qRegisterMetaType<RocketChatAccountModel *>();

    qRegisterMetaType<MessageModel *>();
    qRegisterMetaType<RoomFilterProxyModel *>();
    qRegisterMetaType<EmojiQmlModel *>();
    qRegisterMetaType<EmojiCategoryQmlModel *>();
    qRegisterMetaType<RoomAvatarsModel *>();

    rootContext()->setContextProperty(QStringLiteral("_screenController"), m_screenController);
    rootContext()->setContextProperty(QStringLiteral("_accountManager"), m_accountManager);
    rootContext()->setContextProperty(
        QStringLiteral("_loginScreenController"), m_loginScreenController);
    rootContext()->setContextProperty(
        QStringLiteral("_serverSelectorController"), m_serverSelectorController);
    rootContext()->setContextProperty(
        QStringLiteral("_chatScreenController"), m_chatScreenController);
    rootContext()->setContextProperty(QStringLiteral("_settings"), m_settingsController);
    rootContext()->setContextProperty(QStringLiteral("_iconsModel"), m_rocketChatIconsModel);
    rootContext()->setContextProperty(
        QStringLiteral("_emojiPopupController"), m_emojiPopupController);

    qmlRegisterUncreatableType<Controllers::LoginScreenControllerEnumContainer>(
        "Ruqola", 1, 0, "LoginScreenController", QStringLiteral(""));
    qmlRegisterUncreatableType<Controllers::ChatScreenController>(
        "Ruqola", 1, 0, "ChatScreenController", QStringLiteral(""));

    load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (rootObjects().isEmpty()) {
        qCWarning(RUQOLAQMLPLAIN_LOG) << "Impossible to load object from main.qml";
        return false;
    }

    return true;
}

} // namespace RuqolaQmlPlain
