/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
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

void RocketChatMessageTest::setAdminStatus()
{
    {
        RocketChatMessage m;
        m.setJsonFormat(QJsonDocument::Indented);
        RocketChatMessage::RocketChatMessageResult r = m.setAdminStatus(QStringLiteral("userId"), false, 43);
        compareFile(r.result, QStringLiteral("setAdminStatusFalse"));
    }
    {
        RocketChatMessage m;
        m.setJsonFormat(QJsonDocument::Indented);
        RocketChatMessage::RocketChatMessageResult r = m.setAdminStatus(QStringLiteral("userId"), false, 43);
        compareFile(r.result, QStringLiteral("setAdminStatusTrue"));
    }
}

void RocketChatMessageTest::deleteOAuthApp()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.deleteOAuthApp(QStringLiteral("oauthappid"), 43);
    compareFile(r.result, QStringLiteral("deleteOAuthApp"));
}

void RocketChatMessageTest::addOAuthApp()
{
    {
        RocketChatMessage m;
        m.setJsonFormat(QJsonDocument::Indented);
        RocketChatMessage::RocketChatMessageResult r = m.addOAuthApp(QStringLiteral("oauthappname"), true, QStringLiteral("url"), 43);
        compareFile(r.result, QStringLiteral("addOAuthApp"));
    }
    {
        RocketChatMessage m;
        m.setJsonFormat(QJsonDocument::Indented);
        RocketChatMessage::RocketChatMessageResult r = m.addOAuthApp(QStringLiteral("oauthappname"), false, QStringLiteral("url"), 43);
        compareFile(r.result, QStringLiteral("addOAuthApp-disabled"));
    }
}

void RocketChatMessageTest::updateOAuthApp()
{
    // TODO
    {
        RocketChatMessage m;
        m.setJsonFormat(QJsonDocument::Indented);
        RocketChatMessage::RocketChatMessageResult r = m.addOAuthApp(QStringLiteral("updateoauthappname"), true, QStringLiteral("url"), 43);
        compareFile(r.result, QStringLiteral("updateOAuthApp"));
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
    RocketChatMessage::RocketChatMessageResult r = m.openRoom(QStringLiteral("bla"), 43);
    compareFile(r.result, QStringLiteral("openroom"));
}

void RocketChatMessageTest::getroombyid()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.getRoomById(QStringLiteral("bla"), 43);
    compareFile(r.result, QStringLiteral("getroombyid"));
}

void RocketChatMessageTest::bannerDismiss()
{
    RocketChatMessage m;
    m.setJsonFormat(QJsonDocument::Indented);
    RocketChatMessage::RocketChatMessageResult r = m.bannerDismiss(QStringLiteral("banner_read"), 43);
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
