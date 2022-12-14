/*
   SPDX-FileCopyrightText: 2017-2022 Laurent Montel <montel@kde.org>

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
    void shouldJoinRoom();
    void shouldSetDefaultStatus();
    void shouldInformTypingStatus();

    void shouldUserAutoComplete();
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

    void blockUser();
    void unBlockUser();
    void deleteFileMessage();

    void streamNotifyUserOtrEnd();
    void streamNotifyUserOtrHandshake();
    void streamNotifyUserOtrAcknowledge();

    void deleteCustomSound();
    void listCustomSounds();

    void enable2fa();
    void disable2fa();
    void validateTempToken2fa();
    void regenerateCodes2fa();

    void deleteOAuthApp();
    void addOAuthApp();
    void updateOAuthApp();

    void setAdminStatus();
    void openRoom();
    void getroombyid();

    void bannerDismiss();

    void licenseGetModules();

    void videoConferenceAccepted();
    void videoConferenceRejected();

private:
    void compareFile(const QString &data, const QString &name);
};
