/*
   SPDX-FileCopyrightText: 2023-2026 Laurent Montel <montel@kde.org>

   SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include "libruqolacore_export.h"
#include <QSharedData>
#include <QString>
class QJsonObject;
class LIBRUQOLACORE_EXPORT ModerationMessage : public QSharedData
{
public:
    explicit ModerationMessage(const ModerationMessage &other);
    ModerationMessage();
    ~ModerationMessage();

    [[nodiscard]] QByteArray moderationId() const;
    void setModerationId(const QByteArray &newModerationId);

    void parse(const QJsonObject &obj);

    [[nodiscard]] QByteArray roomId() const;
    void setRoomId(const QByteArray &newRoomId);

    [[nodiscard]] QString roomName() const;
    void setRoomName(const QString &newRoomName);

    [[nodiscard]] QString roomFName() const;
    void setRoomFName(const QString &newRoomFName);

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] bool operator==(const ModerationMessage &other) const;

private:
    LIBRUQOLACORE_NO_EXPORT void parseRoom(const QJsonObject &roomObj);
    QByteArray mModerationId;
    QByteArray mRoomId;
    QString mRoomName;
    QString mRoomFName;
};
class QDebug;
LIBRUQOLACORE_EXPORT QDebug operator<<(QDebug d, const ModerationMessage &t);
