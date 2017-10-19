/*
   Copyright (c) 2017 Montel Laurent <montel@kde.org>

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License or
   ( at your option ) version 3 or, at the discretion of KDE e.V.
   ( which shall act as a proxy as in section 14 of the GPLv3 ), any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "message.h"
#include "rocketchataccount.h"
#include "roomfilterproxymodel.h"
#include "roomwrapper.h"
#include "ruqola.h"
#include "ruqolaaboutdata.h"
#include "ruqolaregisterengine.h"
#include "ruqolautils.h"
#include "usersforroommodel.h"
#include "usersmodel.h"

#include <KLocalizedContext>
#include <QQmlApplicationEngine>
#include <QQmlContext>

RuqolaRegisterEngine::RuqolaRegisterEngine()
{
}

RuqolaRegisterEngine::~RuqolaRegisterEngine()
{
    delete mEngine;
}

bool RuqolaRegisterEngine::initialize()
{
    qmlRegisterSingletonType(QUrl(QStringLiteral("qrc:/ExtraColors.qml")), "KDE.Ruqola.ExtraColors", 1, 0, "RuqolaSingleton");
    qmlRegisterSingletonType<Ruqola>("KDE.Ruqola.Ruqola", 1, 0, "Ruqola", ruqola_singletontype_provider);
    qmlRegisterSingletonType<RuqolaUtils>("KDE.Ruqola.RuqolaUtils", 1, 0, "RuqolaUtils", ruqolautils_singletontype_provider);
    qmlRegisterType<MessageModel>("KDE.Ruqola.MessageModel", 1, 0, "MessageModel");
    qmlRegisterType<DDPClient>("KDE.Ruqola.DDPClient", 1, 0, "DDPClient");
    qmlRegisterType<RoomWrapper>("KDE.Ruqola.RoomWrapper", 1, 0, "RoomWrapper");
    qmlRegisterType<UsersForRoomModel>("KDE.Ruqola.UsersForRoomModel", 1, 0, "UsersForRoomModel");
    qmlRegisterType<RocketChatAccount>("KDE.Ruqola.RocketChatAccount", 1, 0, "RocketChatAccount");
    qmlRegisterType<UsersModel>("KDE.Ruqola.UsersModel", 1, 0, "UsersModel");
    qmlRegisterType<User>("KDE.Ruqola.User", 1, 0, "User");
    qmlRegisterType<RoomFilterProxyModel>("KDE.Ruqola.RoomFilterProxyModel", 1, 0, "RoomFilterProxyModel");
    qmlRegisterType<RuqolaAboutData>("KDE.Ruqola.RuqolaAboutData", 1, 0, "RuqolaAboutData");
    qRegisterMetaType<Message::MessageType>();
    qmlRegisterUncreatableType<Message>("KDE.Ruqola.Message", 1, 0, "Message", QStringLiteral("MessageType is an enum container"));
    (void)Ruqola::self();
    mEngine = new QQmlApplicationEngine;

    QQmlContext *ctxt = mEngine->rootContext();
#if !defined(Q_OS_ANDROID) || !defined(Q_OS_IOS)
    qmlRegisterType<Notification>("KDE.Ruqola.Notification", 1, 0, "Notification");
    ctxt->setContextProperty(QStringLiteral("systrayIcon"), Ruqola::self()->notification());
#endif
    ctxt->setContextObject(new KLocalizedContext(mEngine));

    mEngine->load(QUrl(QStringLiteral("qrc:/Desktop.qml")));

    if (mEngine->rootObjects().isEmpty()) {
        return false;
    }
    return true;
}
