/*
   SPDX-FileCopyrightText: 2022-2025 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include "user.h"
#include <QDebug>
#include <QList>

class LIBRUQOLACORE_EXPORT VideoConferenceInfo
{
    Q_GADGET
public:
    enum class VideoConferenceType : uint8_t {
        Unknown,
        Direct,
        Conference,
    };
    Q_ENUM(VideoConferenceType)

    VideoConferenceInfo();
    ~VideoConferenceInfo();
    void parse(const QJsonObject &content);

    [[nodiscard]] QString url() const;
    void setUrl(const QString &newUrl);

    [[nodiscard]] int status() const;
    void setStatus(int newStatus);

    [[nodiscard]] bool ringing() const;
    void setRinging(bool newRinging);

    [[nodiscard]] QString roomId() const;
    void setRoomId(const QString &newRoomId);

    [[nodiscard]] qint64 createdAtDateTime() const;
    void setCreatedAtDateTime(qint64 newCreatedAtDateTime);

    [[nodiscard]] qint64 endedAtDateTime() const;
    void setEndedAtDateTime(qint64 newEndedAtDateTime);

    [[nodiscard]] VideoConferenceType conferenceType() const;
    void setConferenceType(VideoConferenceType newConferenceType);

    [[nodiscard]] QString providerName() const;
    void setProviderName(const QString &newProviderName);

    [[nodiscard]] bool operator==(const VideoConferenceInfo &other) const;

    [[nodiscard]] QList<User> users() const;
    void setUsers(const QList<User> &newUsers);

    [[nodiscard]] QString statusInformation() const;

    [[nodiscard]] QString messageId() const;
    void setMessageId(const QString &newMessageId);

    [[nodiscard]] QString blockId() const;
    void setBlockId(const QString &newBlockId);

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] bool canJoin() const;

    [[nodiscard]] QString title() const;

    static QJsonObject serialize(const VideoConferenceInfo &videoConfInfo);
    static VideoConferenceInfo deserialize(const QJsonObject &o);

    [[nodiscard]] static QString convertEnumToString(const VideoConferenceInfo &info);

private:
    [[nodiscard]] LIBRUQOLACORE_NO_EXPORT VideoConferenceInfo::VideoConferenceType convertTypeToEnum(const QString &str) const;
    QList<User> mUsers;
    QString mBlockId;
    QString mUrl;
    QString mRoomId;
    QString mProviderName;
    QString mMessageId;
    VideoConferenceType mConferenceType = VideoConferenceType::Unknown;
    qint64 mCreatedAtDateTime = -1;
    qint64 mEndedAtDateTime = -1;
    int mStatus = -1;
    bool mRinging = false;
};
Q_DECLARE_TYPEINFO(VideoConferenceInfo, Q_RELOCATABLE_TYPE);
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const VideoConferenceInfo &t);
