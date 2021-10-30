/*
   Copyright (c) 2017-2021 Laurent Montel <montel@kde.org>

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
#include "ruqola_autotest_helper.h"
#include <QTest>
QTEST_GUILESS_MAIN(RocketChatMessageTest)

RocketChatMessageTest::RocketChatMessageTest(QObject *parent)
    : QObject(parent)
{
}

void RocketChatMessageTest::compareFile(const QString &data, const QString &name)
{
    AutoTestHelper::compareFile(QStringLiteral("/method/"), data.toUtf8(), name);
}

void RocketChatMessageTest::shouldJoinRoom()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.joinRoom(QStringLiteral("foo"), QStringLiteral("access_code"), 43);
    compareFile(r.result, QStringLiteral("joinroom"));
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

void RocketChatMessageTest::shouldUserAutoComplete()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.userAutocomplete(QStringLiteral("foo"), QStringLiteral("exception"), 43);
    compareFile(r.result, QStringLiteral("userAutocomplete"));

    r = m.userAutocomplete(QStringLiteral("foo"), QStringLiteral("foo,bla,bli"), 43);
    compareFile(r.result, QStringLiteral("userAutocomplete-exceptions"));
}

// TODO fix me
void RocketChatMessageTest::shouldSendFileMessage()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    // FIXME
    // RocketChatMessage::RocketChatMessageResult r = m.sendFileMessage(43);
    // compareFile(r.result, QStringLiteral("sendFileMessage"));
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
    RocketChatMessage::RocketChatMessageResult r = m.blockUser(QStringLiteral("rid"), QStringLiteral("userId"), 43);
    compareFile(r.result, QStringLiteral("blockUser"));
}

void RocketChatMessageTest::unBlockUser()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.unblockUser(QStringLiteral("rid"), QStringLiteral("userId"), 43);
    compareFile(r.result, QStringLiteral("unBlockUser"));
}

void RocketChatMessageTest::streamNotifyUserOtrEnd()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.streamNotifyUserOtrEnd(QStringLiteral("userFrom"), QStringLiteral("userTo"), 43);
    compareFile(r.result, QStringLiteral("streamNotifyUserOtrEnd"));
}

void RocketChatMessageTest::streamNotifyUserOtrHandshake()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r =
        m.streamNotifyUserOtrHandshake(QStringLiteral("userFrom"), QStringLiteral("userTo"), QStringLiteral("publickeys"), 43);
    compareFile(r.result, QStringLiteral("streamNotifyUserOtrHandshake"));
}

void RocketChatMessageTest::streamNotifyUserOtrAcknowledge()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r =
        m.streamNotifyUserOtrAcknowledge(QStringLiteral("userFrom"), QStringLiteral("userTo"), QStringLiteral("publickeys"), 43);
    compareFile(r.result, QStringLiteral("streamNotifyUserOtrAcknowledge"));
}

void RocketChatMessageTest::listCustomSounds()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.listCustomSounds(43);
    compareFile(r.result, QStringLiteral("listCustomSounds"));
}

void RocketChatMessageTest::deleteFileMessage()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.deleteFileMessage(QStringLiteral("fileid"), 43);
    compareFile(r.result, QStringLiteral("deleteFileMessage"));
}

void RocketChatMessageTest::deleteCustomSound()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.deleteCustomSound(QStringLiteral("identifier"), 43);
    compareFile(r.result, QStringLiteral("deleteCustomSound"));
}
