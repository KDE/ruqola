/*
   Copyright (c) 2017-2019 Montel Laurent <montel@kde.org>

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
#include "aboutdata/qmlaboutdata.h"
#include "aboutdata/qmlaboutdataauthormodel.h"
#include "ruqolaregisterengine.h"
#include "ruqolautils.h"
#include "clipboardproxy.h"
#include "authenticationmanager.h"
#include "accountmanager.h"
#include "notificationoptionswrapper.h"
#include "receivetypingnotificationmanager.h"
#include "serverconfiginfo.h"
#include "userwrapper.h"

#include "model/usersforroommodel.h"
#include "model/usersmodel.h"
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
#include "model/mentionsfilterproxymodel.h"
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
    qmlRegisterSingletonType(QUrl(QStringLiteral("qrc:/ExtraColors.qml")), "KDE.Ruqola.ExtraColors", 1, 0, "RuqolaSingleton");
    qmlRegisterSingletonType(QUrl(QStringLiteral("qrc:/DebugCategory.qml")), "KDE.Ruqola.DebugCategory", 1, 0, "RuqolaDebugCategorySingleton");
    qmlRegisterSingletonType<Ruqola>("KDE.Ruqola.Ruqola", 1, 0, "Ruqola", ruqola_singletontype_provider);
    qmlRegisterSingletonType<RuqolaUtils>("KDE.Ruqola.RuqolaUtils", 1, 0, "RuqolaUtils", ruqolautils_singletontype_provider);
    qmlRegisterSingletonType<NotificationPreferences>("KDE.Ruqola.NotificationPreferences", 1, 0, "NotificationPreferences", notificationpreferences_singletontype_provider);

    qmlRegisterType<MessageModel>("KDE.Ruqola.MessageModel", 1, 0, "MessageModel");
    qmlRegisterType<RoomWrapper>("KDE.Ruqola.RoomWrapper", 1, 0, "RoomWrapper");
    qmlRegisterType<UserWrapper>("KDE.Ruqola.UserWrapper", 1, 0, "UserWrapper");
    qmlRegisterType<Room>("KDE.Ruqola.Room", 1, 0, "Room");
    qmlRegisterType<RocketChatAccount>("KDE.Ruqola.RocketChatAccount", 1, 0, "RocketChatAccount");
    qmlRegisterType<UsersModel>("KDE.Ruqola.UsersModel", 1, 0, "UsersModel");
    qmlRegisterType<RoomFilterProxyModel>("KDE.Ruqola.RoomFilterProxyModel", 1, 0, "RoomFilterProxyModel");
    qmlRegisterType<UsersForRoomFilterProxyModel>("KDE.Ruqola.UsersForRoomFilterProxyModel", 1, 0, "UsersForRoomFilterProxyModel");
    qmlRegisterType<UserCompleterFilterProxyModel>("KDE.Ruqola.UserCompleterFilterModelProxy", 1, 0, "UserCompleterFilterModelProxy");
    qmlRegisterType<FilesForRoomFilterProxyModel>("KDE.Ruqola.FilesForRoomFilterProxyModel", 1, 0, "FilesForRoomFilterProxyModel");
    qmlRegisterType<MentionsFilterProxyModel>("KDE.Ruqola.MentionsFilterProxyModel", 1, 0, "MentionsFilterProxyModel");
    qmlRegisterType<QmlAboutData>("KDE.Ruqola.RuqolaAboutData", 1, 0, "RuqolaAboutData");
    qmlRegisterType<QmlAboutDataAuthorModel>("KDE.Ruqola.RuqolaAboutDataAuthorModel", 1, 0, "RuqolaAboutDataAuthorModel");
    qmlRegisterType<UserCompleterModel>("KDE.Ruqola.UserCompleterModel", 1, 0, "UserCompleterModel");
    qmlRegisterType<StatusModel>("KDE.Ruqola.StatusModel", 1, 0, "StatusModel");
    qmlRegisterType<NotificationPreferenceModel>("KDE.Ruqola.NotificationPreferenceModel", 1, 0, "NotificationPreferenceModel");
    qmlRegisterType<NotificationDesktopDurationPreferenceModel>("KDE.Ruqola.NotificationDesktopDurationPreferenceModel", 1, 0, "NotificationDesktopDurationPreferenceModel");
    qmlRegisterType<NotificationDesktopSoundPreferenceModel>("KDE.Ruqola.NotificationDesktopSoundPreferenceModel", 1, 0, "NotificationDesktopSoundPreferenceModel");
    qmlRegisterType<SearchChannelFilterProxyModel>("KDE.Ruqola.SearchChannelFilterProxyModel", 1, 0, "SearchChannelFilterProxyModel");
    qmlRegisterType<InputCompleterModel>("KDE.Ruqola.InputCompleterModel", 1, 0, "InputCompleterModel");
    qmlRegisterType<LoginMethodModel>("KDE.Ruqola.LoginMethodModel", 1, 0, "LoginMethodModel");
    qmlRegisterType<SearchMessageFilterProxyModel>("KDE.Ruqola.SearchMessageFilterProxyModel", 1, 0, "SearchMessageFilterProxyModel");
    qmlRegisterType<AccountManager>("KDE.Ruqola.AccountManager", 1, 0, "AccountManager");
    qmlRegisterType<RocketChatAccountFilterProxyModel>("KDE.Ruqola.RocketChatAccountFilterProxyModel", 1, 0, "RocketChatAccountFilterProxyModel");
    qmlRegisterType<RocketChatAccountModel>("KDE.Ruqola.RocketChatAccountModel", 1, 0, "RocketChatAccountModel");
    qmlRegisterType<ClipboardProxy>("KDE.Ruqola.Clipboard", 1, 0, "Clipboard");
    qmlRegisterType<NotificationOptionsWrapper>("KDE.Ruqola.NotificationOptionsWrapper", 1, 0, "NotificationOptionsWrapper");
    qmlRegisterType<EmoticonModel>("KDE.Ruqola.EmoticonModel", 1, 0, "EmoticonModel");
    qmlRegisterType<ThreadsFilterProxyModel>("KDE.Ruqola.ThreadsFilterProxyModel", 1, 0, "ThreadsFilterProxyModel");
    qmlRegisterType<DiscussionsFilterProxyModel>("KDE.Ruqola.DiscussionsFilterProxyModel", 1, 0, "DiscussionsFilterProxyModel");
    qmlRegisterType<EmoticonCategoriesModel>("KDE.Ruqola.EmoticonCategoriesModel", 1, 0, "EmoticonCategoriesModel");
    qmlRegisterType<ListMessagesModelFilterProxyModel>("KDE.Ruqola.ListMessagesModelFilterProxyModel", 1, 0, "ListMessagesModelFilterProxyModel");
    qmlRegisterType<AutotranslateLanguagesModel>("KDE.Ruqola.AutotranslateLanguagesModel", 1, 0, "AutotranslateLanguagesModel");

    qmlRegisterType<ReceiveTypingNotificationManager>("KDE.Ruqola.ReceiveTypingNotificationManager", 1, 0, "ReceiveTypingNotificationManager");
    qmlRegisterUncreatableType<Message>("KDE.Ruqola.Message", 1, 0, "Message", QStringLiteral("MessageType is an enum container"));
    qmlRegisterUncreatableType<Channel>("KDE.Ruqola.Channel", 1, 0, "Channel", QStringLiteral("ChannelType is an enum container"));
    qmlRegisterUncreatableType<DDPClient>("KDE.Ruqola.DDPClient", 1, 0, "DDPClient", QStringLiteral("DDPClient.LoginStatus is an enum container"));
    qmlRegisterType<ServerConfigInfo>("KDE.Ruqola.ServerConfigInfo", 1, 0, "ServerConfigInfo");
    qmlRegisterType<ListMessagesModel>("KDE.Ruqola.ListMessagesModel", 1, 0, "ListMessagesModel");
    qRegisterMetaType<Message::MessageType>();
    qRegisterMetaType<RocketChatAccount::RoomInfoType>();
    qRegisterMetaType<RocketChatAccount::NotificationOptionsType>();
    qRegisterMetaType<RocketChatAccount::RoleType>();
    qRegisterMetaType<ListMessagesModel::ListMessageType>();
    qRegisterMetaType<User::PresenceStatus>();
    qRegisterMetaType<AuthenticationManager::OauthType>();
    mEngine = new QQmlApplicationEngine;

    QQmlContext *ctxt = mEngine->rootContext();
#if !defined(Q_OS_ANDROID) || !defined(Q_OS_IOS)
    qmlRegisterType<Notification>("KDE.Ruqola.Notification", 1, 0, "Notification");
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
