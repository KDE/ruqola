/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchatmessagetest.h"
#include "rocketchatmessage.h"
#include "ruqola_autotest_helper.h"
#include <QTest>
using namespace Qt::Literals::StringLiterals;
QTEST_GUILESS_MAIN(RocketChatMessageTest)

RocketChatMessageTest::RocketChatMessageTest(QObject *parent)
    : QObject(parent)
{
}

void RocketChatMessageTest::compareFile(const QString &data, const QString &name)
{
    AutoTestHelper::compareFile(u"/method/"_s, data.toUtf8(), name);
}

void RocketChatMessageTest::shouldSetDefaultStatus()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.setDefaultStatus(User::PresenceStatus::Busy, 42);
    compareFile(r.result, u"defaultstatusbusy"_s);

    r = m.setDefaultStatus(User::PresenceStatus::Away, 43);
    compareFile(r.result, u"defaultstatusaway"_s);

    r = m.setDefaultStatus(User::PresenceStatus::Offline, 44);
    compareFile(r.result, u"defaultstatusoffline"_s);

    r = m.setDefaultStatus(User::PresenceStatus::Online, 45);
    compareFile(r.result, u"defaultstatusonline"_s);
}

void RocketChatMessageTest::shouldInformTypingStatus()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.informTypingStatus(QByteArray("roomId"), u"user1"_s, false, 42);
    compareFile(r.result, u"informtypingstatusfalse"_s);

    r = m.informTypingStatus(QByteArray("roomId2"), u"user2"_s, true, 43);
    compareFile(r.result, u"informtypingstatustrue"_s);
}

// TODO fix me
void RocketChatMessageTest::shouldSendFileMessage()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    // FIXME
    // RocketChatMessage::RocketChatMessageResult r = m.sendFileMessage(43);
    // compareFile(r.result, u"sendFileMessage"_s);
}

void RocketChatMessageTest::shouldSplotlightWithoutUsers()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.searchRoomUsers(QByteArray(), u"room"_s, u"foo,bla"_s, false, true, 43);
    compareFile(r.result, u"spotlight-without-users"_s);
}

void RocketChatMessageTest::shouldSplotlightWithoutRooms()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.searchRoomUsers(QByteArray(), u"room"_s, u"foo,bla"_s, true, false, 43);
    compareFile(r.result, u"spotlight-without-rooms"_s);
}

void RocketChatMessageTest::inputChannelAutocomplete()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.inputChannelAutocomplete(QByteArray(), u"pattern"_s, u"exception"_s, 43);
    compareFile(r.result, u"inputChannelAutocomplete"_s);
}

void RocketChatMessageTest::inputUserAutocomplete()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.inputUserAutocomplete(QByteArray(), u"pattern"_s, u"exception"_s, 43);
    compareFile(r.result, u"inputUserAutocomplete"_s);
}

void RocketChatMessageTest::inputUserWithRoomIdAutocomplete()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.inputUserAutocomplete(QByteArray("RoomId25"), u"pattern"_s, u"exception"_s, 43);
    compareFile(r.result, u"inputUserWithRoomIdAutocomplete"_s);
}

void RocketChatMessageTest::blockUser()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.blockUser(u"rid"_s, u"userId"_s, 43);
    compareFile(r.result, u"blockUser"_s);
}

void RocketChatMessageTest::setAdminStatus()
{
    {
        RocketChatMessage m;
        m.setJsonFormat(QJsonDocument::Indented);
        RocketChatMessage::RocketChatMessageResult r = m.setAdminStatus("userId"_ba, false, 43);
        compareFile(r.result, u"setAdminStatusFalse"_s);
    }
    {
        RocketChatMessage m;
        m.setJsonFormat(QJsonDocument::Indented);
        RocketChatMessage::RocketChatMessageResult r = m.setAdminStatus("userId"_ba, false, 43);
        compareFile(r.result, u"setAdminStatusTrue"_s);
    }
}

void RocketChatMessageTest::unBlockUser()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.unblockUser(u"rid"_s, u"userId"_s, 43);
    compareFile(r.result, u"unBlockUser"_s);
}

