/*
   Copyright (c) 2017-2020 Laurent Montel <montel@kde.org>

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

#include "messages/message.h"
#include "rocketchataccount.h"
#include "roomwrapper.h"
#include "ruqola.h"
#include "ruqola_debug.h"
#include "ruqolaregisterengine.h"
#include "ruqolautils.h"
#include "clipboardproxy.h"
#include "authenticationmanager.h"
#include "accountmanager.h"
#include "notificationoptionswrapper.h"
#include "receivetypingnotificationmanager.h"
#include "serverconfiginfo.h"

#include "model/usersforroommodel.h"
#include "model/usersmodel.h"
#include "model/roommodel.h"
#include "model/statusmodel.h"
#include "model/usersforroomfilterproxymodel.h"
#include "model/usercompleterfilterproxymodel.h"
#include "model/filesforroomfilterproxymodel.h"
#include "model/searchchannelfilterproxymodel.h"
#include "model/inputcompletermodel.h"
#include "model/roomfilterproxymodel.h"
#include "model/usercompletermodel.h"
#include "model/loginmethodmodel.h"
#include "model/searchmessagefilterproxymodel.h"
#include "model/rocketchataccountmodel.h"
#include "model/rocketchataccountfilterproxymodel.h"
#include "model/emoticonmodel.h"
#include "model/notificationpreferencemodel.h"
#include "model/notificationdesktopdurationpreferencemodel.h"
#include "model/notificationdesktopsoundpreferencemodel.h"
#include "model/threadsfilterproxymodel.h"
#include "model/listmessagesmodel.h"
#include "model/discussionsfilterproxymodel.h"
#include "model/emoticoncategoriesmodel.h"
#include "notificationpreferences.h"
#include "model/listmessagesmodelfilterproxymodel.h"
#include "model/autotranslatelanguagesmodel.h"

#include "channel.h"
#include <KLocalizedContext>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#define URI "Ruqola"

RuqolaRegisterEngine::RuqolaRegisterEngine()
{
}

RuqolaRegisterEngine::~RuqolaRegisterEngine()
{
    delete mEngine;
}

bool RuqolaRegisterEngine::initialize()
{
    //Singleton
    qmlRegisterSingletonType(QUrl(QStringLiteral("qrc:/ExtraColors.qml")), URI, 1, 0, "RuqolaSingleton");
    qmlRegisterSingletonType(QUrl(QStringLiteral("qrc:/DebugCategory.qml")), URI, 1, 0, "RuqolaDebugCategorySingleton");
    qmlRegisterSingletonType<Ruqola>(URI, 1, 0, "Ruqola", ruqola_singletontype_provider);
    qmlRegisterSingletonType<RuqolaUtils>(URI, 1, 0, "RuqolaUtils", ruqolautils_singletontype_provider);
    qmlRegisterSingletonType<NotificationPreferences>(URI, 1, 0, "NotificationPreferences", notificationpreferences_singletontype_provider);

    qmlRegisterType<MessageModel>(URI, 1, 0, "MessageModel");
    qmlRegisterType<RoomWrapper>(URI, 1, 0, "RoomWrapper");
    qmlRegisterType<RoomModel>(URI, 1, 0, "RoomModel");
    qmlRegisterType<Room>(URI, 1, 0, "Room");
    qmlRegisterType<RocketChatAccount>(URI, 1, 0, "RocketChatAccount");
    qmlRegisterType<UsersModel>(URI, 1, 0, "UsersModel");
    qmlRegisterType<RoomFilterProxyModel>(URI, 1, 0, "RoomFilterProxyModel");
    qmlRegisterType<UsersForRoomFilterProxyModel>(URI, 1, 0, "UsersForRoomFilterProxyModel");
    qmlRegisterType<UserCompleterFilterProxyModel>(URI, 1, 0, "UserCompleterFilterModelProxy");
    qmlRegisterType<FilesForRoomFilterProxyModel>(URI, 1, 0, "FilesForRoomFilterProxyModel");
    qmlRegisterType<UserCompleterModel>(URI, 1, 0, "UserCompleterModel");
    qmlRegisterType<StatusModel>(URI, 1, 0, "StatusModel");
    qmlRegisterType<NotificationPreferenceModel>(URI, 1, 0, "NotificationPreferenceModel");
    qmlRegisterType<NotificationDesktopDurationPreferenceModel>(URI, 1, 0, "NotificationDesktopDurationPreferenceModel");
    qmlRegisterType<NotificationDesktopSoundPreferenceModel>(URI, 1, 0, "NotificationDesktopSoundPreferenceModel");
    qmlRegisterType<SearchChannelFilterProxyModel>(URI, 1, 0, "SearchChannelFilterProxyModel");
    qmlRegisterType<InputCompleterModel>(URI, 1, 0, "InputCompleterModel");
    qmlRegisterType<LoginMethodModel>(URI, 1, 0, "LoginMethodModel");
    qmlRegisterType<SearchMessageFilterProxyModel>(URI, 1, 0, "SearchMessageFilterProxyModel");
    qmlRegisterType<AccountManager>(URI, 1, 0, "AccountManager");
    qmlRegisterType<RocketChatAccountFilterProxyModel>(URI, 1, 0, "RocketChatAccountFilterProxyModel");
    qmlRegisterType<RocketChatAccountModel>(URI, 1, 0, "RocketChatAccountModel");
    qmlRegisterType<ClipboardProxy>(URI, 1, 0, "Clipboard");
    qmlRegisterType<NotificationOptionsWrapper>(URI, 1, 0, "NotificationOptionsWrapper");
    qmlRegisterType<EmoticonModel>(URI, 1, 0, "EmoticonModel");
    qmlRegisterType<ThreadsFilterProxyModel>(URI, 1, 0, "ThreadsFilterProxyModel");
    qmlRegisterType<DiscussionsFilterProxyModel>(URI, 1, 0, "DiscussionsFilterProxyModel");
    qmlRegisterType<EmoticonCategoriesModel>(URI, 1, 0, "EmoticonCategoriesModel");
    qmlRegisterType<ListMessagesModelFilterProxyModel>(URI, 1, 0, "ListMessagesModelFilterProxyModel");
    qmlRegisterType<AutotranslateLanguagesModel>(URI, 1, 0, "AutotranslateLanguagesModel");

    qmlRegisterType<ReceiveTypingNotificationManager>(URI, 1, 0, "ReceiveTypingNotificationManager");
    qmlRegisterUncreatableType<Message>(URI, 1, 0, "Message", QStringLiteral("MessageType is an enum container"));
    qmlRegisterUncreatableType<Channel>(URI, 1, 0, "Channel", QStringLiteral("ChannelType is an enum container"));
    qmlRegisterUncreatableType<DDPClient>(URI, 1, 0, "DDPClient", QStringLiteral("DDPClient.LoginStatus is an enum container"));
    qmlRegisterType<ServerConfigInfo>(URI, 1, 0, "ServerConfigInfo");
    qmlRegisterType<ListMessagesModel>(URI, 1, 0, "ListMessagesModel");
    qRegisterMetaType<Message::MessageType>();
    qRegisterMetaType<RocketChatAccount::RoomInfoType>();
    qRegisterMetaType<RocketChatAccount::NotificationOptionsType>();
    qRegisterMetaType<RocketChatAccount::RoleType>();
    qRegisterMetaType<ListMessagesModel::ListMessageType>();
    qRegisterMetaType<User::PresenceStatus>();
    qRegisterMetaType<AuthenticationManager::OauthType>();
    qRegisterMetaType<KAboutData>();
    mEngine = new QQmlApplicationEngine;

    QQmlContext *ctxt = mEngine->rootContext();
#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
    qmlRegisterType<Notification>(URI, 1, 0, "Notification");
    ctxt->setContextProperty(QStringLiteral("systrayIcon"), Ruqola::self()->notification());
#endif
    ctxt->setContextObject(new KLocalizedContext(mEngine));

    mEngine->load(QUrl(QStringLiteral("qrc:/Desktop.qml")));

    if (mEngine->rootObjects().isEmpty()) {
        qCWarning(RUQOLA_LOG) << "Impossible to load object from Desktop.qml";
        return false;
    }
    return true;
}
