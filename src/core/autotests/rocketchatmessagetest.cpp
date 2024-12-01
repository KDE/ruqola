/*
   SPDX-FileCopyrightText: 2017-2024 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "rocketchatmessagetest.h"
#include "rocketchatmessage.h"
#include "ruqola_autotest_helper.h"

QTEST_GUILESS_MAIN(RocketChatMessageTest)

RocketChatMessageTest::RocketChatMessageTest(QObject *parent)
    : QObject(parent)
{
}

void RocketChatMessageTest::compareFile(const QString &data, const QString &name)
{
    AutoTestHelper::compareFile(QStringLiteral("/method/"), data.toUtf8(), name);
}

void RocketChatMessageTest::shouldSetDefaultStatus()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.setDefaultStatus(User::PresenceStatus::Busy, 42);
    compareFile(r.result, QStringLiteral("defaultstatusbusy"));

    r = m.setDefaultStatus(User::PresenceStatus::Away, 43);
    compareFile(r.result, QStringLiteral("defaultstatusaway"));

    r = m.setDefaultStatus(User::PresenceStatus::Offline, 44);
    compareFile(r.result, QStringLiteral("defaultstatusoffline"));

    r = m.setDefaultStatus(User::PresenceStatus::Online, 45);
    compareFile(r.result, QStringLiteral("defaultstatusonline"));
}

void RocketChatMessageTest::shouldInformTypingStatus()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.informTypingStatus(QByteArray("roomId"), QStringLiteral("user1"), false, 42);
    compareFile(r.result, QStringLiteral("informtypingstatusfalse"));

    r = m.informTypingStatus(QByteArray("roomId2"), QStringLiteral("user2"), true, 43);
    compareFile(r.result, QStringLiteral("informtypingstatustrue"));
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
    RocketChatMessage::RocketChatMessageResult r = m.searchRoomUsers(QByteArray(), QStringLiteral("room"), QStringLiteral("foo,bla"), false, true, 43);
    compareFile(r.result, QStringLiteral("spotlight-without-users"));
}

void RocketChatMessageTest::shouldSplotlightWithoutRooms()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.searchRoomUsers(QByteArray(), QStringLiteral("room"), QStringLiteral("foo,bla"), true, false, 43);
    compareFile(r.result, QStringLiteral("spotlight-without-rooms"));
}

void RocketChatMessageTest::inputChannelAutocomplete()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.inputChannelAutocomplete(QByteArray(), QStringLiteral("pattern"), QStringLiteral("exception"), 43);
    compareFile(r.result, QStringLiteral("inputChannelAutocomplete"));
}

void RocketChatMessageTest::inputUserAutocomplete()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.inputUserAutocomplete(QByteArray(), QStringLiteral("pattern"), QStringLiteral("exception"), 43);
    compareFile(r.result, QStringLiteral("inputUserAutocomplete"));
}

void RocketChatMessageTest::inputUserWithRoomIdAutocomplete()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.inputUserAutocomplete(QByteArray("RoomId25"), QStringLiteral("pattern"), QStringLiteral("exception"), 43);
    compareFile(r.result, QStringLiteral("inputUserWithRoomIdAutocomplete"));
}

void RocketChatMessageTest::blockUser()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.blockUser(QStringLiteral("rid"), QStringLiteral("userId"), 43);
    compareFile(r.result, QStringLiteral("blockUser"));
}

void RocketChatMessageTest::setAdminStatus()
{
    {
        RocketChatMessage m;
        m.setJsonFormat(QJsonDocument::Indented);
        RocketChatMessage::RocketChatMessageResult r = m.setAdminStatus(QByteArrayLiteral("userId"), false, 43);
        compareFile(r.result, QStringLiteral("setAdminStatusFalse"));
    }
    {
        RocketChatMessage m;
        m.setJsonFormat(QJsonDocument::Indented);
        RocketChatMessage::RocketChatMessageResult r = m.setAdminStatus(QByteArrayLiteral("userId"), false, 43);
        compareFile(r.result, QStringLiteral("setAdminStatusTrue"));
    }
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
        m.streamNotifyUserOtrAcknowledge(QByteArrayLiteral("userFrom"), QByteArrayLiteral("userTo"), QStringLiteral("publickeys"), 43);
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
    RocketChatMessage::RocketChatMessageResult r = m.deleteFileMessage(QByteArrayLiteral("fileid"), 43);
    compareFile(r.result, QStringLiteral("deleteFileMessage"));
}

void RocketChatMessageTest::deleteCustomSound()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.deleteCustomSound(QByteArrayLiteral("identifier"), 43);
    compareFile(r.result, QStringLiteral("deleteCustomSound"));
}

void RocketChatMessageTest::enable2fa()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.enable2fa(43);
    compareFile(r.result, QStringLiteral("enable2fa"));
}

void RocketChatMessageTest::disable2fa()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.disable2fa(QStringLiteral("bla"), 43);
    compareFile(r.result, QStringLiteral("disable2fa"));
}

void RocketChatMessageTest::validateTempToken2fa()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.validateTempToken2fa(QStringLiteral("bla"), 43);
    compareFile(r.result, QStringLiteral("validateTempToken2fa"));
}

void RocketChatMessageTest::regenerateCodes2fa()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.regenerateCodes2fa(QStringLiteral("bla"), 43);
    compareFile(r.result, QStringLiteral("regenerateCodes2fa"));
}

void RocketChatMessageTest::openRoom()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.openRoom(QByteArrayLiteral("bla"), 43);
    compareFile(r.result, QStringLiteral("openroom"));
}

void RocketChatMessageTest::bannerDismiss()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.bannerDismiss(QByteArrayLiteral("banner_read"), 43);
    compareFile(r.result, QStringLiteral("bannerDismiss"));
}

void RocketChatMessageTest::licenseGetModules()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.licenseGetModules(43);
    compareFile(r.result, QStringLiteral("licenseGetModules"));
}

void RocketChatMessageTest::videoConferenceAccepted()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);

    RocketChatMessage::RocketChatMessageResult r = m.videoConferenceAccepted(QStringLiteral("YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"),
                                                                             QStringLiteral("639976cb31e2ca494753c439"),
                                                                             QStringLiteral("YbwG4T2uB3wZSZSKB"),
                                                                             43);
    compareFile(r.result, QStringLiteral("videoConferenceAccepted"));
}

void RocketChatMessageTest::videoConferenceRejected()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);

    RocketChatMessage::RocketChatMessageResult r = m.videoConferenceRejected(QStringLiteral("YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"),
                                                                             QStringLiteral("639976cb31e2ca494753c439"),
                                                                             QStringLiteral("YbwG4T2uB3wZSZSKB"),
                                                                             43);
    compareFile(r.result, QStringLiteral("videoConferenceRejected"));
}

void RocketChatMessageTest::videoConferenceCall()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);

    RocketChatMessage::RocketChatMessageResult r = m.videoConferenceCall(QStringLiteral("YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"),
                                                                         QStringLiteral("639976cb31e2ca494753c439"),
                                                                         QStringLiteral("YbwG4T2uB3wZSZSKB"),
                                                                         43);
    compareFile(r.result, QStringLiteral("videoConferenceCall"));
}

void RocketChatMessageTest::videoConferenceConfirmed()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);

    RocketChatMessage::RocketChatMessageResult r = m.videoConferenceConfirmed(QStringLiteral("YbwG4T2uB3wZSZSKBxkNpoB3T98EEPCj2K"),
                                                                              QStringLiteral("639976cb31e2ca494753c439"),
                                                                              QStringLiteral("YbwG4T2uB3wZSZSKB"),
                                                                              43);
    compareFile(r.result, QStringLiteral("videoConferenceConfirmed"));
}

void RocketChatMessageTest::getRoomByTypeAndName()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);

    RocketChatMessage::RocketChatMessageResult r = m.getRoomByTypeAndName(QByteArrayLiteral("oHfocsBTwc9bSdAxt"), QStringLiteral("c"), 43);
    compareFile(r.result, QStringLiteral("getRoomByTypeAndName"));
}

#include "moc_rocketchatmessagetest.cpp"
