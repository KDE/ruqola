/*
   Copyright (c) 2017-2018 Montel Laurent <montel@kde.org>

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

#include "rocketchatmessagetest.h"
#include "rocketchatmessage.h"
#include <QProcess>
#include <QTest>
QTEST_GUILESS_MAIN(RocketChatMessageTest)

RocketChatMessageTest::RocketChatMessageTest(QObject *parent)
    : QObject(parent)
{
}

void RocketChatMessageTest::compareFile(const QString &data, const QString &name)
{
    const QString refFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1Char('/') + name + QStringLiteral(".ref");
    const QString generatedFile = QLatin1String(RUQOLA_DATA_DIR) + QLatin1Char('/') + name + QStringLiteral("-generated.ref");
    //Create generated file
    QFile f(generatedFile);
    QVERIFY(f.open(QIODevice::WriteOnly | QIODevice::Truncate));
    f.write(data.toUtf8());
    f.close();

    // compare to reference file
    QStringList args = QStringList()
                       << QStringLiteral("-u")
                       << refFile
                       << generatedFile;
    QProcess proc;
    proc.setProcessChannelMode(QProcess::ForwardedChannels);
    proc.start(QStringLiteral("diff"), args);
    QVERIFY(proc.waitForFinished());
    QCOMPARE(proc.exitCode(), 0);
}

void RocketChatMessageTest::shouldGenerateSetTemporaryStatus()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.setTemporaryStatus(User::PresenceStatus::PresenceBusy, 42);

    compareFile(r.result, QStringLiteral("temporarybusy"));

    r = m.setTemporaryStatus(User::PresenceStatus::PresenceAway, 43);
    compareFile(r.result, QStringLiteral("temporaryaway"));

    r = m.setTemporaryStatus(User::PresenceStatus::PresenceOffline, 44);
    compareFile(r.result, QStringLiteral("temporaryoffline"));

    r = m.setTemporaryStatus(User::PresenceStatus::PresenceOnline, 45);
    compareFile(r.result, QStringLiteral("temporaryonline"));
}

void RocketChatMessageTest::shouldHideRoom()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.hideRoom(QStringLiteral("foo"), 42);

    compareFile(r.result, QStringLiteral("hide"));
}

void RocketChatMessageTest::shouldLeaveRoom()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.leaveRoom(QStringLiteral("foo"), 42);

    compareFile(r.result, QStringLiteral("leave"));
}

void RocketChatMessageTest::shouldOpenRoom()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.openRoom(QStringLiteral("foo"), 42);

    compareFile(r.result, QStringLiteral("openroom"));
}

void RocketChatMessageTest::shouldUnarchiveRoom()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.unarchiveRoom(QStringLiteral("foo"), 42);

    compareFile(r.result, QStringLiteral("unarchiveroom"));
}

void RocketChatMessageTest::shouldArchiveRoom()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.archiveRoom(QStringLiteral("foo"), 42);

    compareFile(r.result, QStringLiteral("archiveroom"));
}

void RocketChatMessageTest::shouldEraseRoom()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.eraseRoom(QStringLiteral("foo"), 42);

    compareFile(r.result, QStringLiteral("eraseroom"));
}

void RocketChatMessageTest::shouldGetRoomRoles()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.getRoomRoles(QStringLiteral("foo"), 42);

    compareFile(r.result, QStringLiteral("getroomroles"));
}

void RocketChatMessageTest::shouldSetRoomTopic()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.setRoomTopic(QStringLiteral("foo"), QStringLiteral("topic"), 42);

    compareFile(r.result, QStringLiteral("setRoomTopic"));
}

void RocketChatMessageTest::shouldToggleFavorite()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.toggleFavorite(QStringLiteral("foo"), true, 42);

    compareFile(r.result, QStringLiteral("togglefavoritetrue"));

    r = m.toggleFavorite(QStringLiteral("foo1"), false, 43);

    compareFile(r.result, QStringLiteral("togglefavoritefalse"));
}

void RocketChatMessageTest::shouldSetRoomName()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.setRoomName(QStringLiteral("foo"), QStringLiteral("name1"), 42);

    compareFile(r.result, QStringLiteral("roomname"));
}

void RocketChatMessageTest::shouldSetRoomDescription()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.setRoomDescription(QStringLiteral("foo"), QStringLiteral("description1"), 43);

    compareFile(r.result, QStringLiteral("roomdescription"));
}

void RocketChatMessageTest::shouldSetRoomAnnouncement()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.setRoomAnnouncement(QStringLiteral("foo"), QStringLiteral("announcement"), 43);

    compareFile(r.result, QStringLiteral("roomannouncement"));
}

void RocketChatMessageTest::shouldSetRoomIsReadOnly()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.setRoomIsReadOnly(QStringLiteral("foo"), true, 43);
    compareFile(r.result, QStringLiteral("roomisreadonly"));
}

void RocketChatMessageTest::shouldSetRoomIsDefault()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.setRoomIsDefault(QStringLiteral("foo"), true, 43);
    compareFile(r.result, QStringLiteral("roomisdefault"));
}

void RocketChatMessageTest::shouldJoinRoom()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.joinRoom(QStringLiteral("foo"), QStringLiteral("access_code"), 43);
    compareFile(r.result, QStringLiteral("joinroom"));
}

void RocketChatMessageTest::shouldSetRoomJoinCode()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.setRoomJoinCode(QStringLiteral("foo"), QStringLiteral("access_code"), 43);
    compareFile(r.result, QStringLiteral("setroomjoincode"));
}

void RocketChatMessageTest::shouldSetDefaultStatus()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.setDefaultStatus(User::PresenceStatus::PresenceBusy, 42);
    compareFile(r.result, QStringLiteral("defaultstatusbusy"));

    r = m.setDefaultStatus(User::PresenceStatus::PresenceAway, 43);
    compareFile(r.result, QStringLiteral("defaultstatusaway"));

    r = m.setDefaultStatus(User::PresenceStatus::PresenceOffline, 44);
    compareFile(r.result, QStringLiteral("defaultstatusoffline"));

    r = m.setDefaultStatus(User::PresenceStatus::PresenceOnline, 45);
    compareFile(r.result, QStringLiteral("defaultstatusonline"));
}

void RocketChatMessageTest::shouldInformTypingStatus()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.informTypingStatus(QStringLiteral("roomId"), QStringLiteral("user1"), false, 42);
    compareFile(r.result, QStringLiteral("informtypingstatusfalse"));

    r = m.informTypingStatus(QStringLiteral("roomId2"), QStringLiteral("user2"), true, 43);
    compareFile(r.result, QStringLiteral("informtypingstatustrue"));
}

void RocketChatMessageTest::shouldCreateRoom_data()
{
    QTest::addColumn<QString>("channelname");
    QTest::addColumn<QStringList>("userlst");
    QTest::addColumn<bool>("readonly");
    QTest::addColumn<QString>("filename");

    QTest::newRow("emptyfalse") << QStringLiteral("foo") << QStringList() << false << QStringLiteral("createroomemptyuserfalse");
    QTest::newRow("emptytrue") << QStringLiteral("foo") << QStringList() << true << QStringLiteral("createroomemptyusertrue");
    const QStringList users {
        QStringLiteral("bla"), QStringLiteral("bla2")
    };
    QTest::newRow("emptytrue") << QStringLiteral("foo") << users << true << QStringLiteral("createroomlistusertrue");
}

void RocketChatMessageTest::shouldCreateRoom()
{
    QFETCH(QString, channelname);
    QFETCH(QStringList, userlst);
    QFETCH(bool, readonly);
    QFETCH(QString, filename);

    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.createChannel(channelname, userlst, readonly, 43);
    compareFile(r.result, filename);
}

void RocketChatMessageTest::shouldCreatePrivateGroup_data()
{
    QTest::addColumn<QString>("channelname");
    QTest::addColumn<QStringList>("userlst");
    QTest::addColumn<QString>("filename");

    QTest::newRow("emptyfalse") << QStringLiteral("foo") << QStringList() << QStringLiteral("createprivategroupemptyuser");
    const QStringList users {
        QStringLiteral("bla"), QStringLiteral("bla2")
    };
    QTest::newRow("emptytrue") << QStringLiteral("foo") << users << QStringLiteral("createprivategrouplistuser");
}

void RocketChatMessageTest::shouldSetReaction()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.setReaction(QStringLiteral(":emoji:"), QStringLiteral("messid"), 43);
    compareFile(r.result, QStringLiteral("setReaction"));
}

void RocketChatMessageTest::shouldDeleteMessage()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.deleteMessage(QStringLiteral("messid"), 43);
    compareFile(r.result, QStringLiteral("deleteMessage"));
}

void RocketChatMessageTest::shouldCreatePrivateGroup()
{
    QFETCH(QString, channelname);
    QFETCH(QStringList, userlst);
    QFETCH(QString, filename);

    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.createPrivateGroup(channelname, userlst, 43);
    compareFile(r.result, filename);
}

void RocketChatMessageTest::shouldSearchMessage()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.messageSearch(QStringLiteral("roomId"), QStringLiteral("foo"), 43);
    compareFile(r.result, QStringLiteral("searchMessage"));
}

void RocketChatMessageTest::shouldListEmojiCustom()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.listEmojiCustom(43);
    compareFile(r.result, QStringLiteral("listEmojiCustom"));
}

void RocketChatMessageTest::shouldUserAutoComplete()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.userAutocomplete(QStringLiteral("foo"), QStringLiteral("exception"), 43);
    compareFile(r.result, QStringLiteral("userAutocomplete"));

    r = m.userAutocomplete(QStringLiteral("foo"), QStringLiteral("foo,bla,bli"), 43);
    compareFile(r.result, QStringLiteral("userAutocomplete-exceptions"));
}

//TODO fix me
void RocketChatMessageTest::shouldSendFileMessage()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    //FIXME
    //RocketChatMessage::RocketChatMessageResult r = m.sendFileMessage(43);
    //compareFile(r.result, QStringLiteral("sendFileMessage"));
}

void RocketChatMessageTest::shouldChannelAndPrivateAutocomplete()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.channelAndPrivateAutocomplete(QStringLiteral("room"), QStringLiteral("foo,bla"), 43);
    compareFile(r.result, QStringLiteral("channelAndPrivateAutocomplete"));
}

void RocketChatMessageTest::shouldSplotlightWithoutUsers()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.searchRoomUsers(QStringLiteral("room"), QStringLiteral("foo,bla"), false, true, 43);
    compareFile(r.result, QStringLiteral("spotlight-without-users"));
}

void RocketChatMessageTest::shouldSplotlightWithoutRooms()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.searchRoomUsers(QStringLiteral("room"), QStringLiteral("foo,bla"), true, false, 43);
    compareFile(r.result, QStringLiteral("spotlight-without-rooms"));
}

void RocketChatMessageTest::shouldAddUserToRoom()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.addUserToRoom(QStringLiteral("userId"), QStringLiteral("room"), 43);
    compareFile(r.result, QStringLiteral("addusertoroom"));
}

void RocketChatMessageTest::shouldLogin()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.login(QStringLiteral("user"), QStringLiteral("password"), 43);
    compareFile(r.result, QStringLiteral("login"));
}

void RocketChatMessageTest::shouldLoginProvider()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.loginProvider(QStringLiteral("credentialToken"), QStringLiteral("credentialSecret"), 43);
    compareFile(r.result, QStringLiteral("loginProvider"));
}

void RocketChatMessageTest::inputChannelAutocomplete()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.inputChannelAutocomplete(QStringLiteral("pattern"), QStringLiteral("exception"), 43);
    compareFile(r.result, QStringLiteral("inputChannelAutocomplete"));
}

void RocketChatMessageTest::inputUserAutocomplete()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.inputUserAutocomplete(QStringLiteral("pattern"), QStringLiteral("exception"), 43);
    compareFile(r.result, QStringLiteral("inputUserAutocomplete"));
}

void RocketChatMessageTest::blockUser()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.blockUser(QStringLiteral("userId"), 43);
    compareFile(r.result, QStringLiteral("blockUser"));
}

void RocketChatMessageTest::unBlockUser()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.unblockUser(QStringLiteral("userId"), 43);
    compareFile(r.result, QStringLiteral("unBlockUser"));
}
