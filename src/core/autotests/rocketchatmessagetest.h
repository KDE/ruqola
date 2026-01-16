/*
   SPDX-FileCopyrightText: 2017-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QObject>

class RocketChatMessageTest : public QObject
{
    Q_OBJECT
public:
    explicit RocketChatMessageTest(QObject *parent = nullptr);
    ~RocketChatMessageTest() override = default;
private Q_SLOTS:
    void shouldSetDefaultStatus();
    void shouldInformTypingStatus();

    void shouldSendFileMessage();

#if 0
    RocketChatMessage::RocketChatMessageResult RocketChatMessage::getUsersOfRoom(const QString &roomId, bool showAll, quint64 id)
    RocketChatMessage::RocketChatMessageResult setRoomHasSystemMessages(const QString &roomId, bool systemMessages, quint64 id);
    RocketChatMessage::RocketChatMessageResult readMessages(const QString &roomID, quint64 id);
    RocketChatMessage::RocketChatMessageResult getSubscriptions(const QDateTime &lastUpdate, quint64 id);
#endif

    void shouldSplotlightWithoutUsers();
    void shouldSplotlightWithoutRooms();

    void inputChannelAutocomplete();

    void inputUserAutocomplete();
    void inputUserWithRoomIdAutocomplete();

    void blockUser();
    void unBlockUser();

    void streamNotifyUserOtrEnd();
    void streamNotifyUserOtrHandshake();
    void streamNotifyUserOtrAcknowledge();

    void deleteCustomSound();
    void listCustomSounds();

    void enable2fa();
    void disable2fa();
    void validateTempToken2fa();
    void regenerateCodes2fa();

    void setAdminStatus();
    void openRoom();

    void bannerDismiss();

    void videoConferenceAccepted();
    void videoConferenceRejected();
    void videoConferenceCall();
    void videoConferenceConfirmed();

    void getRoomByTypeAndName();

private:
    void compareFile(const QString &data, const QString &name);
};
