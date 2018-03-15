/*
   Copyright (c) 2018 Montel Laurent <montel@kde.org>

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

#include "restapimethodtest.h"
#include "restapi/restapimethod.h"
#include <QTest>
QTEST_GUILESS_MAIN(RestApiMethodTest)

RestApiMethodTest::RestApiMethodTest(QObject *parent)
    : QObject(parent)
{
}

void RestApiMethodTest::shouldHaveDefaultValue()
{
    RestApiMethod rest;
    QVERIFY(rest.serverUrl().isEmpty());
}

void RestApiMethodTest::shouldAssignDefaultValue()
{
    RestApiMethod rest;
    QString newUrl = QStringLiteral("http://www.kde.org");
    rest.setServerUrl(newUrl);
    QCOMPARE(rest.serverUrl(), newUrl);
}

void RestApiMethodTest::shouldGenerateUrl()
{
    RestApiMethod rest;
    //If server url is empty return null url
    QVERIFY(!rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsList).isValid());
    rest.setServerUrl(QStringLiteral("http://www.kde.org"));
    QVERIFY(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsList).isValid());

    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Login), QUrl(QStringLiteral("http://www.kde.org/api/v1/login")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Logout), QUrl(QStringLiteral("http://www.kde.org/api/v1/logout")));
#if 0
case RestApiUtil::RestApiUrlType::ChatUnStarMessage:
    return QStringLiteral("chat.unStarMessage");
case RestApiUtil::RestApiUrlType::ChatUpdate:
    return QStringLiteral("chat.update");
case RestApiUtil::RestApiUrlType::ChannelsAddAll:
    return QStringLiteral("channels.addAll");
case RestApiUtil::RestApiUrlType::ChannelsAddModerator:
    return QStringLiteral("channels.addModerator");
case RestApiUtil::RestApiUrlType::ChannelsAddOwner:
    return QStringLiteral("channels.addOwner");
case RestApiUtil::RestApiUrlType::ChannelsArchive:
    return QStringLiteral("channels.archive");
case RestApiUtil::RestApiUrlType::ChannelsCleanHistory:
    return QStringLiteral("channels.cleanHistory");
case RestApiUtil::RestApiUrlType::ChannelsClose:
    return QStringLiteral("channels.close");
case RestApiUtil::RestApiUrlType::ChannelsCreate:
    return QStringLiteral("channels.create");
case RestApiUtil::RestApiUrlType::ChannelsGetIntegrations:
    return QStringLiteral("channels.getIntegrations");
case RestApiUtil::RestApiUrlType::ChannelsHistory:
    return QStringLiteral("channels.history");
case RestApiUtil::RestApiUrlType::ChannelsInfo:
    return QStringLiteral("channels.info");
case RestApiUtil::RestApiUrlType::ChannelsInvite:
    return QStringLiteral("channels.invite");
case RestApiUtil::RestApiUrlType::ChannelsKick:
    return QStringLiteral("channels.kick");
case RestApiUtil::RestApiUrlType::ChannelsLeave:
    return QStringLiteral("channels.leave");
case RestApiUtil::RestApiUrlType::ChannelsList:
    return QStringLiteral("channels.list");
case RestApiUtil::RestApiUrlType::ChannelsListJoined:
    return QStringLiteral("channels.list.joined");
case RestApiUtil::RestApiUrlType::ChannelsOpen:
    return QStringLiteral("channels.open");
case RestApiUtil::RestApiUrlType::ChannelsRemoveModerator:
    return QStringLiteral("channels.removeModerator");
case RestApiUtil::RestApiUrlType::ChannelsRemoveOwner:
    return QStringLiteral("channels.removeOwner");
case RestApiUtil::RestApiUrlType::ChannelsRename:
    return QStringLiteral("channels.rename");
case RestApiUtil::RestApiUrlType::ChannelsSetDescription:
    return QStringLiteral("channels.setDescription");
case RestApiUtil::RestApiUrlType::ChannelsSetJoinCode:
    return QStringLiteral("channels.setJoinCode");
case RestApiUtil::RestApiUrlType::ChannelsSetPurpose:
    return QStringLiteral("channels.setPurpose");
case RestApiUtil::RestApiUrlType::ChannelsSetReadOnly:
    return QStringLiteral("channels.setReadOnly");
case RestApiUtil::RestApiUrlType::ChannelsSetTopic:
    return QStringLiteral("channels.setTopic");
case RestApiUtil::RestApiUrlType::ChannelsSetType:
    return QStringLiteral("channels.setType");
case RestApiUtil::RestApiUrlType::ChannelsUnarchive:
    return QStringLiteral("channels.unarchive");
case RestApiUtil::RestApiUrlType::GroupsAddAll:
    return QStringLiteral("groups.addAll");
case RestApiUtil::RestApiUrlType::GroupsAddModerator:
    return QStringLiteral("groups.addModerator");
case RestApiUtil::RestApiUrlType::GroupsAddOwner:
    return QStringLiteral("groups.addOwner");
case RestApiUtil::RestApiUrlType::GroupsArchive:
    return QStringLiteral("groups.archive");
case RestApiUtil::RestApiUrlType::GroupsClose:
    return QStringLiteral("groups.close");
case RestApiUtil::RestApiUrlType::GroupsCreate:
    return QStringLiteral("groups.create");
case RestApiUtil::RestApiUrlType::GroupsGetIntegrations:
    return QStringLiteral("groups.getIntegrations");
case RestApiUtil::RestApiUrlType::GroupsHistory:
    return QStringLiteral("groups.history");
case RestApiUtil::RestApiUrlType::GroupsInfo:
    return QStringLiteral("groups.info");
case RestApiUtil::RestApiUrlType::GroupsInvite:
    return QStringLiteral("groups.invite");
case RestApiUtil::RestApiUrlType::GroupsKick:
    return QStringLiteral("groups.kick");
case RestApiUtil::RestApiUrlType::GroupsLeave:
    return QStringLiteral("groups.leave");
case RestApiUtil::RestApiUrlType::GroupsList:
    return QStringLiteral("groups.list");
case RestApiUtil::RestApiUrlType::GroupsOpen:
    return QStringLiteral("groups.open");
case RestApiUtil::RestApiUrlType::GroupsRemoveModerator:
    return QStringLiteral("groups.removeModerator");
case RestApiUtil::RestApiUrlType::GroupRemoveOwner:
    return QStringLiteral("groups.removeOwner");
case RestApiUtil::RestApiUrlType::GroupsRename:
    return QStringLiteral("groups.rename");
case RestApiUtil::RestApiUrlType::GroupsSetDescription:
    return QStringLiteral("groups.setDescription");
case RestApiUtil::RestApiUrlType::GroupsSetPurpose:
    return QStringLiteral("groups.setPurpose");
case RestApiUtil::RestApiUrlType::GroupsSetReadOnly:
    return QStringLiteral("groups.setReadOnly");
case RestApiUtil::RestApiUrlType::GroupsSetTopic:
    return QStringLiteral("groups.setTopic");
case RestApiUtil::RestApiUrlType::GroupsSetType:
    return QStringLiteral("groups.setType");
case RestApiUtil::RestApiUrlType::GroupsUnarchive:
    return QStringLiteral("groups.unarchive");
case RestApiUtil::RestApiUrlType::ServerInfo:
    return QStringLiteral("info");
case RestApiUtil::RestApiUrlType::Settings:
    return QStringLiteral("settings");
case RestApiUtil::RestApiUrlType::RoomsUpload:
    return QStringLiteral("rooms.upload");
    #endif
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Me), QUrl(QStringLiteral("http://www.kde.org/api/v1/me")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersCreate), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.create")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersCreateToken), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.createToken")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersGetPresence), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.getPresence")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersInfo), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.info")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersList), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.list")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersRegister), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.register")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersGetAvatar), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.getAvatar")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersResetAvatar), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.resetAvatar")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersSetAvatar), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.setAvatar")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::UsersUpdate), QUrl(QStringLiteral("http://www.kde.org/api/v1/users.update")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatDelete), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.delete")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatGetMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.getMessage")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatPinMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.pinMessage")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatPostMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.postMessage")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatReact), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.react")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatStarMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.starMessage")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatUnPinMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/chat.unPinMessage")));
#if 0
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatUnStarMessage), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChatUpdate), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddAll), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddModerator), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsAddOwner), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsArchive), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsCleanHistory), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsClose), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsCreate), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsGetIntegrations), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsHistory), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsInfo), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsInvite), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsKick), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsLeave), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsList), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsListJoined), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsOpen), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsRemoveModerator), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsRemoveOwner), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsRename), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetDescription), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetJoinCode), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetPurpose), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetReadOnly), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetTopic), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsSetType), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ChannelsUnarchive), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsAddAll), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsAddModerator), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsAddOwner), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsArchive), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsClose), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsCreate), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsGetIntegrations), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsHistory), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsInfo), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsInvite), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsKick), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsLeave), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsList), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsOpen), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsRemoveModerator), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupRemoveOwner), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsRename), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetDescription), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetPurpose), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetReadOnly), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetTopic), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsSetType), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::GroupsUnarchive), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::ServerInfo), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::Settings), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
    QCOMPARE(rest.generateUrl(RestApiUtil::RestApiUrlType::RoomsUpload), QUrl(QStringLiteral("http://www.kde.org/api/v1/")));
#endif
}