void RocketChatMessageTest::streamNotifyUserOtrEnd()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.streamNotifyUserOtrEnd(u"userFrom"_s, u"userTo"_s, 43);
    compareFile(r.result, u"streamNotifyUserOtrEnd"_s);
}

void RocketChatMessageTest::streamNotifyUserOtrHandshake()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.streamNotifyUserOtrHandshake(u"userFrom"_s, u"userTo"_s, u"publickeys"_s, 43);
    compareFile(r.result, u"streamNotifyUserOtrHandshake"_s);
}

void RocketChatMessageTest::streamNotifyUserOtrAcknowledge()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.streamNotifyUserOtrAcknowledge(QByteArrayLiteral("userFrom"), "userTo"_ba, u"publickeys"_s, 43);
    compareFile(r.result, u"streamNotifyUserOtrAcknowledge"_s);
}

void RocketChatMessageTest::listCustomSounds()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.listCustomSounds(43);
    compareFile(r.result, u"listCustomSounds"_s);
}

void RocketChatMessageTest::deleteCustomSound()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.deleteCustomSound("identifier"_ba, 43);
    compareFile(r.result, u"deleteCustomSound"_s);
}

void RocketChatMessageTest::enable2fa()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.enable2fa(43);
    compareFile(r.result, u"enable2fa"_s);
}

void RocketChatMessageTest::disable2fa()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.disable2fa(u"bla"_s, 43);
    compareFile(r.result, u"disable2fa"_s);
}

void RocketChatMessageTest::validateTempToken2fa()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.validateTempToken2fa(u"bla"_s, 43);
    compareFile(r.result, u"validateTempToken2fa"_s);
}

void RocketChatMessageTest::regenerateCodes2fa()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.regenerateCodes2fa(u"bla"_s, 43);
    compareFile(r.result, u"regenerateCodes2fa"_s);
}

void RocketChatMessageTest::openRoom()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.openRoom("bla"_ba, 43);
    compareFile(r.result, u"openroom"_s);
}

void RocketChatMessageTest::bannerDismiss()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.bannerDismiss("banner_read"_ba, 43);
    compareFile(r.result, u"bannerDismiss"_s);
}

void RocketChatMessageTest::videoConferenceAccepted()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);

    RocketChatMessage::RocketChatMessageResult r =
        m.videoConferenceAccepted(u"YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"_s, u"639976cb31e2ca494753c439"_s, u"YbwG4T2uB3wZSZSKB"_s, 43);
    compareFile(r.result, u"videoConferenceAccepted"_s);
}

void RocketChatMessageTest::videoConferenceRejected()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);

    RocketChatMessage::RocketChatMessageResult r =
        m.videoConferenceRejected(u"YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"_s, u"639976cb31e2ca494753c439"_s, u"YbwG4T2uB3wZSZSKB"_s, 43);
    compareFile(r.result, u"videoConferenceRejected"_s);
}

void RocketChatMessageTest::videoConferenceCall()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);

    RocketChatMessage::RocketChatMessageResult r =
        m.videoConferenceCall(u"YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"_s, u"639976cb31e2ca494753c439"_s, u"YbwG4T2uB3wZSZSKB"_s, 43);
    compareFile(r.result, u"videoConferenceCall"_s);
}

void RocketChatMessageTest::videoConferenceConfirmed()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);

    RocketChatMessage::RocketChatMessageResult r =
        m.videoConferenceConfirmed(u"YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"_s, u"639976cb31e2ca494753c439"_s, u"YbwG4T2uB3wZSZSKB"_s, 43);
    compareFile(r.result, u"videoConferenceConfirmed"_s);
}

void RocketChatMessageTest::getRoomByTypeAndName()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);

    RocketChatMessage::RocketChatMessageResult r = m.getRoomByTypeAndName("oHfocsBTwc9bSdAxt"_ba, u"c"_s, 43);
    compareFile(r.result, u"getRoomByTypeAndName"_s);
}

#include "moc_rocketchatmessagetest.cpp"
